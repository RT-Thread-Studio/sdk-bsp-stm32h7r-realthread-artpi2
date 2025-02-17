/*
 * Copyright (c) 2006-2025, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-21     KK       the first version
 */
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "app_touchgfx.h"
#include <lcd_port.h>

CRC_HandleTypeDef hcrc;

DMA_HandleTypeDef handle_HPDMA1_Channel1;
DMA_HandleTypeDef handle_HPDMA1_Channel0;

LTDC_HandleTypeDef hltdc;
JPEG_HandleTypeDef hjpeg;
GPU2D_HandleTypeDef hgpu2d;
DMA2D_HandleTypeDef hdma2d;

static void MX_CRC_Init(void);
static void MX_DMA2D_Init(void);

#if defined(PKG_USING_GT911) || defined(PKG_USING_GT9147)
#ifdef PKG_USING_GT9147
    #include "gt9147.h"
#else
    #include "gt911.h"
#endif

#define TOUCH_RST_PIN   GET_PIN(N, 12) // PN12
#define TOUCH_IRQ_PIN   GET_PIN(E, 15) // PE15

int rt_hw_touch_port(void)
{
    struct rt_touch_config config;
    rt_uint8_t rst;
    rst = TOUCH_RST_PIN;
    config.dev_name = "i2c2";
    config.irq_pin.pin  = TOUCH_IRQ_PIN;
    config.irq_pin.mode = PIN_MODE_INPUT_PULLDOWN;
    config.user_data = &rst;
#ifdef PKG_USING_GT9147
    rt_hw_gt9147_init("gt", &config);
#else
    rt_hw_gt911_init("gt911", &config);
#endif
    return 0;
}
INIT_ENV_EXPORT(rt_hw_touch_port);
#endif

/**
  * @brief This function handles HPDMA1 Channel 0 global interrupt.
  */
void HPDMA1_Channel0_IRQHandler(void)
{
    /* USER CODE BEGIN HPDMA1_Channel0_IRQn 0 */
    rt_interrupt_enter();
    /* USER CODE END HPDMA1_Channel0_IRQn 0 */
    HAL_DMA_IRQHandler(&handle_HPDMA1_Channel0);
    /* USER CODE BEGIN HPDMA1_Channel0_IRQn 1 */
    rt_interrupt_leave();
    /* USER CODE END HPDMA1_Channel0_IRQn 1 */
}

/**
  * @brief This function handles HPDMA1 Channel 1 global interrupt.
  */
