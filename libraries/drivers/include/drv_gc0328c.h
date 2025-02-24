/*
 * Copyright (c) 2006-2025, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-27     thread-liu   first version
 */

#ifndef __DRV_GC0328C_H__
#define __DRV_GC0328C_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <board.h>

#define GC0328C_ID              0X9d

const rt_uint8_t gc0328c_init_talbe[][2] = {
    {0xFE,0x80},
    {0xFE,0x80},
    {0xFC,0x16},
    {0xFC,0x16},
    {0xFC,0x16},
    {0xFC,0x16},
    {0xF1,0x00},
    {0xF2,0x00},
    {0xFE,0x00},
    {0x4F,0x00},
    {0x03,0x00},
    {0x04,0xC0},
    {0x42,0x00},
    {0x77,0x5A},
    {0x78,0x40},
    {0x79,0x56},
    {0xFE,0x00},
    {0x0D,0x01},
    {0x0E,0xE8},
    {0x0F,0x02},
    {0x10,0x88},
    {0x09,0x00},
    {0x0A,0x00},
    {0x0B,0x00},
    {0x0C,0x00},
    {0x16,0x00},
    {0x17,0x14},
    {0x18,0x0E},
    {0x19,0x06},
    {0x1B,0x48},
    {0x1F,0xC8},
    {0x20,0x01},
    {0x21,0x78},
    {0x22,0xB0},
    {0x23,0x04},
    {0x24,0x16},
    {0x26,0x00},
    {0x50,0x01},
    {0x70,0x45},
    {0x05,0x00},
    {0x06,0xDE},
    {0x07,0x00},
    {0x08,0xA7},
    {0xFE,0x01},
    {0x29,0x00},
    {0x2A,0x83},
    {0x2B,0x02},
    {0x2C,0x8F},
    {0x2D,0x02},
    {0x2E,0x8F},
    {0x2F,0x02},
    {0x30,0x8F},
    {0x31,0x02},
    {0x32,0x8F},
    {0xFE,0x00},
    {0xFE,0x01},
    {0x50,0x00},
    {0x4F,0x00},
    {0x4C,0x01},
    {0x4F,0x00},
    {0x4F,0x00},
    {0x4F,0x00},
    {0x4F,0x00},
    {0x4F,0x00},
    {0x4D,0x30},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4D,0x40},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4D,0x50},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4D,0x60},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4D,0x70},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4F,0x01},
    {0x50,0x88},
    {0xFE,0x00},
    {0xFE,0x00},
    {0x27,0xB7},
    {0x28,0x7F},
    {0x29,0x20},
    {0x33,0x20},
    {0x34,0x20},
    {0x35,0x20},
    {0x36,0x20},
    {0x32,0x08},
    {0x3B,0x00},
    {0x3C,0x00},
    {0x3D,0x00},
    {0x3E,0x00},
    {0x47,0x00},
    {0x48,0x00},
    {0x40,0x7F},
    {0x41,0x26},
    {0x42,0xFB},
    {0x44,0x26},    //02=yuv=22   26=rgb565
    {0x45,0x00},
    {0x46,0x03},    //HSYNC and VSYNC polarity
    {0x49,0x23},    //first_second_switch
    {0x4F,0x01},
    {0x4B,0x01},
    {0x50,0x01},
    {0x7E,0x0A},
    {0x7F,0x03},
    {0x81,0x15},
    {0x82,0x90},
    {0x83,0x02},
    {0x84,0xE5},
    {0x90,0x2C},
    {0x92,0x02},
    {0x94,0x02},
    {0x95,0x35},
    {0xD1,0x24},
    {0xD2,0x24},
    {0xD3,0x40},
    {0xDD,0xD3},
    {0xDE,0x38},
    {0xE4,0x88},
    {0xE5,0x40},
    {0xD7,0x0E},
    {0xFE,0x00},
    {0xBF,0x0E},
    {0xC0,0x1C},
    {0xC1,0x34},
    {0xC2,0x48},
    {0xC3,0x5A},
    {0xC4,0x6E},
    {0xC5,0x80},
    {0xC6,0x9C},
    {0xC7,0xB4},
    {0xC8,0xC7},
    {0xC9,0xD7},
    {0xCA,0xE3},
    {0xCB,0xED},
    {0xCC,0xF2},
    {0xCD,0xF8},
    {0xCE,0xFD},
    {0xCF,0xFF},
    {0xFE,0x00},
    {0x63,0x00},
    {0x64,0x05},
    {0x65,0x0B},
    {0x66,0x19},
    {0x67,0x2E},
    {0x68,0x40},
    {0x69,0x54},
    {0x6A,0x66},
    {0x6B,0x86},
    {0x6C,0xA7},
    {0x6D,0xC6},
    {0x6E,0xE4},
    {0x6F,0xFF},
    {0xFE,0x01},
    {0x18,0x02},
    {0xFE,0x00},
    {0x97,0x30},
    {0x98,0x00},
    {0x9B,0x60},
    {0x9C,0x60},
    {0xA4,0x50},
    {0xA8,0x80},
    {0xAA,0x40},
    {0xA2,0x23},
    {0xAD,0x28},
    {0xFE,0x01},
    {0x9C,0x00},
    {0x9E,0xC0},
    {0x9F,0x40},
    {0xFE,0x01},
    {0x08,0xA0},
    {0x09,0xE8},
    {0x10,0x08},
    {0x11,0x21},
    {0x12,0x11},
    {0x13,0x45},
    {0x15,0xFC},
    {0x18,0x02},
    {0x21,0xF0},
    {0x22,0x60},
    {0x23,0x30},
    {0x25,0x00},
    {0x24,0x14},
    {0x3D,0x80},
    {0x3E,0x40},
    {0xFE,0x01},
    {0x51,0x88},
    {0x52,0x12},
    {0x53,0x80},
    {0x54,0x60},
    {0x55,0x01},
    {0x56,0x02},
    {0x58,0x00},
    {0x5B,0x02},
    {0x5E,0xA4},
    {0x5F,0x8A},
    {0x61,0xDC},
    {0x62,0xDC},
    {0x70,0xFC},
    {0x71,0x10},
    {0x72,0x10},
    {0x73,0x20},
    {0x74,0x20},
    {0x75,0x01},
    {0x76,0x00},
    {0x77,0x40},
    {0x78,0x70},
    {0x79,0x00},
    {0x7B,0x00},
    {0x7C,0x71},
    {0x7D,0x00},
    {0x80,0x70},
    {0x81,0x58},
    {0x82,0x98},
    {0x83,0x60},
    {0x84,0x58},
    {0x85,0x50},
    {0xFE,0x00},
    {0xFE,0x01},
    {0xC0,0x10},
    {0xC1,0x0C},
    {0xC2,0x0A},
    {0xC6,0x0E},
    {0xC7,0x0B},
    {0xC8,0x0A},
    {0xBA,0x26},
    {0xBB,0x1C},
    {0xBC,0x1D},
    {0xB4,0x23},
    {0xB5,0x1C},
    {0xB6,0x1A},
    {0xC3,0x00},
    {0xC4,0x00},
    {0xC5,0x00},
    {0xC9,0x00},
    {0xCA,0x00},
    {0xCB,0x00},
    {0xBD,0x00},
    {0xBE,0x00},
    {0xBF,0x00},
    {0xB7,0x07},
    {0xB8,0x05},
    {0xB9,0x05},
    {0xA8,0x07},
    {0xA9,0x06},
    {0xAA,0x00},
    {0xAB,0x04},
    {0xAC,0x00},
    {0xAD,0x02},
    {0xAE,0x0D},
    {0xAF,0x05},
    {0xB0,0x00},
    {0xB1,0x07},
    {0xB2,0x03},
    {0xB3,0x00},
    {0xA4,0x00},
    {0xA5,0x00},
    {0xA6,0x00},
    {0xA7,0x00},
    {0xA1,0x3C},
    {0xA2,0x50},
    {0xFE,0x00},
    {0xB1,0x12},
    {0xB2,0xF5},
    {0xB3,0xFE},
    {0xB4,0xE0},
    {0xB5,0x15},
    {0xB6,0xC8},
    {0xFE,0x01},
    {0x50,0x00},
    {0xFE,0x01},
    {0x4F,0x00},
    {0x4C,0x01},
    {0x4F,0x00},
    {0x4F,0x00},
    {0x4F,0x00},
    {0x4D,0x34},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x02},
    {0x4E,0x02},
    {0x4D,0x44},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4D,0x53},
    {0x4E,0x08},
    {0x4E,0x08},
    {0x4E,0x04},
    {0x4E,0x04},
    {0x4D,0x65},
    {0x4E,0x04},
    {0x4D,0x73},
    {0x4E,0x20},
    {0x4D,0x83},
    {0x4E,0x20},
    {0x4F,0x01},
    {0x50,0x88},
    {0xFE,0x00},
    {0xF1,0x07},
    {0xF2,0x01},
};

