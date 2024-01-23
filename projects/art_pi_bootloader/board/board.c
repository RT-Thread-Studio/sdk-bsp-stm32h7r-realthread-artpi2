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

#define DBG_TAG "board"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

void system_clock_config(int target_freq_mhz)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Configure the system Power Supply */
  if (HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY) != HAL_OK)
  {
    /* Initialization error */
    while(1);
  }

  /* Enable voltage range 1 for VOS High level */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    /* Initialization error */
    while(1);
  }

  /* Activate PLL1 with HSI as source (HSI is ON at reset) */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL1.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL1.PLLM = 4;
  RCC_OscInitStruct.PLL1.PLLN = 25; /* PLL1 VCO = 64/4 * 25 = 400MHz */
  RCC_OscInitStruct.PLL1.PLLP = 1;  /* PLL1 P =400MHz */
  RCC_OscInitStruct.PLL1.PLLQ = 1;  /* PLL1 Q =400MHz */
  RCC_OscInitStruct.PLL1.PLLR = 1;  /* PLL1 R =400MHz */
  RCC_OscInitStruct.PLL1.PLLS = 1;  /* PLL1 S =400MHz */
  RCC_OscInitStruct.PLL1.PLLT = 1;  /* PLL1 T =400MHz */
  RCC_OscInitStruct.PLL1.PLLFractional = 0;
  RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization error */
    while(1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1, PCLK2,
     PCLK4 and PCLK5 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK  | \
                                 RCC_CLOCKTYPE_PCLK1  | RCC_CLOCKTYPE_PCLK2 | \
                                 RCC_CLOCKTYPE_PCLK4  | RCC_CLOCKTYPE_PCLK5);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;   /* System CPU clock=pll1p_ck */
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV2;     /* AXI/AHB System bus clock=System CPU clock/2 */
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;     /* APB1 bus clock=System bus clock/4 */
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;     /* APB2 bus clock=System bus clock/4 */
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;     /* APB4 bus clock=System bus clock/4 */
  RCC_ClkInitStruct.APB5CLKDivider = RCC_APB5_DIV2;     /* APB5 bus clock=System bus clock/4 */
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization error */
    while(1);
  }
//  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_LSE, RCC_MCODIV_1);
}
int clock_information(void)
{
    LOG_D("System Clock information");
    LOG_D("SYSCLK_Frequency = %d", HAL_RCC_GetSysClockFreq());
    LOG_D("HCLK_Frequency   = %d", HAL_RCC_GetHCLKFreq());
    LOG_D("PCLK1_Frequency  = %d", HAL_RCC_GetPCLK1Freq());
    LOG_D("PCLK2_Frequency  = %d", HAL_RCC_GetPCLK2Freq());

    return RT_EOK;
}
INIT_BOARD_EXPORT(clock_information);

void clk_init(char *clk_source, int source_freq, int target_freq)
{
    system_clock_config(target_freq);
}

/**
  * @brief  This function sets up the default MPU configuration with a background region
  *         preventing any accesses to sub-regions 0x60000000-0x7FFFFFFF, 0x80000000-0x9FFFFFFF
  *         and 0xC0000000-0xDFFFFFFF
  * @note : This action is strongly recommended to avoid any issue with speculative read access
  *         on the external memory area
  * @retval None
  */
static void MPU_Config(void)
{
  /* Configure the MMU to avoid any issue relative to speculative access */
  MPU_Region_InitTypeDef default_config = {
    .Enable = MPU_REGION_ENABLE,
    .Number = MPU_REGION_NUMBER0,
    .BaseAddress = 0,
    .Size = MPU_REGION_SIZE_4GB,
    .SubRegionDisable = 0xA7,
    .TypeExtField = MPU_TEX_LEVEL0,
    .AccessPermission = MPU_REGION_NO_ACCESS,
    .DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE,
    .IsShareable = MPU_ACCESS_SHAREABLE,
    .IsCacheable = MPU_ACCESS_NOT_CACHEABLE,
    .IsBufferable = MPU_ACCESS_NOT_BUFFERABLE
  };

  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Set a global region to avoid any speculative access issue */
  HAL_MPU_ConfigRegion(&default_config);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

RT_WEAK void rt_hw_board_init()
{
    extern void hw_board_init(char *clock_src, int32_t clock_src_freq, int32_t clock_target_freq);

    MPU_Config();
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
