/*
 * Copyright (c) 2006-2024, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-01-24     yuanjie      first version
 */

#include "w25qxx.h"
#include <rtthread.h>
#include <drv_common.h>
#include "mx66uw1g45g.h"

#define DRV_DEBUG
#define LOG_TAG             "drv.mx66uw"
#include <drv_log.h>

// #define DBG_TAG "drv.mx66uw"
// #define DBG_LVL DBG_LOG
// #include <rtdbg.h>

XSPI_HandleTypeDef       hxspi2;

/**
  * @brief XSPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_XSPI2_Init(void)
{
  /* USER CODE BEGIN XSPI2_Init 0 */
  
  /* USER CODE END XSPI2_Init 0 */

  XSPIM_CfgTypeDef sXspiManagerCfg = {0};

  /* USER CODE BEGIN XSPI2_Init 1 */
  
  /* USER CODE END XSPI2_Init 1 */
  /* XSPI2 parameter configuration*/
  hxspi2.Instance = XSPI2;
  hxspi2.Init.FifoThresholdByte = 4;
  hxspi2.Init.MemoryMode = HAL_XSPI_SINGLE_MEM;
  hxspi2.Init.MemoryType = HAL_XSPI_MEMTYPE_MACRONIX;
  hxspi2.Init.MemorySize = HAL_XSPI_SIZE_32GB;
  hxspi2.Init.ChipSelectHighTimeCycle = 2U;
  hxspi2.Init.FreeRunningClock = HAL_XSPI_FREERUNCLK_DISABLE;
  hxspi2.Init.ClockMode = HAL_XSPI_CLOCK_MODE_0;
  hxspi2.Init.WrapSize = HAL_XSPI_WRAP_NOT_SUPPORTED;
  hxspi2.Init.ClockPrescaler = 0U;
  hxspi2.Init.SampleShifting = HAL_XSPI_SAMPLE_SHIFT_NONE;
  hxspi2.Init.DelayHoldQuarterCycle = HAL_XSPI_DHQC_ENABLE;
  hxspi2.Init.ChipSelectBoundary = HAL_XSPI_BONDARYOF_NONE;
  hxspi2.Init.MaxTran = 0U;
  hxspi2.Init.Refresh = 0U;
  hxspi2.Init.MemorySelect = HAL_XSPI_CSSEL_NCS1;
  if (HAL_XSPI_Init(&hxspi2) != HAL_OK)
  {
    Error_Handler();
  }
//  sXspiManagerCfg.nCSOverride = HAL_XSPI_CSSEL_OVR_DISABLED;
//  sXspiManagerCfg.IOPort = HAL_XSPIM_IOPORT_2;
//  if (HAL_XSPIM_Config(&hxspi2, &sXspiManagerCfg, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
//  {
//    Error_Handler();
//  }
  /* USER CODE BEGIN XSPI2_Init 2 */
  
  /* USER CODE END XSPI2_Init 2 */
}

/**
  * @brief  User Option Bytes Configuration
  *         The User Option Bytes are configured as follows :
  *            VDDIO_HSLV = 1 (enable the configuration of pads below 2.5V,
  *                            I/O speed otpmization at low-voltage allowed)
  *            XSPI1_HSLV = 1 (enable I/O XSPIM Port 1 high-speed option)
  *            XSPI2_HSLV = 1 (enable I/O XSPIM Port 2 high-speed option)
  *            Other User Option Bytes remain unchanged
  * @retval None
  */
