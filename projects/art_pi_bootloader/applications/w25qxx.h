/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-15     whj4674672   first version
 */

#ifndef __W25QXX_H
#define __W25QXX_H

#include "board.h"

#define MX66UM_MAXFreq 200000000u   // 200Mhz

void MX66UW_Init(void);
int32_t MX66UW_Memory_Mapped_Enable(void);
// void MX66UW_Memory_Mapped_Enable(void);

#endif
