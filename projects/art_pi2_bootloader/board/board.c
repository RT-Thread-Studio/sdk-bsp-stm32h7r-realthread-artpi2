/*
 * Copyright (c) 2006-2025, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-29     RealThread   first version
 */

#include "board.h"

#define DBG_TAG "board"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Configure the system Power Supply */
    if (HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY) != HAL_OK)
    {
        /* Initialization error */
        Error_Handler();
    }
    /** Configure the main internal regulator output voltage
    */
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE0) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL1.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL1.PLLM = 12;
    RCC_OscInitStruct.PLL1.PLLN = 300;
    RCC_OscInitStruct.PLL1.PLLP = 1;
    RCC_OscInitStruct.PLL1.PLLQ = 9;      // for i2s1 audio
    RCC_OscInitStruct.PLL1.PLLR = 2;
    RCC_OscInitStruct.PLL1.PLLS = 2;
    RCC_OscInitStruct.PLL1.PLLT = 2;
    RCC_OscInitStruct.PLL1.PLLFractional = 0;
    RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL2.PLLSource = RCC_PLLSOURCE_HSE; //133Mhz: 3,50,2,2,2,3,2 , 146Mhz: 4,73,2,2,2,3,2
    RCC_OscInitStruct.PLL2.PLLM = 6;
    RCC_OscInitStruct.PLL2.PLLN = 100;
    RCC_OscInitStruct.PLL2.PLLP = 2;
    RCC_OscInitStruct.PLL2.PLLQ = 2;
    RCC_OscInitStruct.PLL2.PLLR = 2;
    RCC_OscInitStruct.PLL2.PLLS = 2;
    RCC_OscInitStruct.PLL2.PLLT = 3;
    RCC_OscInitStruct.PLL2.PLLFractional = 0;
    RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL3.PLLSource = RCC_PLLSOURCE_HSE; //454*454: 62--60hz, 800*480: 132--60hz
    RCC_OscInitStruct.PLL3.PLLM = 4;
    RCC_OscInitStruct.PLL3.PLLN = 132;
    RCC_OscInitStruct.PLL3.PLLP = 4;  //4:44100 7:48000
    RCC_OscInitStruct.PLL3.PLLQ = 24;
    RCC_OscInitStruct.PLL3.PLLR = 24;
    RCC_OscInitStruct.PLL3.PLLS = 2;
    RCC_OscInitStruct.PLL3.PLLT = 2;
    RCC_OscInitStruct.PLL3.PLLFractional = 0;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2
                                  | RCC_CLOCKTYPE_PCLK4 | RCC_CLOCKTYPE_PCLK5;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
    RCC_ClkInitStruct.APB5CLKDivider = RCC_APB5_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
    {
        Error_Handler();
    }

    /* Enable the power of PORT_M: PM0-PM15*/
    PWR->CSR2 |= PWR_CSR2_USB33DEN;
    /* Wait the detection of the USB33 power */
    /* Note: shall not pass when USB33 voltage lower than 2.9V */
    uint32_t tickstart = HAL_GetTick();
    while ((PWR->CSR2 & PWR_CSR2_USB33RDY) == 0U)
    {
    }
}

int clock_information(void)
{
    LOG_D("System Clock information");
    LOG_D("SYSCLK_Frequency = %d", HAL_RCC_GetSysClockFreq());
    LOG_D("HCLK_Frequency   = %d", HAL_RCC_GetHCLKFreq());
    LOG_D("PCLK1_Frequency  = %d", HAL_RCC_GetPCLK1Freq());
    LOG_D("PCLK2_Frequency  = %d", HAL_RCC_GetPCLK2Freq());
    LOG_D("PLL2S_XSPI1_2_Frequency  = %d", HAL_RCC_GetPLL2SFreq());
    LOG_D("PLL2T_Frequency  = %d", HAL_RCC_GetPLL2TFreq());
    return RT_EOK;
}
INIT_BOARD_EXPORT(clock_information);
