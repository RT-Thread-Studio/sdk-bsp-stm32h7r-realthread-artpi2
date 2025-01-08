/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-08-21     stackRyan    first version
 */

#ifndef _DRV_SSD2828_H_
#define _DRV_SSD2828_H_

#include<rtthread.h>
#include<rtdevice.h>
#include "board.h"

/* RM69330 RGB Timing profile */
/* Bridge side RGB Timing profile */
#define  SSD2828_WIDTH    ((uint16_t)454)          /* LCD PIXEL WIDTH         */
#define  SSD2828_HEIGHT   ((uint16_t)454)          /* LCD PIXEL HEIGHT          */

#define  SSD2828_HSYNC            ((uint16_t)20)   /* Horizontal synchronization */
#define  SSD2828_HBP              ((uint16_t)40)   /* Horizontal back porch     */
#define  SSD2828_HFP              ((uint16_t)20)   /* Horizontal front porch     */
#define  SSD2828_VSYNC            ((uint16_t)4)   /* Vertical synchronization   */
#define  SSD2828_VBP              ((uint16_t)12)    /* Vertical back porch       */
#define  SSD2828_VFP              ((uint16_t)20)    /* Vertical front porch      */


#define SSD2828_WriteREG     0x700000
#define SSD2828_WriteData    0x720000
#define SSD2828_ReadData     0x730000

/* SSD2828 registers */
#define		SSD2828_DIR	    0xB0
#define		SSD2828_VICR1	  0xB1
#define		SSD2828_VICR2	  0xB2
#define		SSD2828_VICR3	  0xB3
#define		SSD2828_VICR4	  0xB4
#define		SSD2828_VICR5	  0xB5
#define		SSD2828_VICR6	  0xB6
#define		SSD2828_CFGR	  0xB7
#define		SSD2828_VCR	    0xB8
#define		SSD2828_PCR	    0xB9
#define		SSD2828_PLCR	  0xBA
#define		SSD2828_CCR	    0xBB
#define		SSD2828_PSCR1	  0xBC
#define		SSD2828_PSCR2	  0xBD
#define		SSD2828_PSCR3	  0xBE
#define		SSD2828_PDR	    0xBF
#define		SSD2828_OCR	    0xC0
#define		SSD2828_MRSR	  0xC1
#define		SSD2828_RDCR	  0xC2
#define		SSD2828_ARSR	  0xC3
#define		SSD2828_LCR	    0xC4
#define		SSD2828_ICR	    0xC5
#define		SSD2828_ISR	    0xC6
#define		SSD2828_ESR	    0xC7
#define		SSD2828_DAR1	  0xC9
#define		SSD2828_DAR2	  0xCA
#define		SSD2828_DAR3	  0xCB
#define		SSD2828_DAR4	  0xCC
#define		SSD2828_DAR5	  0xCD
#define		SSD2828_DAR6	  0xCE
#define		SSD2828_HTTR1	  0xCF
#define		SSD2828_HTTR2	  0xD0
#define		SSD2828_LRTR1	  0xD1
#define		SSD2828_LRTR2	  0xD2
#define		SSD2828_TSR	    0xD3
#define		SSD2828_LRR	    0xD4
#define		SSD2828_PLLR	  0xD5
#define		SSD2828_TR	    0xD6
#define		SSD2828_TECR	  0xD7
#define		SSD2828_ACR1	  0xD8
#define		SSD2828_ACR2	  0xD9
#define		SSD2828_ACR3	  0xDA
#define		SSD2828_ACR4	  0xDB
#define		SSD2828_IOCR	  0xDC
#define		SSD2828_VICR7	  0xDD
#define		SSD2828_LCFR	  0xDE
#define		SSD2828_DAR7	  0xDF
#define		SSD2828_PUCR1	  0xE0
#define		SSD2828_PUCR2	  0xE1
#define		SSD2828_PUCR3	  0xE2
#define		SSD2828_CBCR1	  0xE9
#define		SSD2828_CBCR2	  0xEA
#define		SSD2828_CBSR	  0xEB
#define		SSD2828_ECR	    0xEC
#define		SSD2828_VSDR	  0xED
#define		SSD2828_TMR	    0xEE
#define		SSD2828_GPIO1	  0xEF
#define		SSD2828_GPIO2	  0xF0
#define		SSD2828_DLYA01	0xF1
#define		SSD2828_DLYA23	0xF2
#define		SSD2828_DLYB01	0xF3
#define		SSD2828_DLYB23	0xF4
#define		SSD2828_DLYC01	0xF5
#define		SSD2828_DLYC23	0xF6
#define		SSD2828_ACR5	  0xF7
#define		SSD2828_RR	    0xFF

#define	SSD2828_CFGR_HS					(1 << 0)
#define	SSD2828_CFGR_CKE				(1 << 1)
#define	SSD2828_CFGR_SLP				(1 << 2)
#define	SSD2828_CFGR_VEN				(1 << 3)
#define	SSD2828_CFGR_HCLK				(1 << 4)
#define	SSD2828_CFGR_CSS				(1 << 5)
#define	SSD2828_CFGR_DCS				(1 << 6)
#define	SSD2828_CFGR_REN				(1 << 7)
#define	SSD2828_CFGR_ECD				(1 << 8)
#define	SSD2828_CFGR_EOT				(1 << 9)
#define	SSD2828_CFGR_LPE				(1 << 10)
#define	SSD2828_CFGR_TXD				(1 << 11)

#define SSD2828_VICR6_BLLP              (1 << 5)

#define	SSD2828_VIDEO_MODE_NON_BURST_WITH_SYNC_PULSES	(0 << 2)
#define	SSD2828_VIDEO_MODE_NON_BURST_WITH_SYNC_EVENTS	(1 << 2)
#define	SSD2828_VIDEO_MODE_BURST			(2 << 2)

#define	SSD2828_LP_CLOCK_DIVIDER(n)			(((n) - 1) & 0x3F)

#define	SSD2828_VIDEO_PIXEL_FORMAT_16BPP		0
#define	SSD2828_VIDEO_PIXEL_FORMAT_18BPP_PACKED		1
#define	SSD2828_VIDEO_PIXEL_FORMAT_18BPP_LOOSELY_PACKED	2
#define	SSD2828_VIDEO_PIXEL_FORMAT_24BPP		3


/* RM69330 registers*/
#define CMD_DSI_SET_COLUMN_ADDRESS      0x2A
#define CMD_DSI_SET_PAGE_ADDRESS        0x2B
#define CMD_DSI_EXIT_SLEEP_MODE         0x11
#define CMD_DSI_SET_DISPLAY_ON          0x29

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

// void ssd2828_setmode(MIPI_ModeTypeDef m);
// MIPI_ReadTypeDef ssd2828_generic_read_dt14(uint8_t adr, uint16_t l, uint8_t *p);
// MIPI_ReadTypeDef ssd2828_dcs_read_dt06(uint8_t adr, uint16_t l, uint8_t *p);
void ssd2828_dcs_short_write(uint8_t n);
// void ssd2828_dcs_long_write(uint32_t n);
// void ssd2828_generic_short_write(uint8_t n);
// void ssd2828_generic_long_write(uint32_t n);
uint16_t ssd2828_read_reg(uint8_t reg);
int ssd2828_init(void);
void ssd2828_setreset(uint8_t t);
void ssd2828_shutdown(FunctionalState state);

#endif /* _DRV_SSD2828_H_ */
