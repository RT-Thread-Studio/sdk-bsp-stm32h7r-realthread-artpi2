/**
  ******************************************************************************
  * @file    w35t51nwtbie.h
  * @modify  RT-Thread Community Team
  * @brief   This file contains all the description of the
  *          W35T51NWTBIE OctoSPI memory.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef W35T51NWTBIE_H
#define W35T51NWTBIE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "w35t51nwtbie_conf.h"

/**
  * @brief  W35T51NWTBIE Size configuration
  */
#define W35T51NWTBIE_SECTOR_64K                   (uint32_t)(64 * 1024)        /* 1024 sectors of 64KBytes     */
#define W35T51NWTBIE_SUBSECTOR_4K                 (uint32_t)(4  * 1024)        /* 16384 subsectors of 4KBytes  */

#define W35T51NWTBIE_FLASH_SIZE                   (uint32_t)(512*1024*1024/8)  /* 512 Mbits => 64MBytes        */
#define W35T51NWTBIE_PAGE_SIZE                    (uint32_t)256                /* 131072 pages of 256 Bytes    */

/**
  * @brief  W35T51NWTBIE Timing configuration
  */

#define W35T51NWTBIE_BULK_ERASE_MAX_TIME          460000U
#define W35T51NWTBIE_SECTOR_ERASE_MAX_TIME        1000U
#define W35T51NWTBIE_SUBSECTOR_4K_ERASE_MAX_TIME  400U
#define W35T51NWTBIE_WRITE_REG_MAX_TIME           40U

#define W35T51NWTBIE_RESET_MAX_TIME               100U                 /* when SWreset during erase operation */

#define W35T51NWTBIE_AUTOPOLLING_INTERVAL_TIME    0x20U

/**
  * @brief  W35T51NWTBIE Error codes
  */
#define W35T51NWTBIE_OK                           (0)
#define W35T51NWTBIE_ERROR                        (-1)

/******************************************************************************
  * @brief  W35T51NWTBIE Commands
  ****************************************************************************/

/*******************************************************************/
/********************************* SPI  ****************************/
/*******************************************************************/

/***** READ/WRITE MEMORY Operations with 3-Byte Address checked OK ****************************/
#define W35T51NWTBIE_READ_CMD                             0x03U   /*!< Normal Read 3 Byte Address                            */
#define W35T51NWTBIE_FAST_READ_CMD                        0x0BU   /*!< Fast Read 3(4) Byte Address                           */
#define W35T51NWTBIE_PAGE_PROG_CMD                        0x02U   /*!< Page Program 3 Byte Address, max 256bytes per time    */
#define W35T51NWTBIE_SUBSECTOR_ERASE_4K_CMD               0x20U   /*!< SubSector Erase 4KB 3 Byte Address, typical 45ms, max 200ms */
#define W35T51NWTBIE_SECTOR_ERASE_32K_CMD                 0x52U   /*!< Sector Erase 32KB 3 Byte Address, typical 120ms, max 800ms  */
#define W35T51NWTBIE_SECTOR_ERASE_64K_CMD                 0xD8U   /*!< Sector Erase 64KB 3 Byte Address, typical 150ms, max 2000ms */
#define W35T51NWTBIE_BULK_ERASE_CMD                       0x60U   /*!< Bulk Erase                                            */

/***** READ/WRITE MEMORY Operations with 4-Byte Address checked OK ****************************/
#define W35T51NWTBIE_4_BYTE_ADDR_READ_CMD                 0x13U   /*!< Normal Read 4 Byte address                            */
#define W35T51NWTBIE_4_BYTE_ADDR_FAST_READ_CMD            0x0CU   /*!< Fast Read 4 Byte address                              */
#define W35T51NWTBIE_4_BYTE_PAGE_PROG_CMD                 0x12U   /*!< Page Program 4 Byte Address, max 256bytes per time    */
#define W35T51NWTBIE_4_BYTE_SUBSECTOR_ERASE_4K_CMD        0x21U   /*!< SubSector Erase 4KB 4 Byte Address, typical 45ms, max 200ms  */
#define W35T51NWTBIE_4_BYTE_SECTOR_ERASE_32K_CMD          0x5CU   /*!< SubSector Erase 4KB 4 Byte Address, typical 120ms, max 800ms */
#define W35T51NWTBIE_4_BYTE_SECTOR_ERASE_64K_CMD          0xDCU   /*!< Sector Erase 64KB 4 Byte Address, typical 150ms, max 2000ms  */

