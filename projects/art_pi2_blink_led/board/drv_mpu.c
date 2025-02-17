/*
 * Copyright (c) 2006-2025, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-04-14     whj4674672   first version
 */
#include <rtthread.h>
#include "stm32h7rsxx.h"
#include "board.h"

int mpu_init(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct;

    /* Disable the MPU */
    HAL_MPU_Disable();

    /* Configure the MPU attributes as WT for AXI SRAM */
    MPU_InitStruct.Enable            = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress       = 0x24000000;
    MPU_InitStruct.Size              = MPU_REGION_SIZE_512KB;
    MPU_InitStruct.AccessPermission  = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable      = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable       = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable       = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number            = MPU_REGION_NUMBER0;
    MPU_InitStruct.TypeExtField      = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable  = 0X00;
    MPU_InitStruct.DisableExec       = MPU_INSTRUCTION_ACCESS_ENABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);

#ifdef BSP_USING_PSRAM
    /* Configure the MPU attributes as WT for SDRAM */
    MPU_InitStruct.Enable            = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress       = 0xC0000000;
    MPU_InitStruct.Size              = MPU_REGION_SIZE_32MB;
    MPU_InitStruct.AccessPermission  = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable      = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable       = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable       = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number            = MPU_REGION_NUMBER1;
    MPU_InitStruct.TypeExtField      = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable  = 0x00;
    MPU_InitStruct.DisableExec       = MPU_INSTRUCTION_ACCESS_ENABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);
#endif

#ifdef BSP_USING_ETH
    /* Configure the MPU attributes as Device not cacheable
       for ETH DMA descriptors */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x30040000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_256B;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER2;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Configure the MPU attributes as Cacheable write through
       for LwIP RAM heap which contains the Tx buffers */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x30044000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_16KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER3;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);
#endif

    /* Create a region associated with memory address 0x70000000 HSPI2 OCTAL FLASH*/
    /* TEX=1, Normal memory type, code execution allowed */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER1;
    MPU_InitStruct.BaseAddress = 0x70000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_64MB;
    MPU_InitStruct.SubRegionDisable = 0;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Configure the MPU attributes as WT for HSPI1 PSRAM */
    MPU_InitStruct.Enable            = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress       = 0x90000000;
    MPU_InitStruct.Size              = MPU_REGION_SIZE_32MB;
    MPU_InitStruct.AccessPermission  = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable      = MPU_ACCESS_BUFFERABLE;
    MPU_InitStruct.IsCacheable       = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable       = MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.Number            = MPU_REGION_NUMBER4;
    MPU_InitStruct.TypeExtField      = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable  = 0X00;
    MPU_InitStruct.DisableExec       = MPU_INSTRUCTION_ACCESS_ENABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Enable the MPU */
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

    /* Enable CACHE */
    SCB_EnableICache();
    SCB_EnableDCache();

    return RT_EOK;

}
//INIT_BOARD_EXPORT(mpu_init);
