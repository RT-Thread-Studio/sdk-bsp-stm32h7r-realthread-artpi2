/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
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
    /* Configure the MMU to avoid any issue relative to speculative access */
    MPU_Region_InitTypeDef default_config =
    {
        .Enable = MPU_REGION_ENABLE,
        .Number = MPU_REGION_NUMBER0,
        .BaseAddress = 0,
        .Size = MPU_REGION_SIZE_4GB,
        .SubRegionDisable = 0xA7,
        .TypeExtField = MPU_TEX_LEVEL0,
        .AccessPermission = MPU_REGION_NO_ACCESS,
        .DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE,
        .IsShareable = MPU_ACCESS_SHAREABLE,
        .IsCacheable = MPU_ACCESS_NOT_CACHEABLE,
        .IsBufferable = MPU_ACCESS_NOT_BUFFERABLE
    };

    /* Disable the MPU */
    HAL_MPU_Disable();

    /* Set a global region to avoid any speculative access issue */
    HAL_MPU_ConfigRegion(&default_config);

    /* Enable the MPU */
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

    /* Enable CACHE */
    SCB_EnableICache();
    SCB_EnableDCache();

    return RT_EOK;

}
INIT_BOARD_EXPORT(mpu_init);