/***** Setting commands checked OK ************************************************************/
#define W35T51NWTBIE_WRITE_ENABLE_CMD                     0x06U   /*!< Write Enable                                          */
#define W35T51NWTBIE_WRITE_DISABLE_CMD                    0x04U   /*!< Write Disable                                         */
#define W35T51NWTBIE_PROG_ERASE_SUSPEND_CMD               0x75U //0xB0U   /*!< Program/Erase suspend                         */
#define W35T51NWTBIE_PROG_ERASE_RESUME_CMD                0x7AU //0x30U   /*!< Program/Erase resume                          */
#define W35T51NWTBIE_ENTER_DEEP_POWER_DOWN_CMD            0xB9U   /*!< Enter deep power down                                 */
#define W35T51NWTBIE_RELEASE_FROM_DEEP_POWER_DOWN_CMD     0xABU   /*!< Release from deep power down                          */

/***** RESET commands checked OK ************************************************************/
// #define W35T51NWTBIE_NOP_CMD                              0x00U   /*!< No operation                                          */
#define W35T51NWTBIE_RESET_ENABLE_CMD                     0x66U   /*!< Reset Enable                                          */
#define W35T51NWTBIE_RESET_MEMORY_CMD                     0x99U   /*!< Reset Memory                                          */

/***** Register Commands (SPI) ****************************************************/
#define W35T51NWTBIE_READ_ID_CMD                          0x9EU   /*!< Read IDentification                                   */
// #define W35T51NWTBIE_READ_SERIAL_FLASH_DISCO_PARAM_CMD    0x5AU   /*!< Read Serial Flash Discoverable Parameter              */
#define W35T51NWTBIE_READ_STATUS_REG_CMD                  0x05U   /*!< Read Status Register                                  */
#define W35T51NWTBIE_READ_CFG_REG_CMD                     0x85U //0x15U   /*!< Read configuration Register                           */
#define W35T51NWTBIE_WRITE_STATUS_REG_CMD                 0x01U   /*!< Write Status Register                                 */
#define W35T51NWTBIE_WRITE_VOLATILE_CFG_REG_CMD           0x81U   /*!< Write Status Register                                 */


/*******************************************************************/
/********************************* OPI  ****************************/
/*******************************************************************/

/***** READ/WRITE MEMORY Operations  ****************************/
#define W35T51NWTBIE_OCTA_READ_CMD                             0xEC13U  /*!< Octa IO Read                                     */
#define W35T51NWTBIE_OCTA_READ_DTR_CMD                         0xFDFDU//0xEE11U  /*!< Octa IO Read DTR                        */
#define W35T51NWTBIE_OCTA_PAGE_PROG_CMD                        0x8E8EU //0x12EDU  /*!< Octa Page Program                      */
#define W35T51NWTBIE_OCTA_SUBSECTOR_ERASE_4K_CMD               0x21DEU  /*!< Octa SubSector Erase 4KB                         */
#define W35T51NWTBIE_OCTA_SECTOR_ERASE_64K_CMD                 0xDC23U  /*!< Octa Sector Erase 64KB 3                         */
#define W35T51NWTBIE_OCTA_BULK_ERASE_CMD                       0x609FU  /*!< Octa Bulk Erase                                  */

