/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-02-01     Rudy Lo      The first version
 * 2022-06-20     Rbb666       Add GT9147 Touch Device
 */

#include <lvgl.h>
#include <stdbool.h>
#include <rtdevice.h>
#include <drv_gpio.h>
#include <lcd_port.h>

static rt_device_t ts_dev; /* Touch device handle, Touchscreen */
static struct rt_touch_data *read_data;
static struct rt_touch_info info;

#include "gt9147.h"
#define GT9147_RST_PIN   GET_PIN(N, 12) //PN12
#define GT9147_IRQ_PIN   GET_PIN(E, 15) //PE15

static rt_int16_t last_x = 0;
static rt_int16_t last_y = 0;
static lv_indev_state_t last_state = LV_INDEV_STATE_REL;

static bool touchpad_is_pressed(void)
{
    rt_device_control(ts_dev, RT_TOUCH_CTRL_GET_INFO, &info);

    if (1 == rt_device_read(ts_dev, 0, read_data, 1))
    {
        if (read_data->event == RT_TOUCH_EVENT_DOWN)
        {
            /* swap x and y */
            rt_int16_t tmp_x = read_data->y_coordinate;
            rt_int16_t tmp_y = read_data->x_coordinate;

            /* invert y */
//            tmp_y = 320 - tmp_y;

            /* restore data */
            last_x = tmp_x;
            last_y = tmp_y;

            rt_kprintf("touch: x = %d, y = %d\n", last_x, last_y);
            return true;
        }
    }
    return false;
}

static void touchpad_get_xy(rt_int32_t *x, rt_int32_t *y)
{
    *x = last_x;
    *y = last_y;
}

void lv_port_indev_input(rt_int32_t x, rt_int32_t y, lv_indev_state_t state)
{
    last_state = state;
    last_x = x;
    last_y = y;
}

static void touchpad_read(lv_indev_t *indev, lv_indev_data_t *data)
{
    /*`touchpad_is_pressed` and `touchpad_get_xy` needs to be implemented by you*/
    if (touchpad_is_pressed())
    {
        data->state = LV_INDEV_STATE_PRESSED;
        touchpad_get_xy(&data->point.x, &data->point.y);
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

rt_err_t rt_hw_gt9147_register(void)
{
    void *id;
    struct rt_touch_config config;
    rt_uint8_t rst;
    rst = GT9147_RST_PIN;
    config.dev_name = "i2c2";
    config.irq_pin.pin = GT9147_IRQ_PIN;
    config.irq_pin.mode = PIN_MODE_INPUT_PULLUP;
    config.user_data = &rst;

    if (rt_hw_gt9147_init("gt", &config) != RT_EOK)
    {
        rt_kprintf("touch device gt9147 init failed.\n");
        return -RT_ERROR;
    }
    
    ts_dev = rt_device_find("gt");
    if (ts_dev == RT_NULL)
    {
        rt_kprintf("can't find device:%s\n", "gt");
        return -RT_ERROR;
    }

    if (rt_device_open(ts_dev, RT_DEVICE_FLAG_INT_RX) != RT_EOK)
    {
        rt_kprintf("open device failed!");
        return -RT_ERROR;
    }

    id = rt_malloc(sizeof(rt_uint8_t) * 8);
    rt_device_control(ts_dev, RT_TOUCH_CTRL_GET_ID, id);
    rt_uint8_t * read_id = (rt_uint8_t *) id;
    rt_kprintf("id = %d %d %d %d \n", read_id[0] - '0', read_id[1] - '0', read_id[2] - '0', read_id[3] - '0');

    int x = LCD_WIDTH;
    int y = LCD_HEIGHT;
    rt_device_control(ts_dev, RT_TOUCH_CTRL_SET_X_RANGE, &x); /* if possible you can set your x y coordinate*/
    rt_device_control(ts_dev, RT_TOUCH_CTRL_SET_Y_RANGE, &y);
    rt_device_control(ts_dev, RT_TOUCH_CTRL_GET_INFO, id);
    rt_kprintf("range_x = %d \n", (*(struct rt_touch_info*) id).range_x);
    rt_kprintf("range_y = %d \n", (*(struct rt_touch_info*) id).range_y);
    rt_kprintf("point_num = %d \n", (*(struct rt_touch_info*) id).point_num);
    rt_free(id);
    
    return RT_EOK;
}

lv_indev_t * touch_indev;

void lv_port_indev_init(void)
{   
    lv_indev_t *indev_touchpad;
    /* Register touch device */
    rt_hw_gt9147_register();
    /*Register a touchpad input device*/
    indev_touchpad = lv_indev_create();
    lv_indev_set_type(indev_touchpad, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_touchpad, touchpad_read);
}
