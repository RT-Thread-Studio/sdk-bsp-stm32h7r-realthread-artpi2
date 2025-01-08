/**
  ******************************************************************************
  * @file    Examples/BSP/Src/hspi.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the HSPI Driver
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <rtthread.h>
#include <stm32h7rsxx_hspi.h>

/** @addtogroup STM32U5xx_HAL_Examples
* @{
*/

/** @addtogroup BSP
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HSPI_BUFFER_SIZE     ((uint32_t)0x8000)
#define HSPI_WRITE_READ_ADDR ((uint32_t)0x0050)
#define HSPI_BASE_ADDR       ((uint32_t)0x90000000)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t hspi_aTxBuffer[HSPI_BUFFER_SIZE];
uint8_t hspi_aRxBuffer[HSPI_BUFFER_SIZE];
uint8_t ubHspiRamDmaCplt = 0;

/* Private function prototypes -----------------------------------------------*/
// static void     hspi_SetHint(void);
static void     Fill_Buffer (uint8_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset);
static uint8_t  Buffercmp   (uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
* @brief  HSPI RAM Demo
* @param  None
* @retval None
*/
int32_t HSPI_RAM_demo (void)
{
  int32_t result = 0;
  BSP_HSPI_RAM_Cfg_t sHSPI_Init;
  uint8_t mode;

  /* Demo Hint */
  // hspi_SetHint();

  rt_kprintf("\r\n################## Start HSPI test #####################");
  rt_kprintf("\r\n");
  for (mode = 0; mode < 5; mode++)
  {
    /********************************** PART 1 **********************************/

    /*##-1- Configure the HSPI RAM device ######################################*/
    /* HSPI RAM device configuration */
    if (mode == 0)
    {
      rt_kprintf("\r\n ---- Fixed Latency 2*3, Linear burst, x8 IO ----");
      sHSPI_Init.LatencyType      = BSP_HSPI_RAM_FIXED_LATENCY;
      sHSPI_Init.BurstType        = BSP_HSPI_RAM_LINEAR_BURST;
      sHSPI_Init.BurstLength      = BSP_HSPI_RAM_BURST_16_BYTES;
      sHSPI_Init.ReadLatencyCode  = BSP_HSPI_RAM_READ_LATENCY_3;
      sHSPI_Init.WriteLatencyCode = BSP_HSPI_RAM_WRITE_LATENCY_3;
      sHSPI_Init.IOMode           = BSP_HSPI_RAM_IO_X16_MODE;
    }
    else if (mode == 1)
    {
      rt_kprintf("\r\n ---- Variable Latency 4, Hybrid 16-bytes burst, x8 IO ----");
      sHSPI_Init.LatencyType      = BSP_HSPI_RAM_VARIABLE_LATENCY;
      sHSPI_Init.BurstType        = BSP_HSPI_RAM_LINEAR_BURST;
      sHSPI_Init.BurstLength      = BSP_HSPI_RAM_BURST_16_BYTES;
      sHSPI_Init.ReadLatencyCode  = BSP_HSPI_RAM_READ_LATENCY_4;
      sHSPI_Init.WriteLatencyCode = BSP_HSPI_RAM_WRITE_LATENCY_4;
      sHSPI_Init.IOMode           = BSP_HSPI_RAM_IO_X16_MODE;
    }
    else if (mode == 2)
    {
      rt_kprintf("\r\n ---- Fixed Latency 2*5, Hybrid 32-bytes burst, x16 IO ----");
      sHSPI_Init.LatencyType      = BSP_HSPI_RAM_FIXED_LATENCY;
      sHSPI_Init.BurstType        = BSP_HSPI_RAM_LINEAR_BURST;
      sHSPI_Init.BurstLength      = BSP_HSPI_RAM_BURST_32_BYTES;
      sHSPI_Init.ReadLatencyCode  = BSP_HSPI_RAM_READ_LATENCY_5;
      sHSPI_Init.WriteLatencyCode = BSP_HSPI_RAM_WRITE_LATENCY_5;
      sHSPI_Init.IOMode           = BSP_HSPI_RAM_IO_X16_MODE;
    }
    else if (mode == 3)
    {
      rt_kprintf("\r\n ---- Variable Latency 6, Hybrid 64-bytes burst, x16 IO ----");
      sHSPI_Init.LatencyType      = BSP_HSPI_RAM_VARIABLE_LATENCY;
      sHSPI_Init.BurstType        = BSP_HSPI_RAM_LINEAR_BURST;
      sHSPI_Init.BurstLength      = BSP_HSPI_RAM_BURST_64_BYTES;
      sHSPI_Init.ReadLatencyCode  = BSP_HSPI_RAM_READ_LATENCY_6;
      sHSPI_Init.WriteLatencyCode = BSP_HSPI_RAM_WRITE_LATENCY_6;
      sHSPI_Init.IOMode           = BSP_HSPI_RAM_IO_X16_MODE;
    }
    else if (mode == 4)
    {
      rt_kprintf("\r\n ---- Fixed Latency 2*7, Linear burst, x16 IO ----");
      sHSPI_Init.LatencyType      = BSP_HSPI_RAM_FIXED_LATENCY;
      sHSPI_Init.BurstType        = BSP_HSPI_RAM_LINEAR_BURST;
      sHSPI_Init.BurstLength      = BSP_HSPI_RAM_BURST_64_BYTES;
      sHSPI_Init.ReadLatencyCode  = BSP_HSPI_RAM_READ_LATENCY_7;
      sHSPI_Init.WriteLatencyCode = BSP_HSPI_RAM_WRITE_LATENCY_7;
      sHSPI_Init.IOMode           = BSP_HSPI_RAM_IO_X16_MODE;
    }
    rt_kprintf("\r\n");

    if (BSP_HSPI_RAM_Init(0, &sHSPI_Init) != BSP_ERROR_NONE)
    {
      rt_kprintf("\r\nHSPI RAM Initialization : Failed");
      rt_kprintf("\r\nHSPI RAM Test Aborted");
      rt_kprintf("\r\n");
      result--;
    }
    else
    {
      /*##-2- HSPI RAM memory read/write access  ###############################*/
      /* Fill the buffer to write */
      Fill_Buffer(hspi_aTxBuffer, HSPI_BUFFER_SIZE, 0xD20F);

      /* Write data to the HSPI RAM memory */
      if(BSP_HSPI_RAM_Write(0, hspi_aTxBuffer, HSPI_WRITE_READ_ADDR, HSPI_BUFFER_SIZE) != BSP_ERROR_NONE)
      {
        rt_kprintf("\r\nHSPI RAM Write : Failed");
        rt_kprintf("\r\nHSPI RAM Test Aborted");
        rt_kprintf("\r\n");
        result--;
      }
      else
      {
        /* Read back data from the HSPI RAM memory */
        if(BSP_HSPI_RAM_Read(0, hspi_aRxBuffer, HSPI_WRITE_READ_ADDR, HSPI_BUFFER_SIZE) != BSP_ERROR_NONE)
        {
          rt_kprintf("\r\nHSPI RAM Read : Failed");
          rt_kprintf("\r\nHSPI RAM Test Aborted");
          rt_kprintf("\r\n");
          result--;
        }
        else
        {
          /*##-3- Checking data integrity ######################################*/
          if(Buffercmp(hspi_aRxBuffer, hspi_aTxBuffer, HSPI_BUFFER_SIZE) > 0)
          {
            rt_kprintf("\r\nHSPI RAM Compare : Failed");
            rt_kprintf("\r\nHSPI RAM Test Aborted");
            rt_kprintf("\r\n");
            result--;
          }
          else
          {
            rt_kprintf("\r\nHSPI Test : OK");
            rt_kprintf("\r\n");
          }
        }
      }
    }

    /* De-initialization in order to have correct configuration memory on next try */
    BSP_HSPI_RAM_DeInit(0);
  }
  rt_kprintf("\r\n################## End HSPI test #####################");
  rt_kprintf("\r\n");

  return 0;
}
MSH_CMD_EXPORT_ALIAS(HSPI_RAM_demo, psramtest, Test Hspi psram)

/**
* @brief  HSPI RAM Demo
* @param  None
* @retval None
*/
int32_t HSPI_RAM_XIP(void)
{
  int32_t result = 0;
  BSP_HSPI_RAM_Cfg_t sHSPI_Init;
  uint8_t mode = 4;

  /* Demo Hint */
  // hspi_SetHint();

//  rt_kprintf("\r\n################## Start HSPI test #####################");
//  rt_kprintf("\r\n");

  /*##-1- Configure the HSPI RAM device ######################################*/
  /* HSPI RAM device configuration */
  if (mode == 0)
  {
    rt_kprintf("\r\n ---- Fixed Latency 2*3, Linear burst, x16 IO ----");
    sHSPI_Init.LatencyType = BSP_HSPI_RAM_FIXED_LATENCY;
    sHSPI_Init.BurstType = BSP_HSPI_RAM_LINEAR_BURST;
    sHSPI_Init.BurstLength = BSP_HSPI_RAM_BURST_16_BYTES;
    sHSPI_Init.ReadLatencyCode = BSP_HSPI_RAM_READ_LATENCY_3;
    sHSPI_Init.WriteLatencyCode = BSP_HSPI_RAM_WRITE_LATENCY_3;
    sHSPI_Init.IOMode = BSP_HSPI_RAM_IO_X16_MODE;
  }
  else if (mode == 1)
  {
    rt_kprintf("\r\n ---- Variable Latency 4, Hybrid 16-bytes burst, x8 IO ----");
    sHSPI_Init.LatencyType = BSP_HSPI_RAM_VARIABLE_LATENCY;
    sHSPI_Init.BurstType = BSP_HSPI_RAM_LINEAR_BURST;
    sHSPI_Init.BurstLength = BSP_HSPI_RAM_BURST_16_BYTES;
    sHSPI_Init.ReadLatencyCode = BSP_HSPI_RAM_READ_LATENCY_4;
    sHSPI_Init.WriteLatencyCode = BSP_HSPI_RAM_WRITE_LATENCY_4;
    sHSPI_Init.IOMode = BSP_HSPI_RAM_IO_X16_MODE;
  }
  else if (mode == 2)
  {
    rt_kprintf("\r\n ---- Fixed Latency 2*5, Hybrid 32-bytes burst, x16 IO ----");
    sHSPI_Init.LatencyType = BSP_HSPI_RAM_FIXED_LATENCY;
    sHSPI_Init.BurstType = BSP_HSPI_RAM_LINEAR_BURST;
    sHSPI_Init.BurstLength = BSP_HSPI_RAM_BURST_32_BYTES;
    sHSPI_Init.ReadLatencyCode = BSP_HSPI_RAM_READ_LATENCY_5;
    sHSPI_Init.WriteLatencyCode = BSP_HSPI_RAM_WRITE_LATENCY_5;
    sHSPI_Init.IOMode = BSP_HSPI_RAM_IO_X16_MODE;
  }
  else if (mode == 3)
  {
    rt_kprintf("\r\n ---- Variable Latency 6, Hybrid 64-bytes burst, x16 IO ----");
    sHSPI_Init.LatencyType = BSP_HSPI_RAM_VARIABLE_LATENCY;
    sHSPI_Init.BurstType = BSP_HSPI_RAM_LINEAR_BURST;
    sHSPI_Init.BurstLength = BSP_HSPI_RAM_BURST_64_BYTES;
    sHSPI_Init.ReadLatencyCode = BSP_HSPI_RAM_READ_LATENCY_6;
    sHSPI_Init.WriteLatencyCode = BSP_HSPI_RAM_WRITE_LATENCY_6;
    sHSPI_Init.IOMode = BSP_HSPI_RAM_IO_X16_MODE;
  }
  else if (mode == 4)
  {
    rt_kprintf("\r\n ---- Fixed Latency 2*7, Linear burst, x16 IO ----");
    sHSPI_Init.LatencyType = BSP_HSPI_RAM_FIXED_LATENCY;
    sHSPI_Init.BurstType = BSP_HSPI_RAM_LINEAR_BURST;
    sHSPI_Init.BurstLength = BSP_HSPI_RAM_BURST_64_BYTES;
    sHSPI_Init.ReadLatencyCode = BSP_HSPI_RAM_READ_LATENCY_7;
    sHSPI_Init.WriteLatencyCode = BSP_HSPI_RAM_WRITE_LATENCY_7;
    sHSPI_Init.IOMode = BSP_HSPI_RAM_IO_X16_MODE;
  }
  rt_kprintf("\r\n");

  if (BSP_HSPI_RAM_Init(0, &sHSPI_Init) != BSP_ERROR_NONE)
  {
    rt_kprintf("\r\nHSPI RAM Initialization : Failed");
    rt_kprintf("\r\n");
    result--;
  }
  else
  {
    rt_kprintf("\r\nHSPI RAM Initialization : OK");
    rt_kprintf("\r\n");
    BSP_HSPI_RAM_EnableMemoryMappedMode(0);
  }

  return 0;
}
MSH_CMD_EXPORT_ALIAS(HSPI_RAM_XIP, xipEN, hspi psram xip mapping)


/**
* @brief  Fills buffer with user predefined data.
* @param  pBuffer: pointer on the buffer to fill
* @param  uwBufferLenght: size of the buffer to fill
* @param  uwOffset: first value to fill on the buffer
* @retval None
*/
static void Fill_Buffer(uint8_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;

  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
  }
}

/**
* @brief  Compares two buffers.
* @param  pBuffer1, pBuffer2: buffers to be compared.
* @param  BufferLength: buffer's length
* @retval 1: pBuffer identical to pBuffer1
*         0: pBuffer differs from pBuffer1
*/
static uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/**
  * @brief  Rx Transfer completed callback.
  * @param  hhspi: HSPI handle
  * @retval None
  */
void HAL_XSPI_RxCpltCallback(XSPI_HandleTypeDef *xspi)
{
  ubHspiRamDmaCplt++;
}

/**
  * @brief  Tx Transfer completed callback.
  * @param  hhspi: HSPI handle
  * @retval None
  */
void HAL_XSPI_TxCpltCallback(XSPI_HandleTypeDef *xspi)
{
  ubHspiRamDmaCplt++;
}
/**
* @}
*/

/**
* @}
*/

