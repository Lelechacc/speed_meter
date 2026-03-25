#ifndef __UI_CONF_H__
#define __UI_CONF_H__
#include "lvgl.h"
#define LV_FONT_CUSTOM_DECLARE LV_FONT_DECLARE(EN)

typedef struct ui_data
{                                    // 保存的都是主题类型
    lv_subject_t gps_sat_num;        // 保存GPS卫星数量
    lv_subject_t run_status;         // 保存运动状态
    lv_subject_t battery_level;      // 保存电池电量
    lv_subject_t battery_percentage; // 保存电池电量百分比

    // general_tab的数据
    lv_subject_t gps_latitude;  // 保存纬度
    lv_subject_t gps_longitude; // 保存经度
    lv_subject_t gps_altitude;  // 保存海拔
    lv_subject_t gps_speed;     // 保存速度
    lv_subject_t gps_course;    // 保存方向
    lv_subject_t gps_hour;      // 保存小时
    lv_subject_t gps_minute;    // 保存分钟
    lv_subject_t gps_second;    // 保存秒
    lv_subject_t gps_slope;     // 保存坡度
    lv_subject_t gps_distance;  // 保存距离

} ui_data_t;

typedef struct map_args
{
    // 显示屏幕的大小
    int32_t screen_width;
    int32_t screen_height;
    // 缩放等级  需要绑定 需要是主题
    lv_subject_t zoom_level;
    uint8_t old_zoom;
    // 像素点坐标
    float pixel_x;
    float pixel_y;
    // 瓦片的坐标
    int tile_x;
    int tile_y;

    // 中心点的参数
    lv_obj_t *center_point;
    float center_x;
    float center_y;
    int center_size;
} map_args_t;

typedef struct ui
{
    ui_data_t data;
    lv_obj_t *tableview;
    lv_obj_t *staus_bar;

    // --- 状态栏 ---
    lv_obj_t *gps_label;
    lv_obj_t *run_status_label;
    lv_obj_t *battery_label;
    lv_obj_t *set_lang_con;
    lv_obj_t *set_lang_label;
    lv_obj_t *set_dark_con;
    lv_obj_t *set_dark_label;
    // --- General Tab ---
    lv_obj_t *gen_speed_obj;
    lv_obj_t *gen_speed_label;
    lv_obj_t *gen_dist_obj;
    lv_obj_t *gen_dist_label;
    lv_obj_t *gen_slope_obj;
    lv_obj_t *gen_slope_label;
    lv_obj_t *gen_time_obj;
    lv_obj_t *gen_hour_label;
    lv_obj_t *gen_second_label;
    lv_obj_t *gen_min_label;
    lv_obj_t *gen_sat_obj;
    lv_obj_t *gen_long_label;
    lv_obj_t *gen_lat_label;

    // --- GPX Tab ---
    lv_obj_t *gpx_map_obj;
    lv_obj_t *gpx_speed_obj;
    lv_obj_t *gpx_speed_label;
    lv_obj_t *gpx_mile_obj;
    lv_obj_t *gpx_mile_label;
    lv_obj_t *gpx_slider;
    lv_obj_t *set_dropdown;
    map_args_t map_args;
    // 保存地图相关计算参数
    lv_obj_t *map_container;
    // 电池
    lv_obj_t *battery_label_num; // 🚀 新增：电池百分比数字标签
} ui_t;

// 保存地图相关参数

void ui_apply_theme(ui_t *ui, int mode); // 声明函数
#endif                                   /* __UI_CONF_H__ */