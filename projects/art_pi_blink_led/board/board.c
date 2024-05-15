/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-29     RealThread   first version
 */
#include <board.h>
#include <rtdevice.h>

#define DBG_TAG "board"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

void SystemClock_Config(void)
{
}

int clock_information(void)
{
    LOG_I("System Clock information");
    LOG_I("SYSCLK_Frequency = %d", HAL_RCC_GetSysClockFreq());
    LOG_I("HCLK_Frequency   = %d", HAL_RCC_GetHCLKFreq());
    LOG_I("PCLK1_Frequency  = %d", HAL_RCC_GetPCLK1Freq());
    LOG_I("PCLK2_Frequency  = %d", HAL_RCC_GetPCLK2Freq());
    LOG_I("XSPI1_Frequency  = %d", HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_XSPI1));
    LOG_I("XSPI2_Frequency  = %d", HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_XSPI2));

    return RT_EOK;
}
INIT_BOARD_EXPORT(clock_information);

#ifdef BSP_USING_PSRAM
#ifdef RT_USING_MEMHEAP_AS_HEAP
    static struct rt_memheap system_heap;
#endif

static int PSRAM_Init(void)
{
    LOG_I("psram init success, mapped at 0x%X, size is %d bytes", PSRAM_BANK_ADDR, PSRAM_SIZE);
#ifdef RT_USING_MEMHEAP_AS_HEAP
    /* If RT_USING_MEMHEAP_AS_HEAP is enabled, SDRAM is initialized to the heap */
    rt_memheap_init(&system_heap, "psram", (void *)PSRAM_BANK_ADDR, PSRAM_SIZE);
#endif
    return RT_EOK;
}
INIT_BOARD_EXPORT(PSRAM_Init);
#endif
