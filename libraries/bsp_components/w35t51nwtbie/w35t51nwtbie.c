/**
  ******************************************************************************
  * @file    w35t51nwtbie.c
  * @modify  RT-Thread Community Team
  * @brief   This file provides the W35T51NWTBIE OctoSPI drivers.
  ******************************************************************************
  ******************************************************************************
  * W35T51NWTBIE action :
  *   STR Octal IO protocol (SOPI) and DTR Octal IO protocol (DOPI) bits of
  *   Configuration Register 2 :
  *     DOPI = 1 and SOPI = 0: Operates in DTR Octal IO protocol (accepts 8-8-8 commands)
  *     DOPI = 0 and SOPI = 1: Operates in STR Octal IO protocol (accepts 8-8-8 commands)
  *     DOPI = 0 and SOPI = 0: Operates in Single IO protocol (accepts 1-1-1 commands)
  *   Enter SOPI mode by configuring DOPI = 0 and SOPI = 1 in CR2-Addr0
  *   Exit SOPI mode by configuring DOPI = 0 and SOPI = 0 in CR2-Addr0
  *   Enter DOPI mode by configuring DOPI = 1 and SOPI = 0 in CR2-Addr0
  *   Exit DOPI mode by configuring DOPI = 0 and SOPI = 0 in CR2-Addr0
  *
  *   Memory commands support STR(Single Transfer Rate) &
  *   DTR(Double Transfer Rate) modes in OPI
  *
  *   Memory commands support STR(Single Transfer Rate) &
  *   DTR(Double Transfer Rate) modes in SPI
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "w35t51nwtbie.h"


/**
  * @brief  Get Flash information
  * @param  pInfo pointer to information structure
  * @retval error status
  */
int32_t W35T51NWTBIE_GetFlashInfo(W35T51NWTBIE_Info_t *pInfo)
{
  /* Configure the structure with the memory configuration */
  pInfo->FlashSize              = W35T51NWTBIE_FLASH_SIZE;
  pInfo->EraseSectorSize        = W35T51NWTBIE_SECTOR_64K;
  pInfo->EraseSectorsNumber     = (W35T51NWTBIE_FLASH_SIZE / W35T51NWTBIE_SECTOR_64K);
  pInfo->EraseSubSectorSize     = W35T51NWTBIE_SUBSECTOR_4K;
  pInfo->EraseSubSectorNumber   = (W35T51NWTBIE_FLASH_SIZE / W35T51NWTBIE_SUBSECTOR_4K);
  pInfo->EraseSubSector1Size    = W35T51NWTBIE_SUBSECTOR_4K;
  pInfo->EraseSubSector1Number  = (W35T51NWTBIE_FLASH_SIZE / W35T51NWTBIE_SUBSECTOR_4K);
  pInfo->ProgPageSize           = W35T51NWTBIE_PAGE_SIZE;
  pInfo->ProgPagesNumber        = (W35T51NWTBIE_FLASH_SIZE / W35T51NWTBIE_PAGE_SIZE);

  return W35T51NWTBIE_OK;
};

// #define MEMORY_READY_MATCH_VALUE    0x00
// #define MEMORY_READY_MASK_VALUE     0x01

/**
  * @brief  Polling WIP(Write In Progress) bit become to 0
  *         SPI/OPI;
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  Rate Transfer rate
  * @retval error status
  */
int32_t W35T51NWTBIE_AutoPollingMemReady(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode,
                                         W35T51NWTBIE_Transfer_t Rate)
{
  XSPI_RegularCmdTypeDef  s_command = {0};
  XSPI_AutoPollingTypeDef s_config = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == W35T51NWTBIE_SPI_MODE) && (Rate == W35T51NWTBIE_DTR_TRANSFER))
  {
    return W35T51NWTBIE_ERROR;
  }

  /* Configure automatic polling mode to wait for memory ready */
  s_command.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect           = HAL_XSPI_SELECT_IO_7_0;
  s_command.InstructionMode    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_1_LINE
                                 : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == W35T51NWTBIE_DTR_TRANSFER)
                                 ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                 : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_8_BITS
                                 : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction        = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? W35T51NWTBIE_READ_STATUS_REG_CMD
                                 : W35T51NWTBIE_OCTA_READ_STATUS_REG_CMD;
  s_command.AddressMode        = (Mode == W35T51NWTBIE_SPI_MODE) ? HAL_XSPI_ADDRESS_NONE : HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode     = (Rate == W35T51NWTBIE_DTR_TRANSFER)
                                 ? HAL_XSPI_ADDRESS_DTR_ENABLE
                                 : HAL_XSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressWidth        = HAL_XSPI_ADDRESS_24_BITS; //TODO 32bits
  s_command.Address            = 0U;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode           = (Mode == W35T51NWTBIE_SPI_MODE) ? HAL_XSPI_DATA_1_LINE : HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode        = (Rate == W35T51NWTBIE_DTR_TRANSFER)
                                 ? HAL_XSPI_DATA_DTR_ENABLE
                                 : HAL_XSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles        = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? 0U
                                 : ((Rate == W35T51NWTBIE_DTR_TRANSFER)
                                    ? DUMMY_CYCLES_REG_OCTAL_DTR
                                    : DUMMY_CYCLES_REG_OCTAL);
  s_command.DataLength             = (Rate == W35T51NWTBIE_DTR_TRANSFER) ? 2U : 1U;
  s_command.DQSMode            = (Rate == W35T51NWTBIE_DTR_TRANSFER) ? HAL_XSPI_DQS_ENABLE : HAL_XSPI_DQS_DISABLE;
