/*
 * Copyright (c) 2006-2025, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-08     stackyuan    first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <drv_common.h>

/* Definitions of environment analog values */
  /* Value of analog reference voltage (Vref+), connected to analog voltage   */
  /* supply Vdda (unit: mV).                                                  */
  #define VDDA_APPLI                       (3300UL)

/* Definitions of data related to this example */
  /* Init variable out of expected ADC conversion data range */
  #define VAR_CONVERTED_DATA_INIT_VALUE    (__LL_ADC_DIGITAL_SCALE(LL_ADC_RESOLUTION_12B) + 1)


const static uint16_t hardware_vol_rank[8] =
{
  50,   // less than 50mV, fail
  75,   // 50-75 (normally 65mV) HW=0.1, BOM=0.1
  90,
  65535,
  65535,
  65535,
  65535,
  65535,
};

const static uint16_t hardware_ver_rank[8] =
{
  0x0101, //HW 0.1, BOM 0.1
  0x0201,
  0x0202,
  0x1010,
  0x1011,
  0xFFFF,
  0xFFFF,
  0xFFFF,
};


/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void)
{

    /* USER CODE BEGIN ADC1_Init 0 */

    /* USER CODE END ADC1_Init 0 */

    ADC_MultiModeTypeDef multimode = {0};
    ADC_ChannelConfTypeDef sConfig = {0};

    /* USER CODE BEGIN ADC1_Init 1 */

    /* USER CODE END ADC1_Init 1 */

    /** Common config
     */
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    hadc1.Init.LowPowerAutoWait = DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.SamplingMode = ADC_SAMPLING_MODE_NORMAL;
    hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
    hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
    hadc1.Init.OversamplingMode = DISABLE;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure the ADC multi-mode
     */
    multimode.Mode = ADC_MODE_INDEPENDENT;
    if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Regular Channel
     */
    sConfig.Channel = ADC_CHANNEL_15;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;
    sConfig.OffsetSign = ADC_OFFSET_SIGN_NEGATIVE;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC1_Init 2 */

    /* USER CODE END ADC1_Init 2 */
}

uint32_t detect_hardware_ver()
{
    /* Variables for ADC conversion data */
    __IO uint16_t uhADCxConvertedData = VAR_CONVERTED_DATA_INIT_VALUE; /* ADC group regular conversion data */

    /* Variables for ADC conversion data computation to physical values */
    uint16_t uhADCxConvertedData_Voltage_mVolt = 0; /* Value of voltage calculated from ADC conversion data (unit: mV) */
    uint16_t hardware_ver = 0;
    /* Initialize ADC1 configured peripherals */
    MX_ADC1_Init();

    /* Perform ADC calibration */
    if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) != HAL_OK)
    {
        /* Calibration Error */
        Error_Handler();
    }
    /* Start ADC group regular conversion */
    if (HAL_ADC_Start(&hadc1) != HAL_OK)
    {
        /* ADC conversion start error */
        Error_Handler();
    }
    /* Wait for ADC conversion completed */
    if (HAL_ADC_PollForConversion(&hadc1, 10) != HAL_OK)
    {
        /* End Of Conversion flag not set on time */
        Error_Handler();
    }

    /* Retrieve ADC conversion data */
    uhADCxConvertedData = HAL_ADC_GetValue(&hadc1);

    /* Computation of ADC conversions raw data to physical values             */
    /* using helper macro.                                                    */
    uhADCxConvertedData_Voltage_mVolt = __LL_ADC_CALC_DATA_TO_VOLTAGE(VDDA_APPLI, uhADCxConvertedData, LL_ADC_RESOLUTION_12B);

    if (uhADCxConvertedData_Voltage_mVolt < hardware_vol_rank[0])
    {
        LOG_E("no hardware version detected on GPIO PA3");
    }
    else
    {
        for (uint8_t i = 0; i < sizeof(hardware_vol_rank) / sizeof(uint16_t) - 1; i++)
        {
            if ((uhADCxConvertedData_Voltage_mVolt >= hardware_vol_rank[i]) && (uhADCxConvertedData_Voltage_mVolt <= hardware_vol_rank[i + 1]))
            {
                hardware_ver = hardware_ver_rank[i];
                LOG_I("Hardware Version: %d.%d, BoM Version: %d.%d\n", ((hardware_ver & 0xF000) >> 12), ((hardware_ver & 0x0F00) >> 8), ((hardware_ver & 0x00F0) >> 4), (hardware_ver & 0x000F));
                break;
            }
        }
    }

    rt_kprintf("ADC read %d, voltage %d\n", uhADCxConvertedData, uhADCxConvertedData_Voltage_mVolt);

    // ToDO: deinit ADC
}
