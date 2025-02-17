/**
  ******************************************************************************
  * File Name          : STM32TouchController.cpp
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* USER CODE BEGIN STM32TouchController */

#include <STM32TouchController.hpp>
#include <rtthread.h>

#if defined(PKG_USING_GT911) || defined(PKG_USING_GT9147)
#ifdef PKG_USING_GT9147
#include "gt9147.h"
#else
#include "gt911.h"
#endif
#include <lcd_port.h>

static rt_device_t dev = RT_NULL;
static struct rt_touch_data *read_data = NULL;
static struct rt_touch_info info;

int touch_init(uint16_t x, uint16_t y)
{
    void *id;
#ifdef PKG_USING_GT9147
    dev = rt_device_find("gt");
#else
    dev = rt_device_find("gt911");
#endif
    if (dev == RT_NULL)
    {
        rt_kprintf("can't find device gt\n");
        return -1;
    }

    if (rt_device_open(dev, RT_DEVICE_FLAG_INT_RX) != RT_EOK)
    {
        rt_kprintf("open device failed!");
        return -1;
    }

    id = rt_malloc(sizeof(rt_uint8_t) * 8);
    rt_device_control(dev, RT_TOUCH_CTRL_GET_ID, id);
    rt_uint8_t *read_id = (rt_uint8_t *)id;
    rt_kprintf("id = %c %c %c %c \n", read_id[0], read_id[1], read_id[2], read_id[3]);

    /* if possible you can set your x y coordinate*/
//    rt_device_control(dev, RT_TOUCH_CTRL_SET_X_RANGE, &x);
//    rt_device_control(dev, RT_TOUCH_CTRL_SET_Y_RANGE, &y);

    rt_device_control(dev, RT_TOUCH_CTRL_GET_INFO, &info);

    rt_kprintf("range_x = %d \n", info.range_x);
    rt_kprintf("range_y = %d \n", info.range_y);
    rt_kprintf("point_num = %d \n", info.point_num);

    read_data = (struct rt_touch_data *)rt_malloc(sizeof(struct rt_touch_data) * info.point_num);

    rt_free(id);

    return RT_EOK;
}
#endif

void STM32TouchController::init()
{
    /**
     * Initialize touch controller and driver
     *
     */
#if defined(PKG_USING_GT911) || defined(PKG_USING_GT9147)
    if (touch_init(LCD_WIDTH, LCD_HEIGHT) != RT_EOK)
        return;
#endif
}

bool STM32TouchController::sampleTouch(int32_t &x, int32_t &y)
{
    /**
     * By default sampleTouch returns false,
     * return true if a touch has been detected, otherwise false.
     *
     * Coordinates are passed to the caller by reference by x and y.
     *
     * This function is called by the TouchGFX framework.
     * By default sampleTouch is called every tick, this can be adjusted by HAL::setTouchSampleRate(int8_t);
     *
     */
#if defined(PKG_USING_GT911) || defined(PKG_USING_GT9147)
    rt_device_read(dev, 0, read_data, info.point_num);
    if (read_data[0].event == RT_TOUCH_EVENT_DOWN || read_data[0].event == RT_TOUCH_EVENT_MOVE)
    {
        x = LCD_WIDTH - read_data[0].x_coordinate;
        y = LCD_HEIGHT - read_data[0].y_coordinate;
        return true;
    }
    else
#endif
    {
        return false;
    }
}

/* USER CODE END STM32TouchController */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