#if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
#endif

  s_config.MatchValue         = 0U;
  s_config.MatchMask          = W35T51NWTBIE_SR_WIP;
  s_config.MatchMode          = HAL_XSPI_MATCH_MODE_AND;
  s_config.IntervalTime      = W35T51NWTBIE_AUTOPOLLING_INTERVAL_TIME;
  s_config.AutomaticStop = HAL_XSPI_AUTOMATIC_STOP_ENABLE;

  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  if (HAL_XSPI_AutoPolling(Ctx, &s_config, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  return W35T51NWTBIE_OK;
}

/* Read/Write Array Commands (3/4 Byte Address Command Set) *********************/
/**
  * @brief  Reads an amount of data from the OSPI memory on STR mode.
  *         SPI/OPI; 1-1-1/8-8-8
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  AddressSize Address size
  * @param  pData Pointer to data to be read
  * @param  ReadAddr Read start address
  * @param  Size Size of data to read
  * @retval OSPI memory status
  */
int32_t W35T51NWTBIE_ReadSTR(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode,
                             W35T51NWTBIE_AddressSize_t AddressSize, uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* OPI mode and 3-bytes address size not supported by memory */
  if ((Mode == W35T51NWTBIE_OPI_MODE) && (AddressSize == W35T51NWTBIE_3BYTES_SIZE))
  {
    return W35T51NWTBIE_ERROR;
  }

  /* Initialize the read command */
  s_command.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect           = HAL_XSPI_SELECT_IO_7_0;
  s_command.InstructionMode    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_1_LINE
                                 : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_8_BITS
                                 : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction        = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? ((AddressSize == W35T51NWTBIE_3BYTES_SIZE)
                                    ? W35T51NWTBIE_FAST_READ_CMD
                                    : W35T51NWTBIE_4_BYTE_ADDR_FAST_READ_CMD)
                                 : W35T51NWTBIE_OCTA_READ_CMD;
  s_command.AddressMode        = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_ADDRESS_1_LINE
                                 : HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode     = HAL_XSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressWidth        = (AddressSize == W35T51NWTBIE_3BYTES_SIZE)
                                 ? HAL_XSPI_ADDRESS_24_BITS
                                 : HAL_XSPI_ADDRESS_32_BITS;
  s_command.Address            = ReadAddr;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode           = (Mode == W35T51NWTBIE_SPI_MODE) ? HAL_XSPI_DATA_1_LINE : HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode        = HAL_XSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles        = (Mode == W35T51NWTBIE_SPI_MODE) ? DUMMY_CYCLES_READ : DUMMY_CYCLES_READ_OCTAL;
  s_command.DataLength             = Size;
  s_command.DQSMode            = HAL_XSPI_DQS_DISABLE;
#if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
#endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  /* Reception of the data */
  if (HAL_XSPI_Receive(Ctx, pData, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  return W35T51NWTBIE_OK;
}

/**
  * @brief  Reads an amount of data from the OSPI memory on DTR mode.
  *         OPI
  * @param  Ctx Component object pointer
  * @param  AddressSize Address size
  * @param  pData Pointer to data to be read
  * @param  ReadAddr Read start address
  * @param  Size Size of data to read
  * @note   Only OPI mode support DTR transfer rate
  * @retval OSPI memory status
  */
int32_t W35T51NWTBIE_ReadDTR(XSPI_HandleTypeDef *Ctx, uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* Initialize the read command */
  s_command.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect           = HAL_XSPI_SELECT_IO_7_0;
  s_command.InstructionMode    = HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_ENABLE;
  s_command.InstructionWidth    = HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction        = W35T51NWTBIE_OCTA_READ_DTR_CMD;
  s_command.AddressMode        = HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode     = HAL_XSPI_ADDRESS_DTR_ENABLE;
  s_command.AddressWidth        = HAL_XSPI_ADDRESS_32_BITS;
  s_command.Address            = ReadAddr;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode           = HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode        = HAL_XSPI_DATA_DTR_ENABLE;
  s_command.DummyCycles        = DUMMY_CYCLES_READ_OCTAL_DTR;
  s_command.DataLength             = Size;
  s_command.DQSMode            = HAL_XSPI_DQS_ENABLE;
#if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
#endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  /* Reception of the data */
  if (HAL_XSPI_Receive(Ctx, pData, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  return W35T51NWTBIE_OK;
}

/**
  * @brief  Writes an amount of data to the OSPI memory.
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  AddressSize Address size
  * @param  pData Pointer to data to be written
  * @param  WriteAddr Write start address
  * @param  Size Size of data to write. Range 1 ~ W35T51NWTBIE_PAGE_SIZE
  * @note   Address size is forced to 3 Bytes when the 4 Bytes address size
  *         command is not available for the specified interface mode
  * @retval OSPI memory status
  */
int32_t W35T51NWTBIE_PageProgram(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode,
                                 W35T51NWTBIE_AddressSize_t AddressSize, uint8_t *pData, uint32_t WriteAddr,
                                 uint32_t Size)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* OPI mode and 3-bytes address size not supported by memory */
  if ((Mode == W35T51NWTBIE_OPI_MODE) && (AddressSize == W35T51NWTBIE_3BYTES_SIZE))
  {
    return W35T51NWTBIE_ERROR;
  }

  /* Initialize the program command */
  s_command.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect           = HAL_XSPI_SELECT_IO_7_0;
  s_command.InstructionMode    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_1_LINE
                                 : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_8_BITS
                                 : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction        = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? ((AddressSize == W35T51NWTBIE_3BYTES_SIZE)
                                    ? W35T51NWTBIE_PAGE_PROG_CMD
                                    : W35T51NWTBIE_4_BYTE_PAGE_PROG_CMD)
                                 : W35T51NWTBIE_OCTA_PAGE_PROG_CMD;
  s_command.AddressMode        = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_ADDRESS_1_LINE
                                 : HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode     = HAL_XSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressWidth        = (AddressSize == W35T51NWTBIE_3BYTES_SIZE)
                                 ? HAL_XSPI_ADDRESS_24_BITS
                                 : HAL_XSPI_ADDRESS_32_BITS;
  s_command.Address            = WriteAddr;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode           = (Mode == W35T51NWTBIE_SPI_MODE) ? HAL_XSPI_DATA_1_LINE : HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode        = HAL_XSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles        = 0U;
  s_command.DataLength             = Size;
  s_command.DQSMode            = HAL_XSPI_DQS_DISABLE;
#if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
#endif

  /* Configure the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  /* Transmission of the data */
  if (HAL_XSPI_Transmit(Ctx, pData, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  return W35T51NWTBIE_OK;
}

/**
  * @brief  Writes an amount of data to the OSPI memory on DTR mode.
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  pData Pointer to data to be written
  * @param  WriteAddr Write start address
  * @param  Size Size of data to write. Range 1 ~ W35T51NWTBIE_PAGE_SIZE
  * @note   Only OPI mode support DTR transfer rate
  * @retval OSPI memory status
  */
int32_t W35T51NWTBIE_PageProgramDTR(XSPI_HandleTypeDef *Ctx, uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* Initialize the program command */
  s_command.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect           = HAL_XSPI_SELECT_IO_7_0;
  s_command.InstructionMode    = HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_ENABLE;
  s_command.InstructionWidth    = HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction        = W35T51NWTBIE_OCTA_PAGE_PROG_CMD;
  s_command.AddressMode        = HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode     = HAL_XSPI_ADDRESS_DTR_ENABLE;
  s_command.AddressWidth        = HAL_XSPI_ADDRESS_32_BITS;
  s_command.Address            = WriteAddr;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode           = HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode        = HAL_XSPI_DATA_DTR_ENABLE;
  s_command.DummyCycles        = 0U;
  s_command.DataLength             = Size;
  s_command.DQSMode            = HAL_XSPI_DQS_DISABLE;
#if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
#endif

  /* Configure the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  /* Transmission of the data */
  if (HAL_XSPI_Transmit(Ctx, pData, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  return W35T51NWTBIE_OK;
}

/**
  * @brief  Erases the specified block of the OSPI memory.
  *         W35T51NWTBIE support 4K, 64K size block erase commands.
  *         SPI/OPI; 1-1-1/8-8-8
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  AddressSize Address size
  * @param  BlockAddress Block address to erase
  * @param  BlockSize Block size to erase
  * @retval OSPI memory status
  */
int32_t W35T51NWTBIE_BlockErase(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode, W35T51NWTBIE_Transfer_t Rate,
                                W35T51NWTBIE_AddressSize_t AddressSize, uint32_t BlockAddress,
                                W35T51NWTBIE_Erase_t BlockSize)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == W35T51NWTBIE_SPI_MODE) && (Rate == W35T51NWTBIE_DTR_TRANSFER))
  {
    return W35T51NWTBIE_ERROR;
  }

  /* OPI mode and 3-bytes address size not supported by memory */
  if ((Mode == W35T51NWTBIE_OPI_MODE) && (AddressSize == W35T51NWTBIE_3BYTES_SIZE))
  {
    return W35T51NWTBIE_ERROR;
  }

  /* Initialize the erase command */
  s_command.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect           = HAL_XSPI_SELECT_IO_7_0;
  s_command.InstructionMode    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_1_LINE
                                 : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == W35T51NWTBIE_DTR_TRANSFER)
                                 ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                 : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_8_BITS
                                 : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.AddressMode        = (Mode == W35T51NWTBIE_SPI_MODE) ? HAL_XSPI_ADDRESS_1_LINE : HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode     = (Rate == W35T51NWTBIE_DTR_TRANSFER)
                                 ? HAL_XSPI_ADDRESS_DTR_ENABLE
                                 : HAL_XSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressWidth        = (AddressSize == W35T51NWTBIE_3BYTES_SIZE)
                                 ? HAL_XSPI_ADDRESS_24_BITS
                                 : HAL_XSPI_ADDRESS_32_BITS;
  s_command.Address            = BlockAddress;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode           = HAL_XSPI_DATA_NONE;
  s_command.DummyCycles        = 0U;
  s_command.DQSMode            = HAL_XSPI_DQS_DISABLE;
#if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
#endif

  switch (Mode)
  {
    case W35T51NWTBIE_OPI_MODE :
      if (BlockSize == W35T51NWTBIE_ERASE_64K)
      {
        s_command.Instruction = W35T51NWTBIE_OCTA_SECTOR_ERASE_64K_CMD;
      }
      else
      {
        s_command.Instruction = W35T51NWTBIE_OCTA_SUBSECTOR_ERASE_4K_CMD;
      }
      break;

    case W35T51NWTBIE_SPI_MODE :
    default:
      if (BlockSize == W35T51NWTBIE_ERASE_64K)
      {
        s_command.Instruction = (AddressSize == W35T51NWTBIE_3BYTES_SIZE)
                                ? W35T51NWTBIE_SECTOR_ERASE_64K_CMD
                                : W35T51NWTBIE_4_BYTE_SECTOR_ERASE_64K_CMD;
      }
      else if(BlockSize == W35T51NWTBIE_ERASE_32K)
      {
        s_command.Instruction = (AddressSize == W35T51NWTBIE_3BYTES_SIZE)
                                ? W35T51NWTBIE_SECTOR_ERASE_32K_CMD
                                : W35T51NWTBIE_4_BYTE_SECTOR_ERASE_32K_CMD;
      }
      else
      {
        s_command.Instruction = (AddressSize == W35T51NWTBIE_3BYTES_SIZE)
                                ? W35T51NWTBIE_SUBSECTOR_ERASE_4K_CMD
                                : W35T51NWTBIE_4_BYTE_SUBSECTOR_ERASE_4K_CMD;
      }
      break;
  }

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  return W35T51NWTBIE_OK;
}

/**
  * @brief  Whole chip erase.
  *         SPI/OPI; 1-0-0/8-0-0
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @retval error status
  */
int32_t W35T51NWTBIE_ChipErase(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode, W35T51NWTBIE_Transfer_t Rate)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  // if ((Mode == W35T51NWTBIE_SPI_MODE) && (Rate == W35T51NWTBIE_DTR_TRANSFER))
  // {
  //   return W35T51NWTBIE_ERROR;
  // }

  /* Initialize the erase command */
  s_command.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect           = HAL_XSPI_SELECT_IO_7_0;
  s_command.InstructionMode    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_1_LINE
                                 : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == W35T51NWTBIE_DTR_TRANSFER)
                                 ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                 : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_8_BITS
                                 : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction        = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? W35T51NWTBIE_BULK_ERASE_CMD
                                 : W35T51NWTBIE_OCTA_BULK_ERASE_CMD;
  s_command.AddressMode        = HAL_XSPI_ADDRESS_NONE;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode           = HAL_XSPI_DATA_NONE;
  s_command.DummyCycles        = 0U;
  s_command.DQSMode            = HAL_XSPI_DQS_DISABLE;
#if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
#endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  return W35T51NWTBIE_OK;
}

/**
  * @brief  Enable memory mapped mode for the OSPI memory on STR mode.
  *         SPI/OPI; 1-1-1/8-8-8
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  AddressSize Address size
  * @retval OSPI memory status
  */
int32_t W35T51NWTBIE_EnableSTRMemoryMappedMode(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode,
                                               W35T51NWTBIE_AddressSize_t AddressSize)
{
  XSPI_RegularCmdTypeDef      s_command = {0};
  XSPI_MemoryMappedTypeDef s_mem_mapped_cfg = {0};

  /* OPI mode and 3-bytes address size not supported by memory */
  if ((Mode == W35T51NWTBIE_OPI_MODE) && (AddressSize == W35T51NWTBIE_3BYTES_SIZE))
  {
    return W35T51NWTBIE_ERROR;
  }

  /* Initialize the read command */
  s_command.OperationType      = HAL_XSPI_OPTYPE_READ_CFG;
  s_command.IOSelect           = HAL_XSPI_SELECT_IO_7_0;
  s_command.InstructionMode    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_1_LINE
                                 : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_8_BITS
                                 : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction        = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? ((AddressSize == W35T51NWTBIE_3BYTES_SIZE)
                                    ? W35T51NWTBIE_FAST_READ_CMD
                                    : W35T51NWTBIE_4_BYTE_ADDR_FAST_READ_CMD)
                                 : W35T51NWTBIE_OCTA_READ_CMD;
  s_command.AddressMode        = (Mode == W35T51NWTBIE_SPI_MODE) ? HAL_XSPI_ADDRESS_1_LINE : HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode     = HAL_XSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressWidth        = (AddressSize == W35T51NWTBIE_3BYTES_SIZE)
                                 ? HAL_XSPI_ADDRESS_24_BITS
                                 : HAL_XSPI_ADDRESS_32_BITS;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode           = (Mode == W35T51NWTBIE_SPI_MODE) ? HAL_XSPI_DATA_1_LINE : HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode        = HAL_XSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles        = (Mode == W35T51NWTBIE_SPI_MODE) ? DUMMY_CYCLES_READ : DUMMY_CYCLES_READ_OCTAL;
  s_command.DQSMode            = HAL_XSPI_DQS_DISABLE;
#if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
#endif

  /* Send the read command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  /* Initialize the program command */
  s_command.OperationType      = HAL_XSPI_OPTYPE_WRITE_CFG;
  s_command.Instruction        = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? ((AddressSize == W35T51NWTBIE_3BYTES_SIZE)
                                    ? W35T51NWTBIE_PAGE_PROG_CMD
                                    : W35T51NWTBIE_4_BYTE_PAGE_PROG_CMD)
                                 : W35T51NWTBIE_OCTA_PAGE_PROG_CMD;
  s_command.DummyCycles        = 0U;

  /* Send the write command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  /* Configure the memory mapped mode */
  s_mem_mapped_cfg.TimeOutActivation = HAL_XSPI_TIMEOUT_COUNTER_DISABLE;

  if (HAL_XSPI_MemoryMapped(Ctx, &s_mem_mapped_cfg) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  return W35T51NWTBIE_OK;
}

/**
  * @brief  Enable memory mapped mode for the OSPI memory on DTR mode.
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  AddressSize Address size
  * @note   Only OPI mode support DTR transfer rate
  * @retval OSPI memory status
  */
int32_t W35T51NWTBIE_EnableDTRMemoryMappedMode(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Mode);

  XSPI_RegularCmdTypeDef      s_command = {0};
  XSPI_MemoryMappedTypeDef s_mem_mapped_cfg = {0};

  /* Initialize the read command */
  s_command.OperationType      = HAL_XSPI_OPTYPE_READ_CFG;
  s_command.IOSelect           = HAL_XSPI_SELECT_IO_7_0;
  s_command.InstructionMode    = HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_ENABLE;
  s_command.InstructionWidth    = HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction        = W35T51NWTBIE_OCTA_READ_DTR_CMD;
  s_command.AddressMode        = HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode     = HAL_XSPI_ADDRESS_DTR_ENABLE;
  s_command.AddressWidth        = HAL_XSPI_ADDRESS_32_BITS;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode           = HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode        = HAL_XSPI_DATA_DTR_ENABLE;
  s_command.DummyCycles        = DUMMY_CYCLES_READ_OCTAL_DTR;
  s_command.DQSMode            = HAL_XSPI_DQS_ENABLE;
#if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
#endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  /* Initialize the program command */
  s_command.OperationType = HAL_XSPI_OPTYPE_WRITE_CFG;
  s_command.Instruction   = W35T51NWTBIE_OCTA_PAGE_PROG_CMD;
  s_command.DummyCycles   = 0U;
  s_command.DQSMode       = HAL_XSPI_DQS_DISABLE;

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }
  /* Configure the memory mapped mode */
  s_mem_mapped_cfg.TimeOutActivation = HAL_XSPI_TIMEOUT_COUNTER_DISABLE;

  if (HAL_XSPI_MemoryMapped(Ctx, &s_mem_mapped_cfg) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  return W35T51NWTBIE_OK;
}

/**
  * @brief  Flash suspend program or erase command
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface select
  * @param  Rate Transfer rate STR or DTR
  * @retval error status
  */
int32_t W35T51NWTBIE_Suspend(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode, W35T51NWTBIE_Transfer_t Rate)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == W35T51NWTBIE_SPI_MODE) && (Rate == W35T51NWTBIE_DTR_TRANSFER))
  {
    return W35T51NWTBIE_ERROR;
  }

  /* Initialize the suspend command */
  s_command.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect           = HAL_XSPI_SELECT_IO_7_0;
  s_command.InstructionMode    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_1_LINE
                                 : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == W35T51NWTBIE_DTR_TRANSFER)
                                 ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                 : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_8_BITS
                                 : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction        = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? W35T51NWTBIE_PROG_ERASE_SUSPEND_CMD
                                 : W35T51NWTBIE_OCTA_PROG_ERASE_SUSPEND_CMD;
  s_command.AddressMode        = HAL_XSPI_ADDRESS_NONE;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode           = HAL_XSPI_DATA_NONE;
  s_command.DummyCycles        = 0U;
  s_command.DQSMode            = HAL_XSPI_DQS_DISABLE;
