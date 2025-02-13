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

#define TOUCH_RST_PIN   GET_PIN(N, 12) // PN12
#define TOUCH_IRQ_PIN   GET_PIN(E, 15) // PE15

#if defined(PKG_USING_GT911) || defined(PKG_USING_GT9147)
#ifdef PKG_USING_GT9147
    #include "gt9147.h"
#else
    #include "gt911.h"
#endif
#endif

static rt_device_t ts_dev; /* Touch device handle, Touchscreen */
static struct rt_touch_data *read_data;
static struct rt_touch_info info;

static rt_int16_t last_x = 0;
static rt_int16_t last_y = 0;

static bool touchpad_is_pressed(void)
{
    rt_device_read(ts_dev, 0, read_data, info.point_num);

    if (read_data[0].event == RT_TOUCH_EVENT_DOWN || read_data[0].event == RT_TOUCH_EVENT_MOVE)
    {
        last_x = LCD_WIDTH - read_data[0].x_coordinate;
        last_y = LCD_HEIGHT - read_data[0].y_coordinate;
        return true;
    }
    else
        return false;
}

static void touchpad_get_xy(rt_int32_t *x, rt_int32_t *y)
{
    *x = last_x;
    *y = last_y;
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

int touch_init(uint16_t x, uint16_t y)
{
    void *id;
#ifdef PKG_USING_GT9147
    ts_dev = rt_device_find("gt");
#else
    dev = rt_device_find("gt911");
#endif
    if (ts_dev == RT_NULL)
    {
        rt_kprintf("can't find device gt\n");
        return -1;
    }

    if (rt_device_open(ts_dev, RT_DEVICE_FLAG_INT_RX) != RT_EOK)
    {
        rt_kprintf("open device failed!");
        return -1;
    }

    id = rt_malloc(sizeof(rt_uint8_t) * 8);
    rt_device_control(ts_dev, RT_TOUCH_CTRL_GET_ID, id);
    rt_uint8_t *read_id = (rt_uint8_t *)id;
    rt_kprintf("id = %c %c %c %c \n", read_id[0], read_id[1], read_id[2], read_id[3]);

    /* if possible you can set your x y coordinate*/
//    rt_device_control(dev, RT_TOUCH_CTRL_SET_X_RANGE, &x);
//    rt_device_control(dev, RT_TOUCH_CTRL_SET_Y_RANGE, &y);

    rt_device_control(ts_dev, RT_TOUCH_CTRL_GET_INFO, &info);

    rt_kprintf("range_x = %d \n", info.range_x);
    rt_kprintf("range_y = %d \n", info.range_y);
    rt_kprintf("point_num = %d \n", info.point_num);

    read_data = (struct rt_touch_data *)rt_malloc(sizeof(struct rt_touch_data) * info.point_num);

    rt_free(id);

    return RT_EOK;
}

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

void lv_port_indev_init(void)
{
    lv_indev_t *indev_touchpad;
    /* Register touch device */
    touch_init(LCD_WIDTH, LCD_HEIGHT);
    /*Register a touchpad input device*/
    indev_touchpad = lv_indev_create();
    lv_indev_set_type(indev_touchpad, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_touchpad, touchpad_read);
}
