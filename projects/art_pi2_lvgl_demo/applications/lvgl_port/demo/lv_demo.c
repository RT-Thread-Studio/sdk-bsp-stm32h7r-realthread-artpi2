/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2021-10-17     Meco Man      First version
 * 2022-05-10     Meco Man      improve rt-thread initialization process
 */

#include "rtconfig.h"
#include "lv_demos.h"

/*********************
 *      DEFINES
 *********************/
#define LV_DEMOS_COUNT (sizeof(demos_entry_info) / sizeof(demo_entry_info_t) - 1)

/**********************
 *      TYPEDEFS
 **********************/

typedef void (*demo_method_cb)(void);

typedef struct
{
    const char *name;
    demo_method_cb entry_cb;
} demo_entry_info_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static const demo_entry_info_t demos_entry_info[] =
{
#if LV_USE_DEMO_WIDGETS
    { "widgets", .entry_cb = lv_demo_widgets },
#endif

#if LV_USE_DEMO_MUSIC
    { "music", .entry_cb = lv_demo_music },
#endif

#if LV_USE_DEMO_STRESS
    { "stress", .entry_cb = lv_demo_stress },
#endif

#if LV_USE_DEMO_BENCHMARK
    { "benchmark", .entry_cb = lv_demo_benchmark },
#endif

    { "", .entry_cb = NULL }
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

bool lv_demos_create(const char *name)
{
    const int demos_count = LV_DEMOS_COUNT;

    if (demos_count <= 0)
    {
        LV_LOG_ERROR("Please enable some lv_demos firstly!");
        return false;
    }

    const demo_entry_info_t *entry_info = NULL;
    for (int i = 0; i < demos_count; i++)
    {
        if (lv_strcmp(name, demos_entry_info[i].name) == 0)
        {
            entry_info = &demos_entry_info[i];
        }
    }

    if (entry_info->entry_cb)
    {
        entry_info->entry_cb();
        return true;
    }

    return false;
}

void lv_demos_show_help(void)
{
    int i;
    const int demos_count = LV_DEMOS_COUNT;

    if (demos_count == 0)
    {
        LV_LOG("lv_demos: no demo available!\n");
        return;
    }

    LV_LOG("\nUsage: lv_demos demo [parameters]\n");
    LV_LOG("\ndemo list:\n");

    for (i = 0; i < demos_count; i++)
    {
        LV_LOG("     %s \n", demos_entry_info[i].name);
    }
}

void create_test_screen(lv_obj_t * parent)
{
    // 创建主容器
    lv_obj_t * container = lv_obj_create(parent);
    lv_obj_set_style_bg_color(container, lv_color_make(0x00, 0x00, 0x00), 0);  // 设置背景颜色为黑色
    lv_obj_set_size(container, 800, 480);
    lv_obj_center(container);

    // 创建颜色渐变测试区域
    // 1. 红色渐变到橙色
    lv_obj_t * red_grad = lv_obj_create(container);
    lv_obj_set_style_bg_color(red_grad, lv_color_make(0xFF, 0x00, 0x00), 0);  // 起始颜色：红色
    lv_obj_set_style_bg_grad_color(red_grad, lv_color_make(0xFF, 0xA5, 0x00), 0);  // 结束颜色：橙色
    lv_obj_set_style_bg_grad_dir(red_grad, LV_GRAD_DIR_HOR, 0);
    lv_obj_set_style_border_width(red_grad, 2, 0);
    lv_obj_set_pos(red_grad, 0, 0);
    lv_obj_set_size(red_grad, 200, 200);

    // 2. 绿色渐变到蓝色
    lv_obj_t * green_grad = lv_obj_create(container);
    lv_obj_set_style_bg_color(green_grad, lv_color_make(0x00, 0xFF, 0x00), 0);  // 起始颜色：绿色
    lv_obj_set_style_bg_grad_color(green_grad, lv_color_make(0x00, 0x00, 0xFF), 0);  // 结束颜色：蓝色
    lv_obj_set_style_bg_grad_dir(green_grad, LV_GRAD_DIR_HOR, 0);
    lv_obj_set_style_border_width(green_grad, 2, 0);
    lv_obj_set_pos(green_grad, 200, 0);
    lv_obj_set_size(green_grad, 200, 200);

    // 3. 蓝色渐变到紫色
    lv_obj_t * blue_grad = lv_obj_create(container);
    lv_obj_set_style_bg_color(blue_grad, lv_color_make(0x00, 0x00, 0xFF), 0);  // 起始颜色：蓝色
    lv_obj_set_style_bg_grad_color(blue_grad, lv_color_make(0x4B, 0x00, 0x82), 0);  // 结束颜色：紫色
    lv_obj_set_style_bg_grad_dir(blue_grad, LV_GRAD_DIR_HOR, 0);
    lv_obj_set_style_border_width(blue_grad, 2, 0);
    lv_obj_set_pos(blue_grad, 400, 0);
    lv_obj_set_size(blue_grad, 200, 200);

    // 4. 白色渐变到灰色
    lv_obj_t * white_grad = lv_obj_create(container);
    lv_obj_set_style_bg_color(white_grad, lv_color_make(0xFF, 0xFF, 0xFF), 0);  // 起始颜色：白色
    lv_obj_set_style_bg_grad_color(white_grad, lv_color_make(0x80, 0x80, 0x80), 0);  // 结束颜色：灰色
    lv_obj_set_style_bg_grad_dir(white_grad, LV_GRAD_DIR_HOR, 0);
    lv_obj_set_style_border_width(white_grad, 2, 0);
    lv_obj_set_pos(white_grad, 0, 200);
    lv_obj_set_size(white_grad, 200, 200);

    // 5. 黑色渐变到深灰色
    lv_obj_t * black_grad = lv_obj_create(container);
    lv_obj_set_style_bg_color(black_grad, lv_color_make(0x00, 0x00, 0x00), 0);  // 起始颜色：黑色
    lv_obj_set_style_bg_grad_color(black_grad, lv_color_make(0x40, 0x40, 0x40), 0);  // 结束颜色：深灰色
    lv_obj_set_style_bg_grad_dir(black_grad, LV_GRAD_DIR_HOR, 0);
    lv_obj_set_style_border_width(black_grad, 2, 0);
    lv_obj_set_pos(black_grad, 200, 200);
    lv_obj_set_size(black_grad, 200, 200);

    // 6. 黄色渐变到绿色
    lv_obj_t * yellow_grad = lv_obj_create(container);
    lv_obj_set_style_bg_color(yellow_grad, lv_color_make(0xFF, 0xFF, 0x00), 0);  // 起始颜色：黄色
    lv_obj_set_style_bg_grad_color(yellow_grad, lv_color_make(0x00, 0xFF, 0x00), 0);  // 结束颜色：绿色
    lv_obj_set_style_bg_grad_dir(yellow_grad, LV_GRAD_DIR_HOR, 0);
    lv_obj_set_style_border_width(yellow_grad, 2, 0);
    lv_obj_set_pos(yellow_grad, 400, 200);
    lv_obj_set_size(yellow_grad, 200, 200);

    // 创建全屏颜色渐变条
    lv_obj_t * grad_rect = lv_obj_create(container);
    lv_obj_set_size(grad_rect, 800, 80);
    lv_obj_set_pos(grad_rect, 0, 400);
    lv_obj_set_style_bg_color(grad_rect, lv_color_make(0x80, 0x80, 0x80), 0);  // 灰色背景
    lv_obj_set_style_border_width(grad_rect, 2, 0);

    // 创建渐变色块
    lv_obj_t * grad = lv_obj_create(grad_rect);
    lv_obj_set_style_bg_color(grad, lv_color_make(0xFF, 0x00, 0x00), 0);  // 起始颜色：红色
    lv_obj_set_style_bg_grad_color(grad, lv_color_make(0x00, 0x00, 0xFF), 0);  // 结束颜色：蓝色
    lv_obj_set_style_bg_grad_dir(grad, LV_GRAD_DIR_HOR, 0);
    lv_obj_set_size(grad, 800, 80);
    lv_obj_set_pos(grad, 0, 400);

    // 创建闪烁区域
    lv_obj_t * blink_rect = lv_obj_create(container);
    lv_obj_set_style_bg_color(blink_rect, lv_color_make(0xFF, 0x00, 0x00), 0);  // 红色
    lv_obj_set_style_border_width(blink_rect, 2, 0);
    lv_obj_set_pos(blink_rect, 0, 300);
    lv_obj_set_size(blink_rect, 800, 20);

    // 创建标签
    lv_obj_t * label_red = lv_label_create(red_grad);
    lv_label_set_text(label_red, "RED GRAD");
    lv_obj_center(label_red);

    lv_obj_t * label_green = lv_label_create(green_grad);
    lv_label_set_text(label_green, "GREEN GRAD");
    lv_obj_center(label_green);

    lv_obj_t * label_blue = lv_label_create(blue_grad);
    lv_label_set_text(label_blue, "BLUE GRAD");
    lv_obj_center(label_blue);

    lv_obj_t * label_white = lv_label_create(white_grad);
    lv_label_set_text(label_white, "WHITE GRAD");
    lv_obj_center(label_white);

    lv_obj_t * label_black = lv_label_create(black_grad);
    lv_label_set_text(label_black, "BLACK GRAD");
    lv_obj_center(label_black);

    lv_obj_t * label_yellow = lv_label_create(yellow_grad);
    lv_label_set_text(label_yellow, "YELLOW GRAD");
    lv_obj_center(label_yellow);

    lv_obj_t * label_grad = lv_label_create(grad_rect);
    lv_label_set_text(label_grad, "FULL GRADIENT");
    lv_obj_center(label_grad);

    lv_obj_t * label_blink = lv_label_create(blink_rect);
    lv_label_set_text(label_blink, "BLINK TEST");
    lv_obj_center(label_blink);
}

void lv_user_gui_init(void)
{
    /* display demo; you may replace with your LVGL application at here */
#if LV_USE_DEMO_MUSIC
//    lv_demos_create("music");
    create_test_screen(lv_scr_act());
#elif LV_USE_DEMO_BENCHMARK
    lv_demos_create("benchmark");
#elif LV_USE_DEMO_WIDGETS
    lv_demos_create("widgets");
#elif LV_USE_DEMO_STRESS
    lv_demos_create("stress");
#elif LV_USE_DEMO_RENDER
    lv_demo_render(LV_DEMO_RENDER_SCENE_IMAGE_NORMAL, 128);
#else
#error "Please enable one lvgl demo in the env"
#endif
    lv_demos_show_help();
}
