/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-29     RealThread   first version
 */

#include <rtthread.h>
#include <board.h>
#include <drv_common.h>
#include <rtdevice.h>

#define DBG_TAG "board"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

void system_clock_config(int target_freq_mhz)
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

void clk_init(char *clk_source, int source_freq, int target_freq)
{
    system_clock_config(target_freq);
}

void rt_hw_board_init()
{
    extern void hw_board_init(char *clock_src, int32_t clock_src_freq, int32_t clock_target_freq);
    extern int mpu_init(void);
    mpu_init();
    /* Heap initialization */
#if defined(RT_USING_HEAP)
    rt_system_heap_init((void *) HEAP_BEGIN, (void *) HEAP_END);
#endif

    hw_board_init(BSP_CLOCK_SOURCE, BSP_CLOCK_SOURCE_FREQ_MHZ, BSP_CLOCK_SYSTEM_FREQ_MHZ);

    /* Set the shell console output device */
#if defined(RT_USING_DEVICE) && defined(RT_USING_CONSOLE)
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif

    /* Board underlying hardware initialization */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

}

#ifdef RT_USING_PM
/**
  * @brief  Configures system clock after wake-up from STOP: enable HSI, PLL
  *         and select PLL as system clock source.
  * @param  None
  * @retval None
  */
void SystemClock_ReConfig(uint8_t mode)
{
    switch (mode)
    {
    case PM_RUN_MODE_HIGH_SPEED:
//        SystemClock_480M();
        break;
    case PM_RUN_MODE_NORMAL_SPEED:
//        SystemClock_240M();
        break;
    case PM_RUN_MODE_MEDIUM_SPEED:
//        SystemClock_120M();
        break;
    case PM_RUN_MODE_LOW_SPEED:
//        SystemClock_60M();
        break;
    default:
        break;
    }

    // SystemClock_MSI_OFF();
}
#endif