static void OB_Config(void)
{
  FLASH_OBProgramInitTypeDef FLASH_OBProgramInitStruct = {0U};
  uint32_t current_user_ob_cfg;

  /* Get current configuration of option bytes */
  HAL_FLASHEx_OBGetConfig(&FLASH_OBProgramInitStruct);
  current_user_ob_cfg = FLASH_OBProgramInitStruct.USERConfig1;

  /* Initialized required fields for User Option Bytes configuration */
  FLASH_OBProgramInitStruct.OptionType = OPTIONBYTE_USER;
  FLASH_OBProgramInitStruct.USERType = 0U;
  FLASH_OBProgramInitStruct.USERConfig1 = 0U;

  if ((current_user_ob_cfg & OB_VDDIO_HSLV_ENABLE) == 0U)
  {
    /* VDDIO HSLV need to be updated */
    FLASH_OBProgramInitStruct.USERType |= OB_USER_VDDIO_HSLV;
    FLASH_OBProgramInitStruct.USERConfig1 |= OB_VDDIO_HSLV_ENABLE;
  }

  if ((current_user_ob_cfg & OB_XSPI1_HSLV_ENABLE) == 0U)
  {
    /* XSPI1 HSLV need to be updated */
    FLASH_OBProgramInitStruct.USERType |= OB_USER_XSPI1_HSLV;
    FLASH_OBProgramInitStruct.USERConfig1 |= OB_XSPI1_HSLV_ENABLE;
  }

  if ((current_user_ob_cfg & OB_XSPI2_HSLV_ENABLE) == 0U)
  {
    /* XSPI2 HSLV need to be updated */
    FLASH_OBProgramInitStruct.USERType |= OB_USER_XSPI2_HSLV;
    FLASH_OBProgramInitStruct.USERConfig1 |= OB_XSPI2_HSLV_ENABLE;
  }

  if (FLASH_OBProgramInitStruct.USERType != 0U)
  {
    /* At least one User Option Byte should be modified */
    if (HAL_FLASH_Unlock() != HAL_OK)
    {
      /* Flash unlock error */
      Error_Handler();
    }

    if (HAL_FLASH_OB_Unlock() != HAL_OK)
    {
      /* Option bytes unlock error */
      Error_Handler();
    }

    if (HAL_FLASHEx_OBProgram(&FLASH_OBProgramInitStruct) != HAL_OK)
    {
      /* Option Bytes modification error */
      Error_Handler();
    }

    if (HAL_FLASH_OB_Lock() != HAL_OK)
    {
      /* Option bytes lock error */
      Error_Handler();
    }

    if (HAL_FLASH_Lock() != HAL_OK)
    {
      /* Flash lock error */
      Error_Handler();
    }
  }
}


int32_t MX_XSPI_SetClock(XSPI_HandleTypeDef *Ctx, uint32_t ClockIn, uint32_t ClockRequested, uint32_t *ClockReal)
{
  int32_t retr = MX66UW1G45G_OK;
  uint32_t divider;

  if (ClockRequested == 0u)
  {
    retr = MX66UW1G45G_ERROR;
  }
  else
  {
    divider = (ClockIn / ClockRequested);
    if (divider >= 1u) 
    {
      *ClockReal = ClockIn / divider;
      if (*ClockReal <= ClockRequested) 
      {
        divider--;
      }
    }

#if 0
    divider++;
    divider++;
    divider++;
    divider++;
    divider++;
    divider++;
    divider++;
    divider++;
    divider++;
#endif

    /* real clock calculation */
    *ClockReal = ClockIn / (divider + 1u);

    MODIFY_REG(Ctx->Instance->DCR2, XSPI_DCR2_PRESCALER, (uint32_t)divider << XSPI_DCR2_PRESCALER_Pos);
  }

  return retr;
}

/**
 * @brief Send command to control register, with/without payload continued.
 * 
 * @param Ctx Component object pointer
 * @param Command command to send
 * @param Data payload after command
 * @param DataSize payload size
 * @return int32_t error status MX66UW1G45G_OK or MX66UW1G45G_ERROR
 */
int32_t MX66UW1G45G_SendCommand(XSPI_HandleTypeDef *Ctx, uint8_t Command,
                                    uint8_t *Data, uint16_t DataSize)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* Initialize the writing of configuration register */
  s_command.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
  s_command.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
  s_command.Instruction = Command;
  s_command.AddressMode = HAL_XSPI_ADDRESS_NONE;
