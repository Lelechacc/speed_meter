#include "ui_general_tab.h"

void ui_general_tab_init(lv_obj_t *parent, ui_t *ui)
{
    // 设置网格布局
    lv_obj_set_layout(parent, LV_LAYOUT_GRID);
    // 设置网格布局参数
    // 列布局
    static int32_t colum_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    // 行布局
    static int32_t row_dsc[] = {LV_GRID_FR(2), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    // 设置网格布局参数
    lv_obj_set_grid_dsc_array(parent, colum_dsc, row_dsc);

    // 设置table格式设置
    //  1.1.去除table圆角
    lv_obj_set_style_radius(parent, 0, LV_PART_MAIN);
    // 1.2.设置table边框
    lv_obj_set_style_border_width(parent, 0, LV_PART_MAIN);
    // 1.3.去除table边距
    lv_obj_set_style_pad_all(parent, 0, LV_PART_MAIN);
    // 设置背景颜色为薄荷底色
    // lv_obj_set_style_bg_color(parent, lv_color_hex(0xF4F0FF), 0);
    // 设置卡片之间的间隙，露出底色
    lv_obj_set_style_pad_all(parent, 10, LV_PART_MAIN);
    lv_obj_set_style_pad_row(parent, 8, LV_PART_MAIN);
    lv_obj_set_style_pad_column(parent, 8, LV_PART_MAIN);
    /*********************************************************** */
    // 2.1.填充网格空间，把速度的框子设置好
    lv_obj_t *speed_obj = lv_obj_create(parent);
    // 把speed_obj绑定到ui->gen_speed_obj
    ui->gen_speed_obj = speed_obj;
    lv_obj_clear_flag(speed_obj, LV_OBJ_FLAG_SCROLLABLE); // 禁止滑动
    // 2.2 把speed_obj放到第1行第1列，占2列
    lv_obj_set_grid_cell(speed_obj, LV_GRID_ALIGN_STRETCH, 0, 2, LV_GRID_ALIGN_STRETCH, 0, 1);
    // 2.3 设置speed_obj边框，设置speed_obj圆角
    lv_obj_set_style_radius(speed_obj, 10, LV_PART_MAIN);
    lv_obj_set_style_border_width(speed_obj, 2, LV_PART_MAIN);
    // 2.4设置边框的样式：深青边框 + 纯白底 + 15圆角
    lv_obj_set_style_radius(speed_obj, 15, 0);
    lv_obj_set_style_border_width(speed_obj, 3, 0);
    // lv_obj_set_style_border_color(speed_obj, lv_color_hex(0xA06CD5), 0);
    // lv_obj_set_style_bg_color(speed_obj, lv_color_hex(0xFFFFFF), 0);
    /*********************************************************** */
    // 3.1.填充速度的格子
    lv_obj_t *speed_label = lv_label_create(speed_obj);
    // 把speed_label绑定到ui->gen_speed_label
    ui->gen_speed_label = speed_label; //
    // 绑定速度到速度格子
    lv_label_bind_text(speed_label, &ui->data.gps_speed, "%0.1f km/h");
    // 速度字体大小设置
    lv_obj_set_style_text_font(speed_label, &lv_font_montserrat_30, LV_PART_MAIN);
    // 把速度文字放到速度格子中间
    lv_obj_align(speed_label, LV_ALIGN_CENTER, 0, 0);
    // 设置深海蓝文字
    // lv_obj_set_style_text_color(speed_label, lv_color_hex(0x2D1B4E), 0);
    /*********************************************************** */
    // 4.1.创建距离的格子
    lv_obj_t *distance_obj = lv_obj_create(parent);
    // 把distance_obj绑定到ui->gen_dist_obj
    ui->gen_dist_obj = distance_obj;
    // 4.2 把distance_obj放到第2行第1列，占2列
    lv_obj_set_grid_cell(distance_obj, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
    // 4.3禁止滑动
    lv_obj_clear_flag(distance_obj, LV_OBJ_FLAG_SCROLLABLE);
    // 4.4 设置distance_obj边框，设置distance_obj圆角
    lv_obj_set_style_radius(distance_obj, 12, 0);
    lv_obj_set_style_border_width(distance_obj, 3, 0);
    // lv_obj_set_style_border_color(distance_obj, lv_color_hex(0xA06CD5), 0);
    /*********************************************************** */
    // 4.5填充距离的格子
    lv_obj_t *distance_label = lv_label_create(distance_obj);
    ui->gen_dist_label = distance_label;
    // 绑定距离lable到距离格子
    lv_label_bind_text(distance_label, &ui->data.gps_distance, "%0.2f km");
    // 距离字体大小设置
    lv_obj_set_style_text_font(distance_label, &lv_font_montserrat_20, 0);
    // 亮蓝色文字
    // lv_obj_set_style_text_color(distance_label, lv_color_hex(0x7A4CBA), 0);
    // 把距离文字放到距离格子中间
    lv_obj_align(distance_label, LV_ALIGN_CENTER, 0, 0);
    /*********************************************************** */
    // 5.1.创建坡度的格子
    lv_obj_t *slope_obj = lv_obj_create(parent);
    ui->gen_slope_obj = slope_obj;
    // 6.2 把slope_obj放到第2行第2列，占1列
    lv_obj_set_grid_cell(slope_obj, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
    // 6.3禁止滑动
    lv_obj_clear_flag(slope_obj, LV_OBJ_FLAG_SCROLLABLE);
    // 6.4 添加边框
    lv_obj_set_style_radius(slope_obj, 12, 0);
    lv_obj_set_style_border_width(slope_obj, 3, 0);
    // lv_obj_set_style_border_color(slope_obj, lv_color_hex(0xA06CD5), 0);
    /*********************************************************** */
    // 5.5填充坡度的格子
    lv_obj_t *slope_label = lv_label_create(slope_obj);
    ui->gen_slope_label = slope_label;
    // 绑定坡度lable到坡度格子
    lv_label_bind_text(slope_label, &ui->data.gps_slope, "%0.2f %%");
    // 坡度字体大小设置
    lv_obj_set_style_text_font(slope_label, &lv_font_montserrat_20, 0);
    // 亮蓝色文字
    // lv_obj_set_style_text_color(slope_label, lv_color_hex(0x7A4CBA), 0);
    // 把坡度文字放到坡度格子中间
    lv_obj_align(slope_label, LV_ALIGN_CENTER, 0, 0);

    /*********************************************************** */
    // 6.1.创建时间的格子
    lv_obj_t *time_obj = lv_obj_create(parent);
    ui->gen_time_obj = time_obj;
    // 6.2 把time_obj放到第2行第1列，占2列
    lv_obj_set_grid_cell(time_obj, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 2, 1);
    // 6.3禁止滑动
    lv_obj_clear_flag(time_obj, LV_OBJ_FLAG_SCROLLABLE);
    // 6.4 添加边框
    lv_obj_set_style_radius(time_obj, 12, 0);
    // 6.4 设置time_obj圆角
    lv_obj_set_style_border_width(time_obj, 3, 0);
    // 6.5 边框的样式：深青边框 + 纯白底 + 15圆角
    // lv_obj_set_style_border_color(time_obj, lv_color_hex(0xA06CD5), 0);
    /*********************************************************** */
    // 6.6填充时间的格子
    // 创建秒的标签lable
    lv_obj_t *second_label = lv_label_create(time_obj);
    // 创建分钟lable
    lv_obj_t *minute_label = lv_label_create(time_obj);
    // 创建小时lable
    lv_obj_t *time_label = lv_label_create(time_obj);
    /*********************************************************** */

    ui->gen_second_label = second_label;
    ui->gen_min_label = minute_label;
    ui->gen_hour_label = time_label;
    /*********************************************************** */

    // 绑定小时lable到时间格子
    lv_label_bind_text(time_label, &ui->data.gps_hour, "%02d:");
    // 绑定分钟able到分钟格子
    lv_label_bind_text(minute_label, &ui->data.gps_minute, " %02d");
    // 绑定秒able到秒格子
    lv_label_bind_text(second_label, &ui->data.gps_second, ":%02d"); // %02d 保证秒是两位数，比如 : 05
    /*********************************************************** */

    // 把分钟文字放到格子中间
    lv_obj_align(minute_label, LV_ALIGN_CENTER,-5, 0);
    // 把小时文字放到格子中间
    lv_obj_align(time_label, LV_ALIGN_CENTER, -27, 0);
    // 把秒文字放到格子中间
    lv_obj_align(second_label, LV_ALIGN_CENTER, 29, 0);
    /*********************************************************** */
    // 秒字体大小设置
    lv_obj_set_style_text_font(second_label, &lv_font_montserrat_20, 0);
    // 分钟字体大小设置
    lv_obj_set_style_text_font(minute_label, &lv_font_montserrat_20, 0);
    // 小时字体大小设置
    lv_obj_set_style_text_font(time_label, &lv_font_montserrat_20, 0);

    /*********************************************************** */
    // 7.1.创建经纬度的格子
    lv_obj_t *sat_obj = lv_obj_create(parent);
    ui->gen_sat_obj = sat_obj;
    // 7.2 把sat_obj放到第2行第2列，占1列
    lv_obj_set_grid_cell(sat_obj, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 2, 1);
    // 7.3禁止滑动
    lv_obj_clear_flag(sat_obj, LV_OBJ_FLAG_SCROLLABLE);
    // 7.4 添加边框
    lv_obj_set_style_radius(sat_obj, 12, 0);
    lv_obj_set_style_border_width(sat_obj, 3, 0);
    // lv_obj_set_style_border_color(sat_obj, lv_color_hex(0xA06CD5), 0);
    /*********************************************************** */
    // 7.5填充格子
    // 经度的格子
    lv_obj_t *longitude_label = lv_label_create(sat_obj);
    ui->gen_long_label = longitude_label;
    // 纬度的格子
    lv_obj_t *latitude_label = lv_label_create(sat_obj);
    ui->gen_lat_label = latitude_label;

    // 绑定经度lable到经度格子
    lv_label_bind_text(longitude_label, &ui->data.gps_latitude, "%0.4f");
    // 绑定纬度lable到纬度格子
    lv_label_bind_text(latitude_label, &ui->data.gps_longitude, "%0.4f");
    // 经度字体大小设置
    lv_obj_set_style_text_font(longitude_label, &lv_font_montserrat_20, LV_PART_MAIN);
    // 纬度字体大小设置
    lv_obj_set_style_text_font(latitude_label, &lv_font_montserrat_20, LV_PART_MAIN);
    // // 亮蓝色文字
    // lv_obj_set_style_text_color(longitude_label, lv_color_hex(0x7A4CBA), 0);
    // lv_obj_set_style_text_color(latitude_label, lv_color_hex(0x7A4CBA), 0);

    // 把字放到经度格子中间
    lv_obj_align(longitude_label, LV_ALIGN_CENTER, 0, -10);
    lv_obj_align(latitude_label, LV_ALIGN_CENTER, 0, 10);
    /*********************************************************** */
}


