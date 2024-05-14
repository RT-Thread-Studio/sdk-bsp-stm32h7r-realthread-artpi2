/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-02     RT-Thread    first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"

#define LED_PIN GET_PIN(O, 1)

int main(void)
{
    rt_uint32_t count = 1;

    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);

    while(count++)
    {
        rt_thread_mdelay(500);
        rt_pin_write(LED_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED_PIN, PIN_LOW);
    }
    return RT_EOK;
}

#include "stm32h7rsxx.h"
static int vtor_config(void)
{
    /* Vector Table Relocation in Internal XSPI2_BASE */
    SCB->VTOR = XSPI2_BASE;
    return 0;
}
INIT_BOARD_EXPORT(vtor_config);

#define EXT_SDRAM_ADDR ((uint32_t)0x90000000)
#define EXT_SDRAM_SIZE (32 * 1024 * 1024)
#define TEST_ADDRESS 0 // start from zero address
#define TEST_BUF_SIZE 256

/**
 * @brief
 *
 */
static void WriteSpeedTest(void)
{
    uint32_t start, end, cnt;
    uint32_t i, j;
    int32_t iTime;
    uint32_t *pBuf;

    j = 0;
    pBuf = (uint32_t *)EXT_SDRAM_ADDR;
    start = rt_tick_get_millisecond();

    for (i = 1024 * 1024 / 4; i > 0; i--)
    {
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;

        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;

        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;

        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
    }
    end = rt_tick_get_millisecond();
    cnt = end - start;
    iTime = cnt;
    /* readback check */
    j = 0;
    pBuf = (uint32_t *)EXT_SDRAM_ADDR;
    for (i = 0; i < 1024 * 1024 * 8; i++)
    {
        if (*pBuf++ != j++)
        {
            rt_kprintf("read error j=%d\r\n", j);
            break;
        }
    }

    /* speed print out */
    rt_kprintf("32MB read duration: [method1]:%dms  [method2]:%d, write speed: %dMB/s\r\n",
               iTime, cnt, (EXT_SDRAM_SIZE / 1024 / 1024 * 1000) / (iTime));
}
MSH_CMD_EXPORT_ALIAS(WriteSpeedTest, testwrite, WriteSpeedTest)

/**
 * @brief
 *
 */
static void ReadSpeedTest(void)
{
    uint32_t start, end, cnt;
    uint32_t i;
    int32_t iTime;
    uint32_t *pBuf;
    __IO uint32_t ulTemp;

    pBuf = (uint32_t *)EXT_SDRAM_ADDR;
    start = rt_tick_get_millisecond();

    for (i = 1024 * 1024 / 4; i > 0; i--)
    {
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;

        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;

        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;

        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
    }
    end = rt_tick_get_millisecond();
    cnt = end - start;
    iTime = cnt;

    rt_kprintf("32MB write duration: [method1]:%dms  [method2]:%d, write speed: %dMB/s\r\n",
               iTime, cnt, (EXT_SDRAM_SIZE / 1024 / 1024 * 1000) / (iTime));
}
MSH_CMD_EXPORT_ALIAS(ReadSpeedTest, testread, ReadSpeedTest);

/**
 * @brief
 *
 */
static void ReadWriteTest(void)
{
    uint32_t i;
    uint32_t *pBuf;

    /* payload data 0xAAAA5555 */
    pBuf = (uint32_t *)(EXT_SDRAM_ADDR + TEST_ADDRESS);
    for (i = 0; i < TEST_BUF_SIZE; i++)
    {
        pBuf[i] = 0xAAAA5555;
    }

    rt_kprintf("physical address: %08X, size: %dbyte, display: %d details: \r\n", EXT_SDRAM_ADDR + TEST_ADDRESS, EXT_SDRAM_SIZE, TEST_BUF_SIZE * 4);

    /* print data */
    pBuf = (uint32_t *)(EXT_SDRAM_ADDR + TEST_ADDRESS);
    for (i = 0; i < TEST_BUF_SIZE; i++)
    {
        rt_kprintf(" %04X", pBuf[i]);

        if ((i & 7) == 7)
        {
            rt_kprintf("\r\n"); /* 32byte display per line */
        }
        else if ((i & 7) == 3)
        {
            rt_kprintf(" - ");
        }
    }
}
MSH_CMD_EXPORT_ALIAS(ReadWriteTest, testrw, ReadWriteTest);
