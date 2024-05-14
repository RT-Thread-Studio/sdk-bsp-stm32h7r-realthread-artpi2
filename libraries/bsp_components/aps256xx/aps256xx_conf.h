/**
  ******************************************************************************
  * @file    aps256xx_conf.h
  * @author  MCD Application Team
  * @brief   APS256XX 16bits-OSPI PSRAM memory configuration template file.
  *          This file should be copied to the application folder and renamed
  *          to aps256xx_conf.h
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APS256XX_CONF_H
#define APS256XX_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7rsxx_hal.h"

/** @addtogroup BSP
  * @{
  */
#define CONF_HSPI_DS   APS256XX_MR0_DS_HALF
#define CONF_HSPI_PASR APS256XX_MR4_PASR_FULL
#define CONF_HSPI_RF   APS256XX_MR4_RF_4X

#define DEFAULT_READ_LATENCY_CODE  APS256XX_READ_LATENCY_5
#define DEFAULT_WRITE_LATENCY_CODE APS256XX_WRITE_LATENCY_5
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* APS256XX_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
