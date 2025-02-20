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

#include <lcd_port.h>
#define COLOR_BUFFER  (LV_HOR_RES_MAX * LV_VER_RES_MAX * 2)

static lv_display_t *disp;

DMA2D_HandleTypeDef hdma2d;
extern LTDC_HandleTypeDef hltdc;

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
    hdma2d.LayerCfg[1].ChromaSubSampling = DMA2D_NO_CSS;
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

static void lcd_fb_flush(lv_display_t  *disp_drv, const lv_area_t *area, uint8_t *color_p)
{
    lv_coord_t width = lv_area_get_width(area);
    lv_coord_t height = lv_area_get_height(area);

    SCB_CleanInvalidateDCache();

    DMA2D->CR = 0x0U << DMA2D_CR_MODE_Pos;
    DMA2D->FGPFCCR = DMA2D_INPUT_RGB565;
    DMA2D->FGMAR = (uint32_t)color_p;
    DMA2D->FGOR = 0;
    DMA2D->OPFCCR = DMA2D_OUTPUT_RGB565;
    DMA2D->OMAR = hltdc.LayerCfg[0].FBStartAdress + 2 * \
                  (area->y1 * LV_HOR_RES_MAX + area->x1);
    DMA2D->OOR = LV_HOR_RES_MAX - width;
    DMA2D->NLR = (width << DMA2D_NLR_PL_Pos) | (height << DMA2D_NLR_NL_Pos);
    DMA2D->IFCR = 0x3FU;
    DMA2D->CR |= DMA2D_CR_TCIE;
    DMA2D->CR |= DMA2D_CR_START;
}

void lv_port_disp_init(void)
{
    extern struct rt_memheap system_heap;
    uint8_t *lv_disp_buf;
    lv_disp_buf = rt_memheap_alloc(&system_heap, COLOR_BUFFER);
    if (lv_disp_buf != RT_NULL)
        rt_kprintf("lv_disp_buf = %p\n", lv_disp_buf);
    else
        LOG_E("malloc failed\n");

    lvgl_dma2d_config();

    struct drv_lcd_device *lcd;
    lcd = (struct drv_lcd_device *)rt_device_find("lcd");
    if (lcd == RT_NULL)
    {
        LOG_E("lcd device open error!");
        return;
    }

#if defined (LV_COLOR_DEPTH) && (LV_COLOR_DEPTH == 1U)
#if LV_COLOR_DEPTH == 16
    static __attribute__((aligned(32))) uint8_t buf_2[LV_HOR_RES_MAX * LV_VER_RES_MAX * 2];
    lv_st_ltdc_create_direct((void *)lcd->lcd_info.framebuffer, lv_disp_buf, LV_DISPLAY_RENDER_MODE_PARTIAL);
#elif LV_COLOR_DEPTH == 24 || LV_COLOR_DEPTH == 32
    static __attribute__((aligned(32))) uint8_t buf_1[MY_DISP_HOR_RES * MY_DISP_VER_RES];
    static __attribute__((aligned(32))) uint8_t buf_2[MY_DISP_HOR_RES * MY_DISP_VER_RES];
    lv_st_ltdc_create_partial(buf_1, buf_2, sizeof(buf_1), 0);
#else
#error LV_COLOR_DEPTH not supported
#endif
#else
    disp = lv_display_create(LV_HOR_RES_MAX, LV_VER_RES_MAX);
    lv_display_set_buffers(disp, (void *)lcd->lcd_info.framebuffer, lv_disp_buf, COLOR_BUFFER, LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(disp, lcd_fb_flush);

    /* interrupt callback for DMA2D transfer */
    hdma2d.XferCpltCallback = mDMA2Dcallvack;
#endif  /* LV_COLOR_DEPTH == 1U */
}
