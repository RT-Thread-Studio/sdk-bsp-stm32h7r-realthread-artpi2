/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2022-01-28     Rudy Lo       The first version
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <rtconfig.h>

#define LV_USE_SYSMON           1
#define LV_USE_PERF_MONITOR     1
#define LV_USE_MEM_MONITOR      0

#ifdef BSP_USING_ILI9488
    #define LV_COLOR_16_SWAP         0
    #define LV_COLOR_DEPTH           32
    #define LV_HOR_RES_MAX           480
    #define LV_VER_RES_MAX           320
#elif defined(BSP_USING_OLED_454_MIPI)
    #define LV_COLOR_DEPTH           16
    #define LV_USE_DRAW_DMA2D        1
    #define LV_DRAW_DMA2D_HAL_INCLUDE "stm32h7rsxx.h"
    #define LV_HOR_RES_MAX           454
    #define LV_VER_RES_MAX           454
#elif defined(BSP_USING_LCD_800_RGB)
    #define LV_COLOR_DEPTH           16
    #define LV_USE_ST_LTDC           1
    #define LV_ST_LTDC_USE_DMA2D_FLUSH           1
    #define LV_DRAW_DMA2D_HAL_INCLUDE "stm32h7rsxx.h"
    #define LV_USE_NEMA_GFX          0
    #define LV_HOR_RES_MAX           800
    #define LV_VER_RES_MAX           480
#endif

#ifdef BSP_USING_LVGL_WIDGETS_DEMO
    #define LV_USE_DEMO_WIDGETS 1
    #define LV_DEMO_WIDGETS_SLIDESHOW   0
#endif  /* BSP_USING_LVGL_WIDGETS_DEMO */

/*Benchmark your system*/
#ifdef BSP_USING_LVGL_BENCHMARK_DEMO
    #define LV_USE_DEMO_BENCHMARK 1
    /*Use RGB565A8 images with 16 bit color depth instead of ARGB8565*/
    #define LV_DEMO_BENCHMARK_RGB565A8  1
    #define LV_FONT_MONTSERRAT_14       1
    #define LV_FONT_MONTSERRAT_20       1
    #define LV_FONT_MONTSERRAT_24       1
    #define LV_FONT_MONTSERRAT_26       1
#endif  /* BSP_USING_LVGL_BENCHMARK_DEMO */

/*Stress test for LVGL*/
#ifdef BSP_USING_LVGL_STRESS_DEMO
    #define LV_USE_DEMO_STRESS 1
#endif  /* BSP_USING_LVGL_STRESS_DEMO */

/*Render test for LVGL*/
#ifdef BSP_USING_LVGL_RENDER_DEMO
    #define LV_USE_DEMO_RENDER 1
#endif  /* BSP_USING_LVGL_RENDER_DEMO */

/*Music player demo*/
#ifdef BSP_USING_LVGL_MUSIC_DEMO
    #define LV_USE_DEMO_MUSIC 1
    #define LV_DEMO_MUSIC_SQUARE    0
    #define LV_DEMO_MUSIC_LANDSCAPE 1
    #define LV_DEMO_MUSIC_ROUND     0
    #define LV_DEMO_MUSIC_LARGE     0
    #define LV_DEMO_MUSIC_AUTO_PLAY 1
    #define LV_FONT_MONTSERRAT_12   1
    #define LV_FONT_MONTSERRAT_16   1
    #define LV_FONT_MONTSERRAT_22   1
    #define LV_FONT_MONTSERRAT_32   1
#endif  /* BSP_USING_LVGL_MUSIC_DEMO */

#endif