#if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
#endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  return W35T51NWTBIE_OK;
}

/**
  * @brief  Flash resume program or erase command
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface select
  * @param  Rate Transfer rate STR or DTR
  * @retval error status
  */
int32_t W35T51NWTBIE_Resume(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode, W35T51NWTBIE_Transfer_t Rate)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == W35T51NWTBIE_SPI_MODE) && (Rate == W35T51NWTBIE_DTR_TRANSFER))
  {
    return W35T51NWTBIE_ERROR;
  }

  /* Initialize the resume command */
  s_command.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect           = HAL_XSPI_SELECT_IO_7_0;
  s_command.InstructionMode    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_1_LINE
                                 : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == W35T51NWTBIE_DTR_TRANSFER)
                                 ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                 : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_8_BITS
                                 : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction        = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? W35T51NWTBIE_PROG_ERASE_RESUME_CMD
                                 : W35T51NWTBIE_OCTA_PROG_ERASE_RESUME_CMD;
  s_command.AddressMode        = HAL_XSPI_ADDRESS_NONE;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode           = HAL_XSPI_DATA_NONE;
  s_command.DummyCycles        = 0U;
  s_command.DQSMode            = HAL_XSPI_DQS_DISABLE;
#if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
#endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  return W35T51NWTBIE_OK;
}