/***** Setting commands ************************************************************/
#define W35T51NWTBIE_OCTA_WRITE_ENABLE_CMD                     0x06F9U   /*!< Octa Write Enable                               */
#define W35T51NWTBIE_OCTA_WRITE_DISABLE_CMD                    0x04FBU   /*!< Octa Write Disable                              */
#define W35T51NWTBIE_OCTA_PROG_ERASE_SUSPEND_CMD               0xB04FU   /*!< Octa Program/Erase suspend                      */
#define W35T51NWTBIE_OCTA_PROG_ERASE_RESUME_CMD                0x30CFU   /*!< Octa Program/Erase resume                       */
#define W35T51NWTBIE_OCTA_ENTER_DEEP_POWER_DOWN_CMD            0xB946U   /*!< Octa Enter deep power down                      */
#define W35T51NWTBIE_OCTA_RELEASE_FROM_DEEP_POWER_DOWN_CMD     0xAB54U   /*!< Octa Release from deep power down               */
#define W35T51NWTBIE_OCTA_SET_BURST_LENGTH_CMD                 0xC03FU   /*!< Octa Set burst length                           */
#define W35T51NWTBIE_OCTA_ENTER_SECURED_OTP_CMD                0xB14EU   /*!< Octa Enter secured OTP)                         */
#define W35T51NWTBIE_OCTA_EXIT_SECURED_OTP_CMD                 0xC13EU   /*!< Octa Exit secured OTP)                          */

/***** RESET commands ************************************************************/
#define W35T51NWTBIE_OCTA_NOP_CMD                              0x00FFU   /*!< Octa No operation                               */
#define W35T51NWTBIE_OCTA_RESET_ENABLE_CMD                     0x6666U   /*!< Octa Reset Enable                               */
#define W35T51NWTBIE_OCTA_RESET_MEMORY_CMD                     0x9999U   /*!< Octa Reset Memory                               */

/***** Register Commands (OPI) ****************************************************/
#define W35T51NWTBIE_OCTA_READ_ID_CMD                          0x9E9EU //0x9F60U   /*!< Octa Read IDentification              */
#define W35T51NWTBIE_OCTA_READ_SERIAL_FLASH_DISCO_PARAM_CMD    0x5AA5U   /*!< Octa Read Serial Flash Discoverable Parameter   */
#define W35T51NWTBIE_OCTA_READ_STATUS_REG_CMD                  0x0505U //0x05FAU   /*!< Octa Read Status Register             */
#define W35T51NWTBIE_OCTA_READ_CFG_REG_CMD                     0x15EAU   /*!< Octa Read configuration Register                */
#define W35T51NWTBIE_OCTA_WRITE_STATUS_REG_CMD                 0x01FEU   /*!< Octa Write Status Register                      */
#define W35T51NWTBIE_OCTA_READ_CFG_REG2_CMD                    0x718EU   /*!< Octa Read configuration Register2               */
#define W35T51NWTBIE_OCTA_WRITE_CFG_REG2_CMD                   0x728DU   /*!< Octa Write configuration Register2              */
#define W35T51NWTBIE_OCTA_READ_FAST_BOOT_REG_CMD               0x16E9U   /*!< Octa Read fast boot Register                    */
#define W35T51NWTBIE_OCTA_WRITE_FAST_BOOT_REG_CMD              0x17E8U   /*!< Octa Write fast boot Register                   */
#define W35T51NWTBIE_OCTA_ERASE_FAST_BOOT_REG_CMD              0x18E7U   /*!< Octa Erase fast boot Register                   */
#define W35T51NWTBIE_OCTA_READ_SECURITY_REG_CMD                0x2BD4U   /*!< Octa Read security Register                     */
#define W35T51NWTBIE_OCTA_WRITE_SECURITY_REG_CMD               0x2FD0U   /*!< Octa Write security Register                    */
#define W35T51NWTBIE_OCTA_READ_LOCK_REG_CMD                    0x2DD2U   /*!< Octa Read lock Register                         */
#define W35T51NWTBIE_OCTA_WRITE_LOCK_REG_CMD                   0x2CD3U   /*!< Octa Write lock Register                        */
#define W35T51NWTBIE_OCTA_READ_DPB_REG_CMD                     0xE01FU   /*!< Octa Read DPB register                          */
#define W35T51NWTBIE_OCTA_WRITE_DPB_REG_CMD                    0xE11EU   /*!< Octa Write DPB register                         */
#define W35T51NWTBIE_OCTA_READ_SPB_STATUS_CMD                  0xE21DU   /*!< Octa Read SPB status                            */
#define W35T51NWTBIE_OCTA_WRITE_SPB_BIT_CMD                    0xE31CU   /*!< Octa SPB bit program                            */
#define W35T51NWTBIE_OCTA_ERASE_ALL_SPB_CMD                    0xE41BU   /*!< Octa Erase all SPB bit                          */
#define W35T51NWTBIE_OCTA_WRITE_PROTECT_SEL_CMD                0x6897U   /*!< Octa Write Protect selection                    */
#define W35T51NWTBIE_OCTA_GANG_BLOCK_LOCK_CMD                  0x7E81U   /*!< Octa Gang block lock: whole chip write protect  */
#define W35T51NWTBIE_OCTA_GANG_BLOCK_UNLOCK_CMD                0x9867U   /*!< Octa Gang block unlock: whole chip write unprote*/
#define W35T51NWTBIE_OCTA_READ_PASSWORD_REGISTER_CMD           0x27D8U   /*!< Octa Read Password                              */
#define W35T51NWTBIE_OCTA_WRITE_PASSWORD_REGISTER_CMD          0x28D7U   /*!< Octa Write Password                             */
#define W35T51NWTBIE_OCTA_PASSWORD_UNLOCK_CMD                  0x29D6U   /*!< Octa Unlock Password                            */
#define W35T51NWTBIE_PAGE_BUFFER_READ                          0x25DAU   /*!< Octa Page Buffer Read                           */
#define W35T51NWTBIE_WRITE_BUFFER_INITIAL                      0x22DDU   /*!< Octa Write Buffer Initial                       */
#define W35T51NWTBIE_WRITE_BUFFER_CONTINUE                     0x24DBU   /*!< Octa Write Buffer Continue                      */
#define W35T51NWTBIE_WRITE_BUFFER_CONFIRM                      0x31CEU   /*!< Octa Write Buffer Confirm                       */

