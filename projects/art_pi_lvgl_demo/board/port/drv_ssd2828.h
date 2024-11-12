/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-08-21     stackRyan    first version
 */

#ifndef _SSD2828_H_
#define _SSD2828_H_
#include "stm32h7rsxx_hal.h"

#define  SSD2828_WIDTH    ((uint16_t)454)          /* LCD PIXEL WIDTH          1600 */
#define  SSD2828_HEIGHT   ((uint16_t)454)          /* LCD PIXEL HEIGHT          */

// #define  SSD2828_HSYNC            ((uint16_t)1)   /* Horizontal synchronization */
// #define  SSD2828_HBP              ((uint16_t)1)   /* Horizontal back porch     */
// #define  SSD2828_HFP              ((uint16_t)1)   /* Horizontal front porch     */
// #define  SSD2828_VSYNC            ((uint16_t)1)   /* Vertical synchronization   */
// #define  SSD2828_VBP              ((uint16_t)1)    /* Vertical back porch       */
// #define  SSD2828_VFP              ((uint16_t)1)    /* Vertical front porch      */

#define  SSD2828_HSYNC            ((uint16_t)20)   /* Horizontal synchronization */
#define  SSD2828_HBP              ((uint16_t)40)   /* Horizontal back porch     */
#define  SSD2828_HFP              ((uint16_t)20)   /* Horizontal front porch     */
#define  SSD2828_VSYNC            ((uint16_t)4)   /* Vertical synchronization   */
#define  SSD2828_VBP              ((uint16_t)12)    /* Vertical back porch       */
#define  SSD2828_VFP              ((uint16_t)20)    /* Vertical front porch      */


#define SSD2828_WriteREG     0x700000
#define SSD2828_WriteData    0x720000
#define SSD2828_ReadData     0x730000

typedef enum
{
  MIPI_READ_FAIL,
  MIPI_READ_SUCCEED
} MIPI_ReadTypeDef;

typedef enum
{
  LP,
  HS,
  VD
} MIPI_ModeTypeDef;


void SSD2828_ShutDown(FunctionalState state);
//void SSD2828_Init(uint8_t lane, uint16_t data_rate);
void SSD2828_SetMode(MIPI_ModeTypeDef m);
MIPI_ReadTypeDef SSD2828_GenericReadDT14(uint8_t adr, uint16_t l, uint8_t *p);
MIPI_ReadTypeDef SSD2828_DcsReadDT06(uint8_t adr, uint16_t l, uint8_t *p);
void SSD2828_DcsShortWrite(uint8_t n);
void SSD2828_DcsLongWrite(uint32_t n);
void SSD2828_GenericShortWrite(uint8_t n);
void SSD2828_GenericLongWrite(uint32_t n);
uint16_t SSD2828ReadReg(uint8_t reg);
void SSD2828_SetReset(uint8_t t);
//void SSD2828_Reset(void);
#endif
