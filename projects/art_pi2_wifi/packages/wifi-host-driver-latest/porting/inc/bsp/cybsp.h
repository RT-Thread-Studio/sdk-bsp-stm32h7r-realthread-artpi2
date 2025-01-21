/***************************************************************************//**
* \file cybsp.h
*
* \brief
* Basic API for setting up specific boards
*
********************************************************************************
* \copyright
* Copyright 2021 Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation
*
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#pragma once

#include "rtconfig.h"
#include "cybsp_types.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define WHD_USE_CUSTOM_MALLOC_IMPL


#ifdef CYBSP_HOST_WAKE_IRQ_EVENT_FALL
#define CYBSP_HOST_WAKE_IRQ_EVENT                       CYHAL_GPIO_IRQ_FALL
#endif

#ifdef CYBSP_HOST_WAKE_IRQ_EVENT_RISE
#define CYBSP_HOST_WAKE_IRQ_EVENT                       CYHAL_GPIO_IRQ_RISE
#endif

#ifdef CYBSP_HOST_WAKE_IRQ_EVENT_BOTH
#define CYBSP_HOST_WAKE_IRQ_EVENT                       CYHAL_GPIO_IRQ_BOTH
#endif


#define CYBSP_WIFI_INTERFACE_TYPE                       CYBSP_SDIO_INTERFACE
#define CYHAL_SDIO_MANUFACTURER                         0x02D0

#ifdef WHD_USING_CHIP_CYW4343W
#define CYHAL_SDIO_DRIVER_ID                            43430
#define CYHAL_SDIO_DRIVER_NAME                          "cyw4343W"
#endif

#ifdef WHD_USING_CHIP_CYW43438
#define CYHAL_SDIO_DRIVER_ID                            43430
#define CYHAL_SDIO_DRIVER_NAME                          "cyw43438"
#endif

#ifdef WHD_USING_CHIP_CYW4373
#define CYHAL_SDIO_DRIVER_ID                            0x4373
#define CYHAL_SDIO_DRIVER_NAME                          "cyw4373"
#endif

#ifdef WHD_USING_CHIP_CYW43012
#define CYHAL_SDIO_DRIVER_ID                            43012
#define CYHAL_SDIO_DRIVER_NAME                          "cyw43012"
#endif

#ifdef WHD_USING_CHIP_CYW43439
#define CYHAL_SDIO_DRIVER_ID                            43430
#define CYHAL_SDIO_DRIVER_NAME                          "cyw43439"
#endif

#ifdef WHD_USING_CHIP_CYW43022
#define CYHAL_SDIO_DRIVER_ID                            43022
#define CYHAL_SDIO_DRIVER_NAME                          "cyw43022"
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

