/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-02-01     Rudy Lo      The first version
 * 2022-06-20     Rbb666       Add SPI and RGB LCD Config
 */

#include <lvgl.h>

//#define DRV_DEBUG
#define LOG_TAG             "lvgl.disp"
#include <drv_log.h>

#ifdef BSP_USING_ILI9488
    #include "drv_spi_ili9488.h"
    #define COLOR_BUFFER  (LV_HOR_RES_MAX * LV_VER_RES_MAX / 5)
#else
    #include <lcd_port.h>
    #define COLOR_BUFFER  (LV_HOR_RES_MAX * LV_VER_RES_MAX * 2)
#endif

static lv_display_t *disp;

#ifdef BSP_USING_LCD_RGB
static DMA2D_HandleTypeDef hdma2d;
extern LTDC_HandleTypeDef LtdcHandle;
static rt_sem_t trans_done_semphr = RT_NULL;

static void mDMA2Dcallvack(DMA2D_HandleTypeDef *hdma2d)
{
    lv_display_flush_ready(disp);
}

static void lvgl_dma2d_config(void)
{
    hdma2d.Instance = DMA2D;
    hdma2d.Init.Mode = DMA2D_M2M;
    hdma2d.Init.ColorMode = DMA2D_OUTPUT_RGB565;
    hdma2d.Init.OutputOffset = 0;
    hdma2d.Init.BytesSwap = DMA2D_BYTES_REGULAR;
    hdma2d.Init.LineOffsetMode = DMA2D_LOM_PIXELS;
    hdma2d.LayerCfg[1].InputOffset = 0;
    hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
    hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
    hdma2d.LayerCfg[1].InputAlpha = 0;
    hdma2d.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA;
    hdma2d.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR;
    if (HAL_DMA2D_Init(&hdma2d) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) != HAL_OK)
    {
        Error_Handler();
    }
}

void DMA2D_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMA2D_IRQHandler(&hdma2d);

    /* exit interrupt */
    rt_interrupt_leave();
}
#endif

static void lcd_fb_flush(lv_display_t  *disp_drv, const lv_area_t *area, uint8_t *color_p)
{
#ifdef BSP_USING_ILI9488
    /* color_p is a buffer pointer; the buffer is provided by LVGL */
    lcd_fill_array(area->x1, area->y1, area->x2, area->y2, color_p);
    lv_disp_flush_ready(disp_drv);
#else
    lv_coord_t width = lv_area_get_width(area);
    lv_coord_t height = lv_area_get_height(area);

    DMA2D->CR = 0x0U << DMA2D_CR_MODE_Pos;
    DMA2D->FGPFCCR = DMA2D_INPUT_RGB565;
    DMA2D->FGMAR = (uint32_t)color_p;
    DMA2D->FGOR = 0;
    DMA2D->OPFCCR = DMA2D_OUTPUT_RGB565;
    DMA2D->OMAR = LtdcHandle.LayerCfg[0].FBStartAdress + 2 * \
                  (area->y1 * LV_HOR_RES_MAX + area->x1);
    DMA2D->OOR = LV_HOR_RES_MAX - width;
    DMA2D->NLR = (width << DMA2D_NLR_PL_Pos) | (height << DMA2D_NLR_NL_Pos);
    DMA2D->IFCR = 0x3FU;
    DMA2D->CR |= DMA2D_CR_TCIE;
    DMA2D->CR |= DMA2D_CR_START;
#endif
}

void lv_port_disp_init(void)
{
    extern struct rt_memheap system_heap;
    uint8_t *lv_disp_buf;
    lv_disp_buf = rt_memheap_alloc(&system_heap, COLOR_BUFFER);
    if (lv_disp_buf != RT_NULL)
        rt_kprintf("lv_disp_buf = %p\n", lv_disp_buf);
    else
        rt_kprintf("malloc failed\n");

#ifdef BSP_USING_LCD_RGB
    rt_err_t result;

    lvgl_dma2d_config();

    rt_device_t lcd_device = rt_device_find("lcd");
    result = rt_device_open(lcd_device, 0);
    if (result != RT_EOK)
    {
        LOG_E("error!");
        return;
    }

    static struct rt_device_graphic_info info;
    /* get framebuffer address */
    result = rt_device_control(lcd_device, RTGRAPHIC_CTRL_GET_INFO, &info);
    if (result != RT_EOK)
    {
        LOG_E("error!");
        return;
    }

    disp = lv_display_create(LV_HOR_RES_MAX, LV_VER_RES_MAX);
    lv_display_set_buffers(disp, lv_disp_buf, NULL, COLOR_BUFFER, LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(disp, lcd_fb_flush);

    /* interrupt callback for DMA2D transfer */
    hdma2d.XferCpltCallback = mDMA2Dcallvack;
#endif
}