const rt_uint8_t gc0328c_5pfs_talbe[][2] =
{
    // all AEC_EXP_LEVEL_X set to 0xa3c = 2620
    {0xFE,0x01},  // page p1

    {0x2B,0x0a},  // AEC_EXP_LEVEL_0 [11:8]
    {0x2C,0x3c},  // AEC_EXP_LEVEL_0 [7:0]

    {0x2D,0x0a},  // AEC_EXP_LEVEL_1
    {0x2E,0x3c},

    {0x2F,0x0a},  // AEC_EXP_LEVEL_2
    {0x30,0x3c},

    {0x31,0x0a},  // AEC_EXP_LEVEL_3
    {0x32,0x3c},

    //{0x33,0x2a},  // AEC_EXP_MIN
    //{0x34,0x3c},
};

const rt_uint8_t gc0328c_10pfs_talbe[][2] =
{
    // all AEC_EXP_LEVEL_X set to 0x51e = 1310
    {0xFE,0x01},  // page p1

    {0x2B,0x05},  // AEC_EXP_LEVEL_0 [11:8]
    {0x2C,0x1e},  // AEC_EXP_LEVEL_0 [7:0]

    {0x2D,0x05},  // AEC_EXP_LEVEL_1
    {0x2E,0x1e},

    {0x2F,0x05},  // AEC_EXP_LEVEL_2
    {0x30,0x1e},

    {0x31,0x05},  // AEC_EXP_LEVEL_3
    {0x32,0x1e},

    //{0x33,0x25},  // // AEC_EXP_MIN
    //{0x34,0x1e},
};

