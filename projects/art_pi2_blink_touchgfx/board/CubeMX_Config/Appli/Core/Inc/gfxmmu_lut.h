/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : gfxmmu_lut.h
  * Description        : header file for GFX MMU Configuration Table
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __gfxmmu_lut_H
#define __gfxmmu_lut_H
#ifdef __cplusplus
 extern "C" {
#endif
// GFX MMU Configuration Table

  #define GFXMMU_FB_SIZE 0
  #define GFXMMU_LUT_FIRST 0
  #define GFXMMU_LUT_LAST  0
  #define GFXMMU_LUT_SIZE 1

uint32_t gfxmmu_lut_config[2*GFXMMU_LUT_SIZE] = {
  0x0
};

#ifdef __cplusplus
}
#endif
#endif /*__ gfxmmu_lut_H */

/**
  * @}
  */

/**
  * @}
  */
