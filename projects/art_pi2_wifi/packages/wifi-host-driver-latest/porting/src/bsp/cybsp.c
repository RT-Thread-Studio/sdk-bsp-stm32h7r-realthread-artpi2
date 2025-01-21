/*
 * MIT License
 *
 * Copyright (c) 2024 Evlers
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Change Logs:
 * Date         Author      Notes
 * 2023-12-21   Evlers      first implementation
 * 2024-05-17   Evlers      fixed an bug where a module could not be reset
 * 2024-05-28   Evlers      add support for pin names
 * 2024-06-05   Evlers      remove the unused header file drv gpio.h
 */

#include "rtthread.h"
#include "rtdevice.h"

#include "cybsp.h"


static int cybsp_init(void)
{
#ifndef CYBSP_USING_PIN_NAME
    rt_base_t pin_number = CYBSP_REG_ON_PIN;
#else
    rt_base_t pin_number = rt_pin_get(CYBSP_REG_ON_PIN_NAME);
#endif
    /* configure the wl_reg_on pin */
    rt_pin_mode(pin_number, PIN_MODE_OUTPUT);

    /* reset modules */
    rt_pin_write(pin_number, PIN_LOW);
    rt_thread_mdelay(2);
    rt_pin_write(pin_number, PIN_HIGH);

    return RT_EOK;
}
INIT_PREV_EXPORT(cybsp_init);

rt_weak void whd_wait_fs_mount (void)
{
    /* Implement a function that waits for the file system to mount.
     * Example: rt_sem_take(fs_mount_sem, RT_WAITING_FOREVER);
     */
}
