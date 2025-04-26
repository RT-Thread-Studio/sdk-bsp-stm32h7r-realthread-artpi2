/*
 * Copyright (c) 2006-2025, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-02     RT-Thread    first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#include "usbh_core.h"

int main(void)
{

	usbh_initialize(0, USB_OTG_HS_PERIPH_BASE);

    while(1)
    {
        rt_thread_mdelay(500);
    }
    return RT_EOK;
}

#include "stm32h7rsxx.h"
static int vtor_config(void)
{
    /* Vector Table Relocation in Internal XSPI2_BASE */
    SCB->VTOR = XSPI2_BASE;
    return 0;
}
INIT_BOARD_EXPORT(vtor_config);

