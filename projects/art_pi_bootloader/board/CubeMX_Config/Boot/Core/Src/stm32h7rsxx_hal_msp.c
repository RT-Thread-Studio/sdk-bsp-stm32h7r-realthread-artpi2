/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32h7rsxx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */
#include "drv_common.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{

  /* Configure the system Power Supply */

  if (HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY) != HAL_OK)
  {
    /* Initialization error */
    Error_Handler();
  }

  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  /* System interrupt init*/

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

/**
* @brief UART MSP Initialization
* This function configures the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(huart->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspInit 0 */

  /* USER CODE END UART4_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART234578;
    PeriphClkInit.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_UART4_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**UART4 GPIO Configuration
    PD1     ------> UART4_TX
    PD0     ------> UART4_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* USER CODE BEGIN UART4_MspInit 1 */

  /* USER CODE END UART4_MspInit 1 */
  }

}

/**
* @brief UART MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspDeInit 0 */

  /* USER CODE END UART4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART4_CLK_DISABLE();

    /**UART4 GPIO Configuration
    PD1     ------> UART4_TX
    PD0     ------> UART4_RX
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_1|GPIO_PIN_0);

  /* USER CODE BEGIN UART4_MspDeInit 1 */

  /* USER CODE END UART4_MspDeInit 1 */
  }

}

/**
* @brief XSPI MSP Initialization
* This function configures the hardware resources used in this example
* @param hxspi: XSPI handle pointer
* @retval None
*/
void HAL_XSPI_MspInit(XSPI_HandleTypeDef* hxspi)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  if(hxspi->Instance == XSPI1)
  {
//    /* Initializes the peripherals clock */
//    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_XSPI1;
//    PeriphClkInit.Xspi1ClockSelection = RCC_XSPI1CLKSOURCE_PLL2S;
//    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
//    {
//      Error_Handler();
//    }

//    /* Enable the power of XSPI */
//    HAL_PWREx_EnableXSPIM1() ;

//    /* Enable the XSPI memory interface clock */
//    __HAL_RCC_XSPI1_CLK_ENABLE();

//    /* Reset the XSPI memory interface */
//    __HAL_RCC_XSPI1_FORCE_RESET();
//    __HAL_RCC_XSPI1_RELEASE_RESET();

//    /* Enable GPIO clocks */
//    __HAL_RCC_GPIOO_CLK_ENABLE();
//    __HAL_RCC_GPIOP_CLK_ENABLE();

//    /* XSPI GPIO pin configuration  */
//    GPIO_InitStruct.Pin       = GPIO_PIN_All;
//    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull      = GPIO_NOPULL;
//    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF9_XSPIM_P1;
//    HAL_GPIO_Init(GPIOP, &GPIO_InitStruct);

//    GPIO_InitStruct.Pin       =  GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4;
//    HAL_GPIO_Init(GPIOO, &GPIO_InitStruct);

//    /* XSPI NCS GPIO pin configuration  */
//    GPIO_InitStruct.Pin       = GPIO_PIN_0;
//    GPIO_InitStruct.Pull      = GPIO_PULLUP;
//    HAL_GPIO_Init(GPIOO, &GPIO_InitStruct);

//    HAL_SBS_EnableIOSpeedOptimize(SBS_IO_XSPI1_HSLV);
  }
  else if(hxspi->Instance == XSPI2)
  {
    /* Initializes the peripherals clock */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_XSPI2;
    PeriphClkInit.Xspi2ClockSelection = RCC_XSPI2CLKSOURCE_PLL2S;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Enable the XSPI memory interface clock */
    __HAL_RCC_XSPI2_CLK_ENABLE();

    /* Reset the XSPI memory interface */
    __HAL_RCC_XSPI2_FORCE_RESET();
    __HAL_RCC_XSPI2_RELEASE_RESET();

    HAL_PWREx_EnableXSPIM2();

    __HAL_RCC_GPION_CLK_ENABLE();

    GPIO_InitStruct.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                                GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_8 | \
                                GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF9_XSPIM_P2;
    HAL_GPIO_Init(GPION, &GPIO_InitStruct);

    HAL_SBS_EnableIOSpeedOptimize(SBS_IO_XSPI2_HSLV);
  }

}

/**
* @brief XSPI MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hxspi: XSPI handle pointer
* @retval None
*/
void HAL_XSPI_MspDeInit(XSPI_HandleTypeDef* hxspi)
{
  if(hxspi->Instance==XSPI2)
  {
  /* USER CODE BEGIN XSPI2_MspDeInit 0 */

  /* USER CODE END XSPI2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_XSPIM_CLK_DISABLE();
    __HAL_RCC_XSPI2_CLK_DISABLE();
  /* USER CODE BEGIN XSPI2_MspDeInit 1 */

  /* USER CODE END XSPI2_MspDeInit 1 */
  }

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
