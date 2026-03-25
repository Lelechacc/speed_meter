#ifndef __UI_H__
#define __UI_H__

#include "lvgl.h"
#include "ui_conf.h"
#include "stdio.h"
#include "ui_general_tab.h"
#include "ui_setting_tab.h"
#include "ui_gpx_tab.h"
#include "ui_translation.h"

#define UI_COLOR_STATUS_BAR    lv_color_hex(0xFEC5BB)  // 状态栏
#define UI_COLOR_BG            lv_color_hex(0xF8EDEB)  // 背景
#define UI_COLOR_FOCUS         lv_color_hex(0xD8E2DC)  // 选中高亮
#define UI_COLOR_TEXT          lv_color_hex(0x6D6875)  // 文字颜色
// 表示状态栏中间的运动状态

static const char *run_status_text[] =
    {
        "BIN_Stopped",
        "BIN_Running",
        "BIN_Waiting",
};
/**
 * @brief 创建ui界面
 * 
 */
void ui_create(void);

/*******************************************************************************
 * @brief 更新电池状态
 * 
 * @param ui  ui结构体指针
 * @param isCharging  是否充电
 * @param percent  百分比
 * */
void ui_update_battery(ui_t *ui, bool isCharging, uint8_t percent);
#endif /* __UI_H__ */