void HPDMA1_Channel1_IRQHandler(void)
{
    /* USER CODE BEGIN HPDMA1_Channel1_IRQn 0 */
    rt_interrupt_enter();
    /* USER CODE END HPDMA1_Channel1_IRQn 0 */
    HAL_DMA_IRQHandler(&handle_HPDMA1_Channel1);
    /* USER CODE BEGIN HPDMA1_Channel1_IRQn 1 */
    rt_interrupt_leave();
    /* USER CODE END HPDMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles LTDC global interrupt.
  */
void LTDC_IRQHandler(void)
{
    /* USER CODE BEGIN LTDC_IRQn 0 */
    rt_interrupt_enter();
    /* USER CODE END LTDC_IRQn 0 */
    HAL_LTDC_IRQHandler(&hltdc);
    /* USER CODE BEGIN LTDC_IRQn 1 */
    rt_interrupt_leave();
    /* USER CODE END LTDC_IRQn 1 */
}

/**
  * @brief This function handles DMA2D global interrupt.
  */
void DMA2D_IRQHandler(void)
{
    /* USER CODE BEGIN DMA2D_IRQn 0 */
    rt_interrupt_enter();
    /* USER CODE END DMA2D_IRQn 0 */
    HAL_DMA2D_IRQHandler(&hdma2d);
    /* USER CODE BEGIN DMA2D_IRQn 1 */
    rt_interrupt_leave();
    /* USER CODE END DMA2D_IRQn 1 */
}

/**
  * @brief This function handles JPEG global interrupt.
  */
void JPEG_IRQHandler(void)
{
    /* USER CODE BEGIN JPEG_IRQn 0 */
    rt_interrupt_enter();
    /* USER CODE END JPEG_IRQn 0 */
    HAL_JPEG_IRQHandler(&hjpeg);
    /* USER CODE BEGIN JPEG_IRQn 1 */
    rt_interrupt_leave();
    /* USER CODE END JPEG_IRQn 1 */
}

/**
  * @brief This function handles GPU2D global interrupt.
  */
void GPU2D_IRQHandler(void)
{
    /* USER CODE BEGIN GPU2D_IRQn 0 */
    rt_interrupt_enter();
    /* USER CODE END GPU2D_IRQn 0 */
    HAL_GPU2D_IRQHandler(&hgpu2d);
    /* USER CODE BEGIN GPU2D_IRQn 1 */
    rt_interrupt_leave();
    /* USER CODE END GPU2D_IRQn 1 */
}

/**
  * @brief This function handles GPU2D Error interrupt.
  */
void GPU2D_ER_IRQHandler(void)
{
    /* USER CODE BEGIN GPU2D_ER_IRQn 0 */
    rt_interrupt_enter();
    /* USER CODE END GPU2D_ER_IRQn 0 */
    HAL_GPU2D_ER_IRQHandler(&hgpu2d);
    /* USER CODE BEGIN GPU2D_ER_IRQn 1 */
    rt_interrupt_leave();
    /* USER CODE END GPU2D_ER_IRQn 1 */
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{
    /* USER CODE BEGIN CRC_Init 0 */

    /* USER CODE END CRC_Init 0 */

    /* USER CODE BEGIN CRC_Init 1 */

    /* USER CODE END CRC_Init 1 */
    hcrc.Instance = CRC;
    hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
    hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
    hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
    hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
    hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
    if (HAL_CRC_Init(&hcrc) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN CRC_Init 2 */

    /* USER CODE END CRC_Init 2 */
}

/**
  * @brief DMA2D Initialization Function
  * @param None
  * @retval None
  */
static void MX_DMA2D_Init(void)
{
    /* USER CODE BEGIN DMA2D_Init 0 */

    /* USER CODE END DMA2D_Init 0 */

    /* USER CODE BEGIN DMA2D_Init 1 */

    /* USER CODE END DMA2D_Init 1 */
    hdma2d.Instance = DMA2D;
    hdma2d.Init.Mode = DMA2D_R2M;
    hdma2d.Init.ColorMode = DMA2D_OUTPUT_RGB888;
    hdma2d.Init.OutputOffset = 0;
    if (HAL_DMA2D_Init(&hdma2d) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN DMA2D_Init 2 */

    /* USER CODE END DMA2D_Init 2 */
}

/**
  * @brief GPU2D Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPU2D_Init(void)
{

    /* USER CODE BEGIN GPU2D_Init 0 */

    /* USER CODE END GPU2D_Init 0 */

    /* USER CODE BEGIN GPU2D_Init 1 */

    /* USER CODE END GPU2D_Init 1 */
    hgpu2d.Instance = GPU2D;
    if (HAL_GPU2D_Init(&hgpu2d) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN GPU2D_Init 2 */

    /* USER CODE END GPU2D_Init 2 */

}

/**
  * @brief HPDMA1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_HPDMA1_Init(void)
{

    /* USER CODE BEGIN HPDMA1_Init 0 */

    /* USER CODE END HPDMA1_Init 0 */

    /* Peripheral clock enable */
    __HAL_RCC_HPDMA1_CLK_ENABLE();

    /* HPDMA1 interrupt Init */
    HAL_NVIC_SetPriority(HPDMA1_Channel0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(HPDMA1_Channel0_IRQn);
    HAL_NVIC_SetPriority(HPDMA1_Channel1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(HPDMA1_Channel1_IRQn);

    /* USER CODE BEGIN HPDMA1_Init 1 */

    /* USER CODE END HPDMA1_Init 1 */
    /* USER CODE BEGIN HPDMA1_Init 2 */

    /* USER CODE END HPDMA1_Init 2 */

}

/**
  * @brief ICACHE_GPU2D Initialization Function
  * @param None
  * @retval None
  */
static void MX_ICACHE_GPU2D_Init(void)
{

    /* USER CODE BEGIN ICACHE_GPU2D_Init 0 */

    /* USER CODE END ICACHE_GPU2D_Init 0 */

    /* USER CODE BEGIN ICACHE_GPU2D_Init 1 */

    /* USER CODE END ICACHE_GPU2D_Init 1 */

    /** Enable instruction cache (default 2-ways set associative cache)
    */
    if (HAL_ICACHE_Enable() != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN ICACHE_GPU2D_Init 2 */

    /* USER CODE END ICACHE_GPU2D_Init 2 */

}

/**
  * @brief JPEG Initialization Function
  * @param None
  * @retval None
  */
static void MX_JPEG_Init(void)
{

    /* USER CODE BEGIN JPEG_Init 0 */

    /* USER CODE END JPEG_Init 0 */

    /* USER CODE BEGIN JPEG_Init 1 */

    /* USER CODE END JPEG_Init 1 */
    hjpeg.Instance = JPEG;
    if (HAL_JPEG_Init(&hjpeg) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN JPEG_Init 2 */

    /* USER CODE END JPEG_Init 2 */

}

/**
  * @brief LTDC Initialization Function
  * @param None
  * @retval None
  */
static void MX_LTDC_Init(void)
{

    /* USER CODE BEGIN LTDC_Init 0 */

    /* USER CODE END LTDC_Init 0 */

    LTDC_LayerCfgTypeDef pLayerCfg = {0};

    /* USER CODE BEGIN LTDC_Init 1 */

    /* USER CODE END LTDC_Init 1 */
    hltdc.Instance = LTDC;
    hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
    hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
    hltdc.Init.HorizontalSync = 47;
    hltdc.Init.VerticalSync = 2;
    hltdc.Init.AccumulatedHBP = 135;
    hltdc.Init.AccumulatedVBP = 34;
    hltdc.Init.AccumulatedActiveW = 935;
    hltdc.Init.AccumulatedActiveH = 514;
    hltdc.Init.TotalWidth = 975;
    hltdc.Init.TotalHeigh = 527;
    hltdc.Init.Backcolor.Blue = 0;
    hltdc.Init.Backcolor.Green = 0;
    hltdc.Init.Backcolor.Red = 0;
    if (HAL_LTDC_Init(&hltdc) != HAL_OK)
    {
        Error_Handler();
    }
    pLayerCfg.WindowX0 = 0;
    pLayerCfg.WindowX1 = 800;
    pLayerCfg.WindowY0 = 0;
    pLayerCfg.WindowY1 = 480;
    pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
    pLayerCfg.Alpha = 255;
    pLayerCfg.Alpha0 = 0;
    pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
    pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
    pLayerCfg.FBStartAdress = 0;
    pLayerCfg.ImageWidth = 800;
    pLayerCfg.ImageHeight = 480;
    pLayerCfg.Backcolor.Blue = 0;
    pLayerCfg.Backcolor.Green = 0;
    pLayerCfg.Backcolor.Red = 0;
    if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN LTDC_Init 2 */
    // Reconfigure pixelformat since TouchGFX project generator does not allow setting different format for LTDC and remaining configuration
    // This way TouchGFX runs 32BPP mode but the LTDC accesses the real framebuffer in 24BPP
//    pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB888;
//    if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
//    {
//        Error_Handler();
//    }
    /* USER CODE END LTDC_Init 2 */
}

static void En_LCDBlacklight(void)
{
    rt_pin_mode(LCD_BL_GPIO_NUM, PIN_MODE_OUTPUT);
    rt_pin_write(LCD_BL_GPIO_NUM, PIN_HIGH);
}

void touchgfx_thread_entry(void *parameter)
{
    MX_HPDMA1_Init();
    MX_LTDC_Init();
    MX_CRC_Init();
    MX_DMA2D_Init();
    MX_JPEG_Init();
    MX_GPU2D_Init();
    MX_ICACHE_GPU2D_Init();

    En_LCDBlacklight();

    MX_TouchGFX_Init();
    MX_TouchGFX_Process();
}

int TouchGFXTask(void)
{
    rt_thread_t tid = NULL;
    tid = rt_thread_create("TouchGFX",
                           touchgfx_thread_entry, RT_NULL,
                           4096, 22, 20);

    if (tid != RT_NULL)
        rt_thread_startup(tid);
    else
        return -1;

    return RT_EOK;
}
INIT_APP_EXPORT(TouchGFXTask);