/******************************************************************************
  * @brief  W35T51NWTBIE Registers
  ****************************************************************************/
/* Status Register */
#define W35T51NWTBIE_SR_WIP                               0x01U   /*!< Write in progress                                       */
#define W35T51NWTBIE_SR_WEL                               0x02U   /*!< Write enable latch                                      */


/**
  * @}
  */

/** @defgroup W35T51NWTBIE_Exported_Types W35T51NWTBIE Exported Types
  * @{
  */
typedef struct
{
  uint32_t FlashSize;                        /*!< Size of the flash                             */
  uint32_t EraseSectorSize;                  /*!< Size of sectors for the erase operation       */
  uint32_t EraseSectorsNumber;               /*!< Number of sectors for the erase operation     */
  uint32_t EraseSubSectorSize;               /*!< Size of subsector for the erase operation     */
  uint32_t EraseSubSectorNumber;             /*!< Number of subsector for the erase operation   */
  uint32_t EraseSubSector1Size;              /*!< Size of subsector 1 for the erase operation   */
  uint32_t EraseSubSector1Number;            /*!< Number of subsector 1 for the erase operation */
  uint32_t ProgPageSize;                     /*!< Size of pages for the program operation       */
  uint32_t ProgPagesNumber;                  /*!< Number of pages for the program operation     */
} W35T51NWTBIE_Info_t;

typedef enum
{
  W35T51NWTBIE_SPI_MODE = 0,                 /*!< 1-1-1 commands, Power on H/W default setting  */
  W35T51NWTBIE_OPI_MODE                      /*!< 8-8-8 commands                                */
} W35T51NWTBIE_Interface_t;