/* Register/Setting Commands **************************************************/
/**
  * @brief  This function send a Write Enable and wait it is effective.
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  Rate Transfer rate STR or DTR
  * @retval error status
  */
int32_t W35T51NWTBIE_WriteEnable(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode, W35T51NWTBIE_Transfer_t Rate)
{
  XSPI_RegularCmdTypeDef     s_command = {0};
  XSPI_AutoPollingTypeDef s_config = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == W35T51NWTBIE_SPI_MODE) && (Rate == W35T51NWTBIE_DTR_TRANSFER))
  {
    //return W35T51NWTBIE_ERROR;
  }

  /* Initialize the write enable command */
  s_command.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect           = HAL_XSPI_SELECT_IO_7_0;
  s_command.InstructionMode    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_1_LINE
                                 : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == W35T51NWTBIE_DTR_TRANSFER)
                                 ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                 : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_8_BITS
                                 : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction        = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? W35T51NWTBIE_WRITE_ENABLE_CMD
                                 : W35T51NWTBIE_OCTA_WRITE_ENABLE_CMD;
  s_command.AddressMode        = HAL_XSPI_ADDRESS_NONE;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode           = HAL_XSPI_DATA_NONE;
  s_command.DummyCycles        = 0U;
  s_command.DQSMode            = HAL_XSPI_DQS_DISABLE;
#if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
#endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  /* Configure automatic polling mode to wait for write enabling */
  s_command.Instruction    = (Mode == W35T51NWTBIE_SPI_MODE)
                             ? W35T51NWTBIE_READ_STATUS_REG_CMD
                             : W35T51NWTBIE_OCTA_READ_STATUS_REG_CMD;
  s_command.AddressMode    = (Mode == W35T51NWTBIE_SPI_MODE) ? HAL_XSPI_ADDRESS_NONE : HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode = (Rate == W35T51NWTBIE_DTR_TRANSFER)
                             ? HAL_XSPI_ADDRESS_DTR_ENABLE
                             : HAL_XSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressWidth    = HAL_XSPI_ADDRESS_24_BITS;
  s_command.Address        = 0U;
  s_command.DataMode       = (Mode == W35T51NWTBIE_SPI_MODE) ? HAL_XSPI_DATA_1_LINE : HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode    = (Rate == W35T51NWTBIE_DTR_TRANSFER) ? HAL_XSPI_DATA_DTR_ENABLE : HAL_XSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles    = (Mode == W35T51NWTBIE_SPI_MODE)
                             ? 0U
                             : ((Rate == W35T51NWTBIE_DTR_TRANSFER)
                                ? DUMMY_CYCLES_REG_OCTAL_DTR
                                : DUMMY_CYCLES_REG_OCTAL);
  s_command.DataLength         = (Rate == W35T51NWTBIE_DTR_TRANSFER) ? 2U : 1U;
  s_command.DQSMode        = (Rate == W35T51NWTBIE_DTR_TRANSFER) ? HAL_XSPI_DQS_ENABLE : HAL_XSPI_DQS_DISABLE;

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  s_config.MatchValue           = 2U;
  s_config.MatchMask            = 2U;
  s_config.MatchMode       = HAL_XSPI_MATCH_MODE_AND;
  s_config.IntervalTime        = W35T51NWTBIE_AUTOPOLLING_INTERVAL_TIME;
  s_config.AutomaticStop   = HAL_XSPI_AUTOMATIC_STOP_ENABLE;

  if (HAL_XSPI_AutoPolling(Ctx, &s_config, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  return W35T51NWTBIE_OK;
}

/**
  * @brief  This function reset the (WEN) Write Enable Latch bit.
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  Rate Transfer rate STR or DTR
  * @retval error status
  */
int32_t W35T51NWTBIE_WriteDisable(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode, W35T51NWTBIE_Transfer_t Rate)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == W35T51NWTBIE_SPI_MODE) && (Rate == W35T51NWTBIE_DTR_TRANSFER))
  {
    return W35T51NWTBIE_ERROR;
  }

  /* Initialize the write disable command */
  s_command.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect           = HAL_XSPI_SELECT_IO_7_0;
  s_command.InstructionMode    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_1_LINE
                                 : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == W35T51NWTBIE_DTR_TRANSFER)
                                 ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                 : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_8_BITS
                                 : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction        = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? W35T51NWTBIE_WRITE_DISABLE_CMD
                                 : W35T51NWTBIE_OCTA_WRITE_DISABLE_CMD;
  s_command.AddressMode        = HAL_XSPI_ADDRESS_NONE;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode           = HAL_XSPI_DATA_NONE;
  s_command.DummyCycles        = 0U;
  s_command.DQSMode            = HAL_XSPI_DQS_DISABLE;
#if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
#endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  return W35T51NWTBIE_OK;
}

/**
  * @brief  Read Flash Status register value
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  Rate Transfer rate STR or DTR
  * @param  Value Status register value pointer
  * @retval error status
  */
