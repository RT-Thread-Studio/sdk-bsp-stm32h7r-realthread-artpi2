/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-15     RT-Thread    first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <drv_common.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

/* defined the LED0 pin: PB1 */
#define LED_RED    GET_PIN(O, 1)
#define LED_BLUE    GET_PIN(O, 5)

int main(void)
{
    int retr=0;
    /* set LED0 pin mode to output */
    rt_pin_mode(LED_RED, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_BLUE, PIN_MODE_OUTPUT);
    rt_pin_write(LED_BLUE, PIN_HIGH);
//    SCB_DisableICache();
//    SCB_DisableDCache();

    while (1)
    {
        rt_pin_write(LED_RED, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED_RED, PIN_LOW);
        rt_thread_mdelay(500);
    }

    return RT_EOK;
}