//   s_command.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_DISABLE;
//   s_command.AddressWidth = HAL_XSPI_ADDRESS_32_BITS;
//   s_command.Address = WriteAddr;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = HAL_XSPI_DATA_1_LINE;
  s_command.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles = 0U;
  s_command.DataLength = DataSize;
  s_command.DQSMode = HAL_XSPI_DQS_DISABLE;
  if (DataSize == 0u)
  {
    s_command.DataMode         = HAL_XSPI_DATA_NONE;
  }
  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }
  if (HAL_XSPI_Transmit(Ctx, Data, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }
  return MX66UW1G45G_OK;
}

/**
 * @brief Send Command to set Octal Flash into OSPI mode
 * 
 * @param Ctx Component object pointer
 * @param Mode Interface select
 * @return int32_t error status MX66UW1G45G_OK or MX66UW1G45G_ERROR
 */
int32_t MX66UW_SPI_Enter_OCTAL_DDR_Mode(XSPI_HandleTypeDef *Ctx, MX66UW1G45G_Interface_t Mode)
{
    uint8_t data[6] = {0};
    /* write enable */
    if (MX66UW1G45G_OK != MX66UW1G45G_WriteEnable(Ctx, Mode, MX66UW1G45G_STR_TRANSFER))
    {
        return MX66UW1G45G_ERROR;
    }
    /* write status register */
    data[0] = 0x00;
    data[1] = 0x20;
    data[2] = 0x00;
    if (MX66UW1G45G_OK != MX66UW1G45G_SendCommand(Ctx, MX66UW1G45G_WRITE_STATUS_REG_CMD, &data[0], 3-1))
    {
        return MX66UW1G45G_ERROR;
    }
    /* write enable */
    if (MX66UW1G45G_OK != MX66UW1G45G_WriteEnable(Ctx, Mode, MX66UW1G45G_STR_TRANSFER))
    {
        return MX66UW1G45G_ERROR;
    }
    /* write config register 2 to octal ddr mode*/
    data[0] = 0x00;
    data[1] = 0x00;
    data[2] = 0x00;
    data[3] = 0x00;
    data[4] = 0x02;
    data[5] = 0x00;
    if (MX66UW1G45G_OK != MX66UW1G45G_SendCommand(Ctx, MX66UW1G45G_WRITE_CFG_REG2_CMD, &data[0], 6-1))
    {
        return MX66UW1G45G_ERROR;
    }
    return MX66UW1G45G_OK;
}

// static MX66UW1G45G_Info_t flash_info = {0};
void MX66UW_Init(void)
{
    rt_uint8_t flash_id[3] = {0};
    uint32_t ClockOut;
    /* Configure the user option bytes to reach 200 MHz on XSPI interface */
    OB_Config();
    MX_XSPI2_Init();
    /* Set clock protection on XSPI memory interface */
    HAL_RCCEx_EnableClockProtection(RCC_CLOCKPROTECT_XSPI);
    MX_XSPI_SetClock(&hxspi2, HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_XSPI2), 50000000u, &ClockOut);
    if (MX66UW1G45G_OK != MX66UW1G45G_ResetEnable(&hxspi2, MX66UW1G45G_SPI_MODE, MX66UW1G45G_STR_TRANSFER))
    {
        LOG_D("ResetEnable failed.");
    }
    if (MX66UW1G45G_OK != MX66UW1G45G_ResetMemory(&hxspi2, MX66UW1G45G_SPI_MODE, MX66UW1G45G_STR_TRANSFER))
    {
        LOG_D("ResetMemory failed.");
    }
    
    // if (MX66UW1G45G_OK != MX66UW1G45G_GetFlashInfo(&flash_info))
    // {
    //     LOG_D("ResetMemory failed.");
    // }
    if (MX66UW1G45G_OK != MX66UW1G45G_ReadID(&hxspi2, MX66UW1G45G_SPI_MODE, MX66UW1G45G_STR_TRANSFER, flash_id))
    {
        LOG_D("ReadID failed.");
    }
    LOG_I("flash id = %.2x%.2x%.2x", flash_id[0], flash_id[1], flash_id[2]);

    if (MX66UW1G45G_OK != MX66UW_SPI_Enter_OCTAL_DDR_Mode(&hxspi2, MX66UW1G45G_SPI_MODE))
    {
        LOG_D("MX66UW_SPI_Enter_OCTAL_DDR_Mode failed.");
    }

    MX_XSPI_SetClock(&hxspi2, HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_XSPI2), 199000000u, &ClockOut);
    // rt_thread_mdelay(100);
    // if (MX66UW1G45G_OK != MX66UW1G45G_EnableMemoryMappedModeSTR(&hxspi2, MX66UW1G45G_SPI_MODE, MX66UW1G45G_4BYTES_SIZE))
    // {
    //     LOG_D("EnableMemoryMappedModeSTR failed.");
    // }