const rt_uint8_t gc0328c_20pfs_talbe[][2] =
{
    // all AEC_EXP_LEVEL_X set to 0x28f = 655
    {0xFE,0x01},  // page p1

    {0x2B,0x02},  // AEC_EXP_LEVEL_0 [11:8]
    {0x2C,0x8f},  // AEC_EXP_LEVEL_0 [7:0]

    {0x2D,0x02},  // AEC_EXP_LEVEL_1
    {0x2E,0x8f},

    {0x2F,0x02},  // AEC_EXP_LEVEL_2
    {0x30,0x8f},

    {0x31,0x02},  // AEC_EXP_LEVEL_3
    {0x32,0x8f},

    //{0x33,0x22},  // // AEC_EXP_MIN
    //{0x34,0x8f},
};

#define GC_QVGA_USE_SUBSAMPLE          1   // the angle of view is not changed when use QVGA
const rt_uint8_t gc0328c_QVGA_320_240_talbe[][2] =
{
#if (GC_QVGA_USE_SUBSAMPLE == 0) // crop window mode
    {0xFE,0x00},
    {0x50,0x01},
    {0x51,0x00},
    {0x52,0x78},
    {0x53,0x00},
    {0x54,0xa0},

    {0x55,0x00},
    {0x56,0xf0},
    {0x57,0x01},
    {0x58,0x40},
#else
    // subsample mode
    {0xFE,0x00},
    {0x59,0x22},

    {0xFE,0x00},
    {0x50,0x01},
    {0x51,0x00},
    {0x52,0x00},
    {0x53,0x00},
    {0x54,0x00},
#endif

    {0x55,0x00},
    {0x56,0xf0},
    {0x57,0x01},
    {0x58,0x40},
};

const rt_uint8_t gc0328c_VGA_640_480_talbe[][2] =
{
#if (GC_QVGA_USE_SUBSAMPLE == 1)
    {0xFE,0x00},
    {0x59,0x11},
#endif

    {0xFE,0x00},
    {0x50,0x01},
    {0x51,0x00},
    {0x52,0x00},
    {0x53,0x00},
    {0x54,0x00},

    {0x55,0x01},
    {0x56,0xe0},
    {0x57,0x02},
    {0x58,0x80},
};

#ifdef __cplusplus
}
#endif

#endif