int32_t W35T51NWTBIE_ReadStatusRegister(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode,
                                        W35T51NWTBIE_Transfer_t Rate, uint8_t *Value)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == W35T51NWTBIE_SPI_MODE) && (Rate == W35T51NWTBIE_DTR_TRANSFER))
  {
    return W35T51NWTBIE_ERROR;
  }

  /* Initialize the reading of status register */
  s_command.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect           = HAL_XSPI_SELECT_IO_7_0;
  s_command.InstructionMode    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_1_LINE
                                 : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == W35T51NWTBIE_DTR_TRANSFER)
                                 ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                 : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_8_BITS
                                 : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction        = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? W35T51NWTBIE_READ_STATUS_REG_CMD
                                 : W35T51NWTBIE_OCTA_READ_STATUS_REG_CMD;
  s_command.AddressMode        = (Mode == W35T51NWTBIE_SPI_MODE) ? HAL_XSPI_ADDRESS_NONE : HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode     = (Rate == W35T51NWTBIE_DTR_TRANSFER)
                                 ? HAL_XSPI_ADDRESS_DTR_ENABLE
                                 : HAL_XSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressWidth        = HAL_XSPI_ADDRESS_32_BITS;
  s_command.Address            = 0U;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode           = (Mode == W35T51NWTBIE_SPI_MODE) ? HAL_XSPI_DATA_1_LINE : HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode        = (Rate == W35T51NWTBIE_DTR_TRANSFER)
                                 ? HAL_XSPI_DATA_DTR_ENABLE
                                 : HAL_XSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles        = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? 0U
                                 : ((Rate == W35T51NWTBIE_DTR_TRANSFER)
                                    ? DUMMY_CYCLES_REG_OCTAL_DTR
                                    : DUMMY_CYCLES_REG_OCTAL);
  s_command.DataLength             = (Rate == W35T51NWTBIE_DTR_TRANSFER) ? 2U : 1U;
  s_command.DQSMode            = (Rate == W35T51NWTBIE_DTR_TRANSFER) ? HAL_XSPI_DQS_ENABLE : HAL_XSPI_DQS_DISABLE;
#if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
#endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  /* Reception of the data */
  if (HAL_XSPI_Receive(Ctx, Value, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  return W35T51NWTBIE_OK;
}

/**
  * @brief  Write Flash Status register
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  Rate Transfer rate STR or DTR
  * @param  Value Value to write to Status register
  * @retval error status
  */
// int32_t W35T51NWTBIE_WriteStatusRegister(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode,
//                                          W35T51NWTBIE_Transfer_t Rate, uint8_t Value)
// {
// }

/**
  * @brief  Write Flash configuration register
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  Rate Transfer rate STR or DTR
  * @param  Value Value to write to configuration register
  * @retval error status
  */
int32_t W35T51NWTBIE_WriteCfgRegister(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode,
                                      W35T51NWTBIE_Transfer_t Rate, uint8_t Value)
{
  XSPI_RegularCmdTypeDef s_command = {0};
  uint8_t reg[2];

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == W35T51NWTBIE_SPI_MODE) && (Rate == W35T51NWTBIE_DTR_TRANSFER))
  {
    return W35T51NWTBIE_ERROR;
  }

  /* In SPI mode, the configuration register is configured with status register */
  if (Mode == W35T51NWTBIE_SPI_MODE)
  {
    if (W35T51NWTBIE_ReadStatusRegister(Ctx, Mode, Rate, &reg[0]) != W35T51NWTBIE_OK)
    {
      return W35T51NWTBIE_ERROR;
    }
    reg[1] = Value;
  }
  else
  {
    reg[0] = Value;
  }

  /* Initialize the writing of configuration register */
  s_command.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect           = HAL_XSPI_SELECT_IO_7_0;
  s_command.InstructionMode    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_1_LINE
                                 : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == W35T51NWTBIE_DTR_TRANSFER)
                                 ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                 : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_8_BITS
                                 : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction        = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? W35T51NWTBIE_WRITE_STATUS_REG_CMD
                                 : W35T51NWTBIE_OCTA_WRITE_STATUS_REG_CMD;
  s_command.AddressMode        = (Mode == W35T51NWTBIE_SPI_MODE) ? HAL_XSPI_ADDRESS_NONE : HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode     = (Rate == W35T51NWTBIE_DTR_TRANSFER)
                                 ? HAL_XSPI_ADDRESS_DTR_ENABLE
                                 : HAL_XSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressWidth        = HAL_XSPI_ADDRESS_32_BITS;
  s_command.Address            = 1U;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode           = (Mode == W35T51NWTBIE_SPI_MODE) ? HAL_XSPI_DATA_1_LINE : HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode        = (Rate == W35T51NWTBIE_DTR_TRANSFER)
                                 ? HAL_XSPI_DATA_DTR_ENABLE
                                 : HAL_XSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles        = 0U;
  s_command.DataLength             = (Mode == W35T51NWTBIE_SPI_MODE) ? 2U : ((Rate == W35T51NWTBIE_DTR_TRANSFER) ? 2U : 1U);
  s_command.DQSMode            = HAL_XSPI_DQS_DISABLE;
#if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
#endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  if (HAL_XSPI_Transmit(Ctx, reg, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  return W35T51NWTBIE_OK;
}

/**
  * @brief  Read Flash configuration register value
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  Rate Transfer rate STR or DTR
  * @param  Value configuration register value pointer
  * @retval error status
  */
int32_t W35T51NWTBIE_ReadCfgRegister(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode,
                                     W35T51NWTBIE_Transfer_t Rate, uint8_t *Value)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == W35T51NWTBIE_SPI_MODE) && (Rate == W35T51NWTBIE_DTR_TRANSFER))
  {
    return W35T51NWTBIE_ERROR;
  }

  /* Initialize the reading of configuration register */
  s_command.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect           = HAL_XSPI_SELECT_IO_7_0;
  s_command.InstructionMode    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_1_LINE
                                 : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == W35T51NWTBIE_DTR_TRANSFER)
                                 ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                 : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_8_BITS
                                 : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction        = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? W35T51NWTBIE_READ_CFG_REG_CMD
                                 : W35T51NWTBIE_OCTA_READ_CFG_REG_CMD;
  s_command.AddressMode        = (Mode == W35T51NWTBIE_SPI_MODE) ? HAL_XSPI_ADDRESS_NONE : HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode     = (Rate == W35T51NWTBIE_DTR_TRANSFER)
                                 ? HAL_XSPI_ADDRESS_DTR_ENABLE
                                 : HAL_XSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressWidth        = HAL_XSPI_ADDRESS_32_BITS;
  s_command.Address            = 1U;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode           = (Mode == W35T51NWTBIE_SPI_MODE) ? HAL_XSPI_DATA_1_LINE : HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode        = (Rate == W35T51NWTBIE_DTR_TRANSFER)
                                 ? HAL_XSPI_DATA_DTR_ENABLE
                                 : HAL_XSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles        = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? 0U
                                 : ((Rate == W35T51NWTBIE_DTR_TRANSFER)
                                    ? DUMMY_CYCLES_REG_OCTAL_DTR
                                    : DUMMY_CYCLES_REG_OCTAL);
  s_command.DataLength             = (Rate == W35T51NWTBIE_DTR_TRANSFER) ? 2U : 1U;
  s_command.DQSMode            = (Rate == W35T51NWTBIE_DTR_TRANSFER) ? HAL_XSPI_DQS_ENABLE : HAL_XSPI_DQS_DISABLE;
