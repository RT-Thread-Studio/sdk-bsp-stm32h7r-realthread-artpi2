/**
  ******************************************************************************
  * @file    FatFs/FatFs_Shared_Device/Common/Inc/mmc_diskio.h
  * @author  MCD Application Team
  * @brief   Header for mmc_diskio.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MMC_DISKIO_H
#define __MMC_DISKIO_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7r_artpi2_mmc.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
//extern const Diskio_drvTypeDef  MMC_Driver;
typedef uint32_t    DSTATUS;
/* Disk Status Bits (DSTATUS) */

#define STA_NOINIT      0x01    /* Drive not initialized */
#define STA_NODISK      0x02    /* No medium in the drive */
#define STA_PROTECT     0x04    /* Write protected */

/* Results of Disk Functions */
typedef enum {
    RES_OK = 0,     /* 0: Successful */
    RES_ERROR,      /* 1: R/W Error */
    RES_WRPRT,      /* 2: Write Protected */
    RES_NOTRDY,     /* 3: Not Ready */
    RES_PARERR      /* 4: Invalid Parameter */
} DRESULT;


DSTATUS MMC_initialize(uint8_t lun);
DSTATUS MMC_getCardInfo(uint8_t lun);
DRESULT MMC_read(uint8_t lun, uint8_t *buff, uint16_t sector, uint32_t count);
DRESULT MMC_write(uint8_t lun, const uint8_t *buff, uint16_t sector, uint32_t count);
#endif /* __MMC_DISKIO_H */


