/**
  ******************************************************************************
  * @file    w35t51nwtbie_conf.h
  * @author  RT-Thread Community Team
  * @brief   W35T51NWTBIE OctoSPI memory configuration template file.
  *          This file should be copied to the application folder and renamed
  *          to w35t51nwtbie_conf.h
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef W35T51NWTBIE_CONF_H
#define W35T51NWTBIE_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7rsxx_hal.h"

/** @addtogroup BSP
  * @{
  */
#define CONF_OSPI_ODS                W35T51NWTBIE_CR_ODS_24   /* W35T51NWTBIE Output Driver Strength */

#define DUMMY_CYCLES_READ            8U
#define DUMMY_CYCLES_READ_OCTAL      6U
#define DUMMY_CYCLES_READ_OCTAL_DTR  6U
#define DUMMY_CYCLES_REG_OCTAL       4U
#define DUMMY_CYCLES_REG_OCTAL_DTR   8U

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* W35T51NWTBIE_CONF_H */