#if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
#endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  /* Reception of the data */
  if (HAL_XSPI_Receive(Ctx, Value, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  return W35T51NWTBIE_OK;
}

/* ID Commands ****************************************************************/
/**
  * @brief  Read Flash 3 Byte IDs.
  *         Manufacturer ID, Memory type, Memory density
  *         SPI/OPI; 1-0-1/1-0-8
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  ID 3 bytes IDs pointer
  * @param  DualFlash Dual flash mode state
  * @retval error status
  */
int32_t W35T51NWTBIE_ReadID(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode, W35T51NWTBIE_Transfer_t Rate,
                            uint8_t *ID, W35T51NWTBIE_AddressSize_t AddressSize)
{
  XSPI_RegularCmdTypeDef s_command = {0};
  HAL_StatusTypeDef retr;
  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == W35T51NWTBIE_SPI_MODE) && (Rate == W35T51NWTBIE_DTR_TRANSFER))
  {
    // return W35T51NWTBIE_ERROR;
  }

  /* Initialize the read ID command */
  s_command.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect           = HAL_XSPI_SELECT_IO_7_0;
  s_command.InstructionMode    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_1_LINE
                                 : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == W35T51NWTBIE_DTR_TRANSFER)
                                 ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                 : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_8_BITS
                                 : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction        = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? W35T51NWTBIE_READ_ID_CMD
                                 : W35T51NWTBIE_OCTA_READ_ID_CMD;
  // s_command.AddressMode        = (Mode == W35T51NWTBIE_SPI_MODE)
  //                                ? HAL_XSPI_ADDRESS_NONE
  //                                : HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressMode        =  HAL_XSPI_ADDRESS_NONE;
  // s_command.AddressDTRMode     = (Rate == W35T51NWTBIE_DTR_TRANSFER)
  //                                ? HAL_XSPI_ADDRESS_DTR_ENABLE
  //                                : HAL_XSPI_ADDRESS_DTR_DISABLE;
  // s_command.AddressWidth        = (AddressSize == W35T51NWTBIE_3BYTES_SIZE)
  //                                ? HAL_XSPI_ADDRESS_24_BITS
  //                                : HAL_XSPI_ADDRESS_32_BITS;
  // s_command.Address            = 0U;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode           = (Mode == W35T51NWTBIE_SPI_MODE) ? HAL_XSPI_DATA_1_LINE : HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode        = HAL_XSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles        = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? 0U
                                 : ((Rate == W35T51NWTBIE_DTR_TRANSFER)
                                    ? DUMMY_CYCLES_REG_OCTAL_DTR
                                    : DUMMY_CYCLES_REG_OCTAL);
  s_command.DataLength             = 3U;
  s_command.DQSMode            = (Rate == W35T51NWTBIE_DTR_TRANSFER) ? HAL_XSPI_DQS_ENABLE : HAL_XSPI_DQS_DISABLE;
#if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
#endif

  /* Configure the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    goto error;
  }

  /* Reception of the data */
  if (HAL_XSPI_Receive(Ctx, ID, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    goto error;
  }
  return W35T51NWTBIE_OK;

error:
  if (retr != HAL_OK )
  {
    /* abort any ongoing transaction for the next action */
    (void)HAL_XSPI_Abort(Ctx);
  }

  return W35T51NWTBIE_ERROR;
}

/* Reset Commands *************************************************************/
/**
  * @brief  Flash reset enable command
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface select
  * @param  Rate Transfer rate STR or DTR
  * @retval error status
  */
int32_t W35T51NWTBIE_ResetEnable(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode, W35T51NWTBIE_Transfer_t Rate)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == W35T51NWTBIE_SPI_MODE) && (Rate == W35T51NWTBIE_DTR_TRANSFER))
  {
    return W35T51NWTBIE_ERROR;
  }

  /* Initialize the reset enable command */
  s_command.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect           = HAL_XSPI_SELECT_IO_7_0;
  s_command.InstructionMode    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_1_LINE
                                 : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == W35T51NWTBIE_DTR_TRANSFER)
                                 ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                 : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_8_BITS
                                 : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction        = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? W35T51NWTBIE_RESET_ENABLE_CMD
                                 : W35T51NWTBIE_OCTA_RESET_ENABLE_CMD;
  s_command.AddressMode        = HAL_XSPI_ADDRESS_NONE;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode           = HAL_XSPI_DATA_NONE;
  s_command.DummyCycles        = 0U;
  s_command.DQSMode            = HAL_XSPI_DQS_DISABLE;
#if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
#endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  return W35T51NWTBIE_OK;
}

/**
  * @brief  Flash reset memory command
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface select
  * @param  Rate Transfer rate STR or DTR
  * @retval error status
  */
int32_t W35T51NWTBIE_ResetMemory(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode, W35T51NWTBIE_Transfer_t Rate)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == W35T51NWTBIE_SPI_MODE) && (Rate == W35T51NWTBIE_DTR_TRANSFER))
  {
    return W35T51NWTBIE_ERROR;
  }

  /* Initialize the reset enable command */
  s_command.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect           = HAL_XSPI_SELECT_IO_7_0;
  s_command.InstructionMode    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_1_LINE
                                 : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == W35T51NWTBIE_DTR_TRANSFER)
                                 ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                 : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_8_BITS
                                 : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction        = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? W35T51NWTBIE_RESET_MEMORY_CMD
                                 : W35T51NWTBIE_OCTA_RESET_MEMORY_CMD;
  s_command.AddressMode        = HAL_XSPI_ADDRESS_NONE;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode           = HAL_XSPI_DATA_NONE;
  s_command.DummyCycles        = 0U;
  s_command.DQSMode            = HAL_XSPI_DQS_DISABLE;
#if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
#endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  return W35T51NWTBIE_OK;
}

/**
  * @brief  Flash enter deep power-down command
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface select
  * @param  Rate Transfer rate STR or DTR
  * @retval error status
  */
int32_t W35T51NWTBIE_EnterPowerDown(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode,
                                    W35T51NWTBIE_Transfer_t Rate)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == W35T51NWTBIE_SPI_MODE) && (Rate == W35T51NWTBIE_DTR_TRANSFER))
  {
    return W35T51NWTBIE_ERROR;
  }

  /* Initialize the enter power down command */
  s_command.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect           = HAL_XSPI_SELECT_IO_7_0;

  s_command.InstructionMode    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_1_LINE
                                 : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == W35T51NWTBIE_DTR_TRANSFER)
                                 ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                 : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_8_BITS
                                 : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction        = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? W35T51NWTBIE_ENTER_DEEP_POWER_DOWN_CMD
                                 : W35T51NWTBIE_OCTA_ENTER_DEEP_POWER_DOWN_CMD;
  s_command.AddressMode        = HAL_XSPI_ADDRESS_NONE;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode           = HAL_XSPI_DATA_NONE;
  s_command.DummyCycles        = 0U;
  s_command.DQSMode            = HAL_XSPI_DQS_DISABLE;
#if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
#endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  return W35T51NWTBIE_OK;
}

/**
  * @brief  Flash release from deep power-down command
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface select
  * @param  Rate Transfer rate STR or DTR
  * @retval error status
  */
int32_t W35T51NWTBIE_ReleaseFromPowerDown(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode,
                                          W35T51NWTBIE_Transfer_t Rate)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == W35T51NWTBIE_SPI_MODE) && (Rate == W35T51NWTBIE_DTR_TRANSFER))
  {
    return W35T51NWTBIE_ERROR;
  }

  /* Initialize the enter power down command */
  s_command.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect           = HAL_XSPI_SELECT_IO_7_0;

  s_command.InstructionMode    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_1_LINE
                                 : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == W35T51NWTBIE_DTR_TRANSFER)
                                 ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                 : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth    = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_8_BITS
                                 : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction        = (Mode == W35T51NWTBIE_SPI_MODE)
                                 ? W35T51NWTBIE_RELEASE_FROM_DEEP_POWER_DOWN_CMD
                                 : W35T51NWTBIE_OCTA_RELEASE_FROM_DEEP_POWER_DOWN_CMD;
  s_command.AddressMode        = HAL_XSPI_ADDRESS_NONE;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode           = HAL_XSPI_DATA_NONE;
  s_command.DummyCycles        = 0U;
  s_command.DQSMode            = HAL_XSPI_DQS_DISABLE;
