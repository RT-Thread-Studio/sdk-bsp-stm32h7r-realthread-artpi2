/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-7      SummerGift   first version
 */

#include "drv_common.h"
#include "board.h"

#ifdef RT_USING_FINSH
#include <finsh.h>
static void reboot(uint8_t argc, char **argv)
{
    rt_hw_cpu_reset();
}
FINSH_FUNCTION_EXPORT_ALIAS(reboot, __cmd_reboot, Reboot System);
#endif /* RT_USING_FINSH */

/* SysTick configuration */
void rt_hw_systick_init(void)
{
#if defined (SOC_SERIES_STM32H7RS)
    HAL_SYSTICK_Config((HAL_RCC_GetSysClockFreq()) / RT_TICK_PER_SECOND);
#else
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / RT_TICK_PER_SECOND);
#endif
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    NVIC_SetPriority(SysTick_IRQn, 0xFF);
}

/**
 * This is the timer interrupt service routine.
 *
 */
void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_IncTick();
    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}

uint32_t HAL_GetTick(void)
{
    return rt_tick_get() * 1000 / RT_TICK_PER_SECOND;
}

void HAL_SuspendTick(void)
{
}

void HAL_ResumeTick(void)
{
}

void HAL_Delay(__IO uint32_t Delay)
{
    rt_thread_mdelay(Delay);
}

/* re-implement tick interface for STM32 HAL */
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    /* Return function status */
    return HAL_OK;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler */
    /* User can add his own implementation to report the HAL error return state */
    while (1)
    {
    }
    /* USER CODE END Error_Handler */
}

/**
 * This function will delay for some us.
 *
 * @param us the delay time of us
 */
void rt_hw_us_delay(rt_uint32_t us)
{
    rt_uint32_t start, now, delta, reload, us_tick;
    start = SysTick->VAL;
    reload = SysTick->LOAD;
    us_tick = SystemCoreClock / 1000000UL;
    do
    {
        now = SysTick->VAL;
        delta = start > now ? start - now : reload + start - now;
    }
    while (delta < us_tick * us);
}

/**
 * This function will initial STM32 board.
 */
void hw_board_init(char *clock_src, int32_t clock_src_freq, int32_t clock_target_freq)
{
    extern void rt_hw_systick_init(void);
    extern void clk_init(char *clk_source, int source_freq, int target_freq);

    /* Update SystemCoreClock variable according to RCC registers values. */
    SystemCoreClockUpdate();
    /* HAL_Init() function is called at the beginning of the program */
    HAL_Init();
    
    /* Peripheral clock enable */
    __HAL_RCC_HPDMA1_CLK_ENABLE();

    /* Configure the system Power Supply */
    if (HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY) != HAL_OK)
    {
        /* Initialization error */
        Error_Handler();
    }

    /* System clock initialization */
    clk_init(clock_src, clock_src_freq, clock_target_freq);

    rt_hw_systick_init();

    /* Pin driver initialization is open by default */
#ifdef RT_USING_PIN
    extern int rt_hw_pin_init(void);
    rt_hw_pin_init();
#endif

    /* USART driver initialization is open by default */
#ifdef RT_USING_SERIAL
    extern int rt_hw_usart_init(void);
    rt_hw_usart_init();
#endif

}
