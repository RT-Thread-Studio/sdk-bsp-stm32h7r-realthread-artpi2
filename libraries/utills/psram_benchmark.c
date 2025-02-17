/*
 * Copyright (c) 2006-2025, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-08     stackyuan    first version
 */

// @brief   This file provides xip benchmarks for psram.

#include <rtthread.h>
#include <board.h>

#ifdef BSP_USING_APS256XX_HSPI_PSRAM

#define DBG_TAG "b-psram"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define EXT_SDRAM_ADDR ((uint32_t)0x90000000)
#define EXT_SDRAM_SIZE (32 * 1024 * 1024)
#define TEST_ADDRESS 0 // start from zero address
#define TEST_BUF_SIZE 256

static void FillBuff(uint32_t pattern)
{

  uint32_t i;
  uint32_t *pBuf;
  uint32_t buf;
  uint32_t err_cnt;

  pBuf = (uint32_t *)EXT_SDRAM_ADDR;
  err_cnt = 0;
  for (i = 0; i < 1024 * 1024 ; i++)
  {
      *pBuf++ = pattern;
      *pBuf++ = pattern;
      *pBuf++ = pattern;
      *pBuf++ = pattern;
      *pBuf++ = pattern;
      *pBuf++ = pattern;
      *pBuf++ = pattern;
      *pBuf++ = pattern;
  }
  // rt_thread_mdelay(1);
  pBuf = (uint32_t *)EXT_SDRAM_ADDR;
  for (i = 0; i < 1024 * 1024 * 8; i++)
  {
    buf = *pBuf;
    if (buf != pattern)
    {
      err_cnt++;
      LOG_W("FillBuff read check error.offset:0x%08x, read:0x%08x,should be:0x%08x", i, buf, pattern);
      if (err_cnt >= 5)
      {
        LOG_W("FillBuff read check error more than 5 times, skip this round.\n");
        break;
      }

    }
    pBuf++;
  }
}
/**
 * @brief
 * @attention with error check
 *
 */
static void WriteSpeedTest(void)
{
    uint32_t start, end, cnt;
    uint32_t i, j;
    int32_t iTime;
    uint32_t *pBuf;



    FillBuff(0x5AA55AA5);
    FillBuff(0xA55AA55A);
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
            rt_kprintf("write check error j=%d\r\n", j);
            break;
        }
    }

    /* speed print out */
    rt_kprintf("32MB write duration: [method1]:%dms  [method2]:%d, write speed: %dMB/s\r\n",
               iTime, cnt, (EXT_SDRAM_SIZE / 1024 / 1024 * 1000) / (iTime));
}
MSH_CMD_EXPORT_ALIAS(WriteSpeedTest, testwrite, PSRAM Write Test)

/**
 * @brief
 * @attention no read error check.
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

    for (i = 1024 * 1024 / 4; i > 0; i--) //128 Byte per cycle, without error check.
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

    /* readback check */
    uint32_t j = 0;
    pBuf = (uint32_t *)EXT_SDRAM_ADDR;
    for (i = 0; i < 1024 * 1024 * 8; i++)
    {
        if (*pBuf++ != j++)
        {
            rt_kprintf("read check error j=%d\r\n", j);
            break;
        }
    }

    rt_kprintf("32MB read duration: [method1]:%dms  [method2]:%d, read speed: %dMB/s\r\n",
               iTime, cnt, (EXT_SDRAM_SIZE / 1024 / 1024 * 1000) / (iTime));
}
MSH_CMD_EXPORT_ALIAS(ReadSpeedTest, testread, PSRAM Read Test);

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
MSH_CMD_EXPORT_ALIAS(ReadWriteTest, testrw, PSRAM Read-Write Test);

void test_memburn_wr()
{
    while(1)
    {
        WriteSpeedTest();
        ReadSpeedTest();
        rt_thread_mdelay(5);
    }

}
MSH_CMD_EXPORT_ALIAS(test_memburn_wr, testburn, PSRAM benchmark);

#endif /* BSP_USING_APS256XX_HSPI_PSRAM */