#if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
#endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return W35T51NWTBIE_ERROR;
  }

  return W35T51NWTBIE_OK;
}

int32_t W35T51NWTBIE_EnterOctal_DTR_Mode(XSPI_HandleTypeDef *Ctx)
{
  uint8_t reg = 0;
  int32_t retr = W35T51NWTBIE_OK;
  XSPI_RegularCmdTypeDef  sCommand = {0};
  XSPI_AutoPollingTypeDef sConfig  = {0};

  sCommand.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  sCommand.InstructionMode    = HAL_XSPI_INSTRUCTION_1_LINE;
  sCommand.InstructionWidth    = HAL_XSPI_INSTRUCTION_8_BITS;
  sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.AddressDTRMode     = HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  sCommand.DataDTRMode        = HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles        = 0;
  sCommand.DQSMode            = HAL_XSPI_DQS_DISABLE;
  sConfig.MatchMode           = HAL_XSPI_MATCH_MODE_AND;
  sConfig.AutomaticStop       = HAL_XSPI_AUTOMATIC_STOP_ENABLE;
  sConfig.IntervalTime        = 0x10;

  {
    /* Enable write operations */
    sCommand.Instruction = W35T51NWTBIE_WRITE_ENABLE_CMD;    // 1-0-0
    sCommand.DataMode    = HAL_XSPI_DATA_NONE;
    sCommand.AddressMode = HAL_XSPI_ADDRESS_NONE;

    if (HAL_XSPI_Command(Ctx, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      retr = -1;
    }

    /* Reconfigure XSPI to automatic polling mode to wait for write enabling */
    sConfig.MatchMask           = 0x02;
    sConfig.MatchValue          = 0x02;

    sCommand.Instruction    = W35T51NWTBIE_READ_STATUS_REG_CMD;  // 1-0-1
    sCommand.DataMode       = HAL_XSPI_DATA_1_LINE;
    sCommand.DataLength         = 1;

    if (HAL_XSPI_Command(Ctx, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      retr = -2;
    }

    if (HAL_XSPI_AutoPolling(Ctx, &sConfig, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      retr = -3;
    }
  }

  {
    sCommand.Instruction = W35T51NWTBIE_WRITE_VOLATILE_CFG_REG_CMD;    //set 22 dummy cycles for fast read
    sCommand.InstructionMode    = HAL_XSPI_INSTRUCTION_1_LINE;
    sCommand.InstructionWidth    = HAL_XSPI_INSTRUCTION_8_BITS;
    sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
    sCommand.AddressMode = HAL_XSPI_ADDRESS_1_LINE;
    sCommand.Address            = 0x1;
    sCommand.AddressWidth        = HAL_XSPI_ADDRESS_24_BITS;
    sCommand.AddressDTRMode     = HAL_XSPI_ADDRESS_DTR_DISABLE;
    sCommand.DataMode    = HAL_XSPI_DATA_1_LINE;
    sCommand.DataDTRMode        = HAL_XSPI_DATA_DTR_DISABLE;
    sCommand.DataLength         = 1;
    sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
    sCommand.DummyCycles        = 0;
    sCommand.DQSMode            = HAL_XSPI_DQS_DISABLE;

    reg = 0x16;   // 22 dummy cycles

    if (HAL_XSPI_Command(Ctx, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      retr = -8;
    }

    if (HAL_XSPI_Transmit(Ctx, &reg, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      retr = -9;
    }

  }

    {
    /* Enable write operations */
    sCommand.Instruction = W35T51NWTBIE_WRITE_ENABLE_CMD;    // 1-0-0
    sCommand.DataMode    = HAL_XSPI_DATA_NONE;
    sCommand.AddressMode = HAL_XSPI_ADDRESS_NONE;

    if (HAL_XSPI_Command(Ctx, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      retr = -1;
    }

    /* Reconfigure XSPI to automatic polling mode to wait for write enabling */
    sConfig.MatchMask           = 0x02;
    sConfig.MatchValue          = 0x02;

    sCommand.Instruction    = W35T51NWTBIE_READ_STATUS_REG_CMD;  // 1-0-1
    sCommand.DataMode       = HAL_XSPI_DATA_1_LINE;
    sCommand.DataLength         = 1;

    if (HAL_XSPI_Command(Ctx, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      retr = -2;
    }

    if (HAL_XSPI_AutoPolling(Ctx, &sConfig, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      retr = -3;
    }
  }

  {
    sCommand.Instruction = W35T51NWTBIE_WRITE_VOLATILE_CFG_REG_CMD;    //set impedence of driver strength
    sCommand.InstructionMode    = HAL_XSPI_INSTRUCTION_1_LINE;
    sCommand.InstructionWidth    = HAL_XSPI_INSTRUCTION_8_BITS;
    sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
    sCommand.AddressMode = HAL_XSPI_ADDRESS_1_LINE;
    sCommand.Address            = 0x03;
    sCommand.AddressWidth        = HAL_XSPI_ADDRESS_24_BITS;
    sCommand.AddressDTRMode     = HAL_XSPI_ADDRESS_DTR_DISABLE;
    sCommand.DataMode    = HAL_XSPI_DATA_1_LINE;
    sCommand.DataDTRMode        = HAL_XSPI_DATA_DTR_DISABLE;
    sCommand.DataLength         = 1;
    sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
    sCommand.DummyCycles        = 0;
    sCommand.DQSMode            = HAL_XSPI_DQS_DISABLE;

    reg = 0xFF;   // 50R

    if (HAL_XSPI_Command(Ctx, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      retr = -8;
    }

    if (HAL_XSPI_Transmit(Ctx, &reg, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      retr = -9;
    }

  }

  {
    /* Enable write operations */
    sCommand.Instruction = W35T51NWTBIE_WRITE_ENABLE_CMD;    // 1-0-0
    sCommand.DataMode    = HAL_XSPI_DATA_NONE;
    sCommand.AddressMode = HAL_XSPI_ADDRESS_NONE;

    if (HAL_XSPI_Command(Ctx, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      retr = -1;
    }

    /* Reconfigure XSPI to automatic polling mode to wait for write enabling */
    sConfig.MatchMask           = 0x02;
    sConfig.MatchValue          = 0x02;

    sCommand.Instruction    = W35T51NWTBIE_READ_STATUS_REG_CMD;  // 1-0-1
    sCommand.DataMode       = HAL_XSPI_DATA_1_LINE;
    sCommand.DataLength         = 1;

    if (HAL_XSPI_Command(Ctx, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      retr = -2;
    }

    if (HAL_XSPI_AutoPolling(Ctx, &sConfig, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      retr = -3;
    }
  }

  {
    sCommand.Instruction = W35T51NWTBIE_WRITE_VOLATILE_CFG_REG_CMD;    // 1-1-1
    sCommand.InstructionMode    = HAL_XSPI_INSTRUCTION_1_LINE;
    sCommand.InstructionWidth    = HAL_XSPI_INSTRUCTION_8_BITS;
    sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
    sCommand.AddressMode = HAL_XSPI_ADDRESS_1_LINE;
    sCommand.Address            = 0x0;
    sCommand.AddressWidth        = HAL_XSPI_ADDRESS_24_BITS;
    sCommand.AddressDTRMode     = HAL_XSPI_ADDRESS_DTR_DISABLE;
    sCommand.DataMode    = HAL_XSPI_DATA_1_LINE;
    sCommand.DataDTRMode        = HAL_XSPI_DATA_DTR_DISABLE;
    sCommand.DataLength         = 1;
    sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
    sCommand.DummyCycles        = 0;
    sCommand.DQSMode            = HAL_XSPI_DQS_DISABLE;
    // reg = 0xDF;   // standard SPI
    reg = 0xE7;   // with in DQS IO Enabled
    // reg = 0xC7;   // with in DQS IO disabled

    if (HAL_XSPI_Command(Ctx, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      retr = -8;
    }

    if (HAL_XSPI_Transmit(Ctx, &reg, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      retr = -9;
    }

  }

  HAL_Delay(100);
//   {
//     sCommand.Instruction    = W35T51NWTBIE_OCTA_READ_STATUS_REG_CMD;    // 8d-0-8d dummy:8
//     sCommand.InstructionMode    = HAL_XSPI_INSTRUCTION_8_LINES;
//     sCommand.InstructionWidth    = HAL_XSPI_INSTRUCTION_8_BITS;
//     sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_ENABLE;
//     sCommand.AddressMode = HAL_XSPI_ADDRESS_NONE;
//     sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
//     sCommand.DataMode       = HAL_XSPI_DATA_8_LINES;
//     sCommand.DataDTRMode        = HAL_XSPI_DATA_DTR_ENABLE;
//     sCommand.DataLength     = 1;
//     sCommand.DummyCycles        = 8;
//     sCommand.DQSMode            = HAL_XSPI_DQS_ENABLE;

//     // sCommand.Instruction = READ_STATUS_REG_CMD;    // 1-0-1
//     // sCommand.InstructionMode    = HAL_XSPI_INSTRUCTION_1_LINE;
//     // sCommand.InstructionWidth    = HAL_XSPI_INSTRUCTION_8_BITS;
//     // sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
//     // sCommand.AddressMode = HAL_XSPI_ADDRESS_NONE;

//     // sCommand.DataMode    = HAL_XSPI_DATA_1_LINE;
//     // sCommand.DataDTRMode        = HAL_XSPI_DATA_DTR_DISABLE;
//     // sCommand.DataLength         = 1;
//     // sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
//     // sCommand.DummyCycles        = 0;
//     // sCommand.DQSMode            = HAL_XSPI_DQS_DISABLE;

//     sConfig.MatchMask           = 0x02;
//     sConfig.MatchValue          = 0x02;
//     // sConfig.MatchValue = 0U;
//     // sConfig.MatchMask = MX66UW1G45G_SR_WIP;
//     sConfig.MatchMode = HAL_XSPI_MATCH_MODE_AND;
//     sConfig.AutomaticStop       = HAL_XSPI_AUTOMATIC_STOP_ENABLE;
//     sConfig.IntervalTime        = 0x10;
//     if (HAL_XSPI_Command(Ctx, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
//     {
//       retr = -10;
//     }

//     if (HAL_XSPI_AutoPolling(Ctx, &sConfig, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
//     {
//       retr = -11;
//     }
// //    uint8_t reg_status[2];
// //    if (HAL_XSPI_Receive(Ctx, reg_status, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
// //    {
// //      retr = -11;
// //    }
//   }

  return retr;

}
int32_t W35T51NWTBIE_EnableMemoryMappedModeDTR(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode)
{
    HAL_StatusTypeDef retr;
    XSPI_RegularCmdTypeDef s_command = {0};
    XSPI_MemoryMappedTypeDef sMemMappedCfg = {0};

    /* Initialize the s_command */
    s_command.InstructionMode = HAL_XSPI_INSTRUCTION_8_LINES;
    s_command.InstructionWidth = HAL_XSPI_INSTRUCTION_16_BITS;
    s_command.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_ENABLE;

    s_command.AddressMode = HAL_XSPI_ADDRESS_8_LINES;
    s_command.AddressWidth = HAL_XSPI_ADDRESS_32_BITS;
    s_command.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_ENABLE;

    s_command.DataMode = HAL_XSPI_DATA_8_LINES;
    s_command.DataDTRMode = HAL_XSPI_DATA_DTR_ENABLE;
    s_command.DQSMode = HAL_XSPI_DQS_ENABLE;

    /* Initialize the read ID command */
    s_command.OperationType = HAL_XSPI_OPTYPE_READ_CFG;
    s_command.Instruction = W35T51NWTBIE_OCTA_READ_DTR_CMD;
    // s_command.Instruction = XSPI_FormatCommand(hXspi->commandExtension, s_command.InstructionWidth, CommandRead);
    s_command.DummyCycles = 22;   // 22 for 200Mhz ODDR mode.
    /* Configure the read command */
    retr = HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE);
    if (retr != HAL_OK)
    {
        goto error;
    }

    /* Initialize the read ID command */
    s_command.OperationType = HAL_XSPI_OPTYPE_WRITE_CFG;
    s_command.Instruction = W35T51NWTBIE_OCTA_PAGE_PROG_CMD;
    // s_command.Instruction = XSPI_FormatCommand(hXspi->commandExtension, s_command.InstructionWidth, CommandWrite);
    s_command.DummyCycles = 4;
    /* Configure the read command */
    retr = HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE);
    if (retr != HAL_OK)
    {
        goto error;
    }

    /* Activation of memory-mapped mode */
    sMemMappedCfg.TimeOutActivation = HAL_XSPI_TIMEOUT_COUNTER_DISABLE;
    sMemMappedCfg.TimeoutPeriodClock = 0x50;
    retr = HAL_XSPI_MemoryMapped(Ctx, &sMemMappedCfg);

error:
    if (retr != HAL_OK)
    {
        /* abort any ongoing transaction for the next action */
        (void)HAL_XSPI_Abort(Ctx);
    }
    /* return status */
    return retr;
}

int32_t W35T51NWTBIE_DisableMemoryMappedMode(XSPI_HandleTypeDef *Ctx)
{
  __DSB();
  return HAL_XSPI_Abort(Ctx);
}

int32_t W35T51NWTBIE_EnableMemoryMappedModeSDR(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode)
{
    HAL_StatusTypeDef retr;
    XSPI_RegularCmdTypeDef s_command = {0};
    XSPI_MemoryMappedTypeDef sMemMappedCfg = {0};

    /* Initialize the s_command */
    s_command.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
    s_command.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
    s_command.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;

    s_command.AddressMode = HAL_XSPI_ADDRESS_1_LINE;
    s_command.AddressWidth = HAL_XSPI_ADDRESS_32_BITS;
    s_command.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_DISABLE;

    s_command.DataMode = HAL_XSPI_DATA_1_LINE;
    s_command.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;
    s_command.DQSMode = HAL_XSPI_DQS_DISABLE;

    /* Initialize the read ID command */
    s_command.OperationType = HAL_XSPI_OPTYPE_READ_CFG;
    s_command.Instruction = W35T51NWTBIE_FAST_READ_CMD;
    // s_command.Instruction = XSPI_FormatCommand(hXspi->commandExtension, s_command.InstructionWidth, CommandRead);
    s_command.DummyCycles = 16;
    /* Configure the read command */
    retr = HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE);
    if (retr != HAL_OK)
    {
        goto error;
    }

    /* Initialize the read ID command */
    s_command.OperationType = HAL_XSPI_OPTYPE_WRITE_CFG;
    s_command.Instruction = W35T51NWTBIE_OCTA_PAGE_PROG_CMD;
    // s_command.Instruction = XSPI_FormatCommand(hXspi->commandExtension, s_command.InstructionWidth, CommandWrite);
    s_command.DummyCycles = 4;
    /* Configure the read command */
    retr = HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE);
    if (retr != HAL_OK)
    {
        goto error;
    }

    /* Activation of memory-mapped mode */
    sMemMappedCfg.TimeOutActivation = HAL_XSPI_TIMEOUT_COUNTER_DISABLE;
    sMemMappedCfg.TimeoutPeriodClock = 0x50;
    retr = HAL_XSPI_MemoryMapped(Ctx, &sMemMappedCfg);

error:
    if (retr != HAL_OK)
    {
        /* abort any ongoing transaction for the next action */
        (void)HAL_XSPI_Abort(Ctx);
    }
    /* return status */
    return retr;
}