typedef enum
{
  W35T51NWTBIE_STR_TRANSFER = 0,             /*!< Single Transfer Rate                          */
  W35T51NWTBIE_DTR_TRANSFER                  /*!< Double Transfer Rate                          */
} W35T51NWTBIE_Transfer_t;

typedef enum
{
  W35T51NWTBIE_ERASE_4K = 0,                 /*!< 4K size Sector erase                          */
  W35T51NWTBIE_ERASE_32K,                    /*!< 64K size Block erase                          */
  W35T51NWTBIE_ERASE_64K,                    /*!< 64K size Block erase                          */
  W35T51NWTBIE_ERASE_BULK                    /*!< Whole bulk erase                              */
} W35T51NWTBIE_Erase_t;

typedef enum
{
  W35T51NWTBIE_3BYTES_SIZE = 0,              /*!< 3 Bytes address mode                           */
  W35T51NWTBIE_4BYTES_SIZE                   /*!< 4 Bytes address mode                           */
} W35T51NWTBIE_AddressSize_t;

/**
  * @}
  */

/** @defgroup W35T51NWTBIE_Exported_Functions W35T51NWTBIE Exported Functions
  * @{
  */
/* Function by commands combined */
int32_t W35T51NWTBIE_GetFlashInfo(W35T51NWTBIE_Info_t *pInfo);
int32_t W35T51NWTBIE_AutoPollingMemReady(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode,
                                         W35T51NWTBIE_Transfer_t Rate);

/* Read/Write Array Commands **************************************************/
int32_t W35T51NWTBIE_ReadSTR(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode,
                             W35T51NWTBIE_AddressSize_t AddressSize, uint8_t *pData, uint32_t ReadAddr, uint32_t Size);
// int32_t W35T51NWTBIE_ReadDTR(XSPI_HandleTypeDef *Ctx, uint8_t *pData, uint32_t ReadAddr, uint32_t Size);
int32_t W35T51NWTBIE_PageProgram(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode,
                                 W35T51NWTBIE_AddressSize_t AddressSize, uint8_t *pData, uint32_t WriteAddr,
                                 uint32_t Size);
// int32_t W35T51NWTBIE_PageProgramDTR(XSPI_HandleTypeDef *Ctx, uint8_t *pData, uint32_t WriteAddr, uint32_t Size);
int32_t W35T51NWTBIE_BlockErase(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode, W35T51NWTBIE_Transfer_t Rate,
                                W35T51NWTBIE_AddressSize_t AddressSize, uint32_t BlockAddress,
                                W35T51NWTBIE_Erase_t BlockSize);
int32_t W35T51NWTBIE_ChipErase(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode, W35T51NWTBIE_Transfer_t Rate);
// int32_t W35T51NWTBIE_EnableMemoryMappedModeSTR(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode,
//                                                W35T51NWTBIE_AddressSize_t AddressSize);
int32_t W35T51NWTBIE_EnterOctal_DTR_Mode(XSPI_HandleTypeDef *Ctx);
int32_t W35T51NWTBIE_EnableMemoryMappedModeDTR(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode);
int32_t W35T51NWTBIE_DisableMemoryMappedMode(XSPI_HandleTypeDef *Ctx);

/* Register/Setting Commands **************************************************/
int32_t W35T51NWTBIE_WriteEnable(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode, W35T51NWTBIE_Transfer_t Rate);


/* ID/Security Commands *******************************************************/
int32_t W35T51NWTBIE_ReadID(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode, W35T51NWTBIE_Transfer_t Rate,
                            uint8_t *ID, W35T51NWTBIE_AddressSize_t AddressSize);

/* Reset Commands *************************************************************/
int32_t W35T51NWTBIE_ResetEnable(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode, W35T51NWTBIE_Transfer_t Rate);
int32_t W35T51NWTBIE_ResetMemory(XSPI_HandleTypeDef *Ctx, W35T51NWTBIE_Interface_t Mode, W35T51NWTBIE_Transfer_t Rate);


/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* W35T51NWTBIE_H */