//    if (MX66UW1G45G_OK != MX66UW1G45G_EnableMemoryMappedModeDTR(&hxspi2, MX66UW1G45G_OPI_MODE))
//    {
//        LOG_D("EnableMemoryMappedModeDTR failed.");
//    }
}

// void MX66UW_ExitOctalMode(void)
// {
//
// }

/**
  * @brief this function return a formatted command
  *
  * @param CommandExtension type of the command extension 0: the complement  1 : the same
  * @param InstructionWidth instruction width
  * @param Command command
  * @return the formatted command
  */
uint16_t XSPI_FormatCommand(uint8_t CommandExtension, uint32_t InstructionWidth, uint8_t Command)
{
  uint16_t retr;
  if  (InstructionWidth == HAL_XSPI_INSTRUCTION_16_BITS)
  {
    retr = ((uint16_t)Command << 8u);
    if (CommandExtension == 0u)
    {
       retr|=  (uint8_t)(~Command & 0xFFu);
    }
    else
    {
       retr|=  (uint8_t)(Command & 0xFFu);
    }
  }
  else
  {
    retr = Command;
  }

  return retr;
}

int32_t MX66UW_Memory_Mapped_Enable(void)
{
    HAL_StatusTypeDef retr;
    XSPI_RegularCmdTypeDef s_command = {0};
    XSPI_MemoryMappedTypeDef sMemMappedCfg = {0};

    /* Initialize the s_command */
    s_command.InstructionMode = 0x04;
    s_command.InstructionWidth = 0x10;
    s_command.InstructionDTRMode = 0x08;

    s_command.AddressMode = 0x400;
    s_command.AddressWidth = 0x3000;
    s_command.AddressDTRMode = 0x800;

    s_command.DataMode = 0x04000000;
    s_command.DataDTRMode = 0x08000000;
    s_command.DQSMode = 0x20000000;

    /* Initialize the read ID command */
    s_command.OperationType = HAL_XSPI_OPTYPE_READ_CFG;
    s_command.Instruction = MX66UW1G45G_OCTA_READ_DTR_CMD;
    // s_command.Instruction = XSPI_FormatCommand(hXspi->commandExtension, s_command.InstructionWidth, CommandRead);
    s_command.DummyCycles = 0x14;
    /* Configure the read command */
    retr = HAL_XSPI_Command(&hxspi2, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE);
    if (retr != HAL_OK)
    {
        goto error;
    }

    /* Initialize the read ID command */
    s_command.OperationType = HAL_XSPI_OPTYPE_WRITE_CFG;
    s_command.Instruction = MX66UW1G45G_OCTA_PAGE_PROG_CMD;
    // s_command.Instruction = XSPI_FormatCommand(hXspi->commandExtension, s_command.InstructionWidth, CommandWrite);
    s_command.DummyCycles = 0x00;
    /* Configure the read command */
    retr = HAL_XSPI_Command(&hxspi2, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE);
    if (retr != HAL_OK)
    {
        goto error;
    }

    /* Activation of memory-mapped mode */
    sMemMappedCfg.TimeOutActivation = HAL_XSPI_TIMEOUT_COUNTER_DISABLE;
    sMemMappedCfg.TimeoutPeriodClock = 0x50;
    retr = HAL_XSPI_MemoryMapped(&hxspi2, &sMemMappedCfg);

error:
    if (retr != HAL_OK)
    {
        /* abort any ongoing transaction for the next action */
        (void)HAL_XSPI_Abort(&hxspi2);
    }
    /* return status */
    return retr;
}
