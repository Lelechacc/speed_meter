#include "ui_gpx_tab.h"
uint8_t code_scroll_flag = 0;
uint8_t code_slider_flag = 0;

uint8_t map_buff[128];
// 创建地图区域样式

static void ui_map_container_init(lv_obj_t *map_obj)
{ // 去除整个容器的边框和圆角
    lv_obj_set_style_border_width(map_obj, 0, LV_PART_MAIN);
    // 圆角为0
    lv_obj_set_style_radius(map_obj, 0, LV_PART_MAIN);
    // 内边距为0
    lv_obj_set_style_pad_all(map_obj, 0, LV_PART_MAIN);
    // 行间距为0
    lv_obj_set_style_pad_row(map_obj, 0, LV_PART_MAIN);
    // 列间距为0
    lv_obj_set_style_pad_column(map_obj, 0, LV_PART_MAIN);
}
// 滑块的回调函数
static void ui_map_content_container_init(lv_obj_t *parent)
{ // 去除整个容器的边框和圆角
    lv_obj_set_style_border_width(parent, 3, LV_PART_MAIN);
    // 圆角为0
    lv_obj_set_style_radius(parent, 0, LV_PART_MAIN);
    // 内边距为0
    lv_obj_set_style_pad_all(parent, 0, LV_PART_MAIN);
    // 行间距为0
    lv_obj_set_style_pad_row(parent, 0, LV_PART_MAIN);
    // 列间距为0
    lv_obj_set_style_pad_column(parent, 0, LV_PART_MAIN);
}

/******************************************************* */
// 移动中心点
static void ui_map_center_point_move(ui_t *ui)
{
    // 第一个刷写屏幕不需要考虑中心点
    if (ui->map_args.center_x != 0 && ui->map_args.center_y != 0)
    {
        // 重新确定位置
        uint32_t x = ui->map_args.center_x - ui->map_args.tile_x * 256 + 256;
        uint32_t y = ui->map_args.center_y - ui->map_args.tile_y * 256 + 256;

        // // 确定好位置在移过去
        lv_obj_set_pos(ui->map_args.center_point, x - ui->map_args.center_size / 2, y - ui->map_args.center_size / 2);

        // ui->map_args.center_x = ui->map_args.pixel_x;
        // ui->map_args.center_y = ui->map_args.pixel_y;
        // 获取中心点容器
        lv_obj_t *center_container = lv_obj_get_parent(ui->map_args.center_point);
        // 把中心点移动到最前面，需要移动的对象和地图图片一个层级
        lv_obj_move_foreground(center_container);
    }
}
/******************************************************* */

// 计算地图坐标对应的瓦片编号 铺地图
static void ui_map_flush_img(lv_obj_t *map_container, ui_t *ui)
{
    int tile_x, tile_y;
    pixel_to_tile_float(ui->map_args.pixel_x, ui->map_args.pixel_y,
                        &tile_x, &tile_y);
    // 添加判断 => 性能提升
    // 如果滚动的距离比较小 => 不需要重新刷写的
    if (ui->map_args.tile_x != tile_x || ui->map_args.tile_y != tile_y)
    {
        ui->map_args.tile_x = tile_x;
        ui->map_args.tile_y = tile_y;
        for (uint8_t i = 0; i < 9; i++)
        {
            lv_obj_t *map_img;

            // 💡 核心优化：只在初次创建 9 个格子，之后直接复用旧格子！
            if (lv_obj_get_child_cnt(map_container) <= i)
            {
                map_img = lv_image_create(map_container);
                lv_obj_set_grid_cell(map_img, LV_GRID_ALIGN_STRETCH, i % 3, 1, LV_GRID_ALIGN_STRETCH, i / 3, 1);
            }
            else
            {
                map_img = lv_obj_get_child(map_container, i);
            }

            lv_memzero(map_buff, 128);
            sprintf((char *)map_buff, "C:/map/%d/%d/%d/tile.bmp", lv_subject_get_int(&ui->map_args.zoom_level), ui->map_args.tile_x + (i % 3) - 1, ui->map_args.tile_y + (i / 3) - 1);

            // 直接换新图片的路径，LVGL 会自动重新渲染
            lv_image_set_src(map_img, map_buff);
        }

        // ui_map_center_point_move(ui);
    }
    ui_map_center_point_move(ui);

    code_scroll_flag = 1;

    // 滚动到中间图片
    lv_obj_scroll_to_x(map_container, 256 + ((uint32_t)(ui->map_args.pixel_x) % 256) - ui->map_args.screen_width / 2, LV_ANIM_OFF);
    lv_obj_scroll_to_y(map_container, 256 + ((uint32_t)(ui->map_args.pixel_y) % 256) - ui->map_args.screen_height / 2, LV_ANIM_OFF);
}

/******************************************************* */
// 添加中心点

static void ui_map_center_point(lv_obj_t *map_container, ui_t *ui)
{

    // map_container有网格布局不能直接设置到中间点 所以加一个中间层
    lv_obj_t *track_center = lv_obj_create(map_container);
    // 配置无边框样式
    ui_map_container_init(track_center);
    // 设置居中
    lv_obj_set_pos(track_center, 0, 0);
    // 占满父元素
    lv_obj_set_size(track_center, TILE_SIZE * 3, TILE_SIZE * 3);
    lv_obj_set_grid_cell(track_center, LV_GRID_ALIGN_STRETCH, 0, 3, LV_GRID_ALIGN_STRETCH, 0, 3);

    // 设置背景为透明
    lv_obj_set_style_bg_opa(track_center, LV_OPA_TRANSP, 0);
    // 取消滚动
    lv_obj_remove_flag(track_center, LV_OBJ_FLAG_SCROLLABLE);
    /******************************************************* */

    // 添加中心点
    lv_obj_t *center_point = lv_obj_create(track_center);
    ui->map_args.center_point = center_point;
    // 常量值
    ui->map_args.center_size = 16;
    lv_obj_set_size(center_point, ui->map_args.center_size, ui->map_args.center_size);
    // 不是使用像素值 而是使用瓦片坐标
    ui->map_args.center_x = ui->map_args.pixel_x;
    ui->map_args.center_y = ui->map_args.pixel_y;

    lv_obj_set_pos(center_point,
                   256 + (uint32_t)(ui->map_args.center_x) % 256 - ui->map_args.center_size / 2,
                   256 + (uint32_t)(ui->map_args.center_y) % 256 - ui->map_args.center_size / 2);
}

/******************************************************* */

void ui_map_scroll_cb(lv_event_t *e)
{
    // 1. 防死循环锁生效
    if (code_scroll_flag == 1)
    {
        code_scroll_flag = 0;
        return;
    }

    lv_obj_t *map_container = lv_event_get_target(e);
    ui_t *ui = lv_event_get_user_data(e);

    // 2.   注意看！这里是 pixel_x，绝对不能是 tile_x！
    int old_x = 256 + ((uint32_t)(ui->map_args.pixel_x) % 256) - ui->map_args.screen_width / 2;
    int old_y = 256 + ((uint32_t)(ui->map_args.pixel_y) % 256) - ui->map_args.screen_height / 2;

    // 3.  注意看！这里是 get_scroll_x，绝对不能是 get_x！
    int new_x = lv_obj_get_scroll_x(map_container);
    int new_y = lv_obj_get_scroll_y(map_container);

    // 计算移动距离并累加到真实坐标
    ui->map_args.pixel_x += (new_x - old_x);
    ui->map_args.pixel_y += (new_y - old_y);

    // 重新铺图
    ui_map_flush_img(map_container, ui);
}

// 创建地图界面
static void ui_map_init(lv_obj_t *map_obj, ui_t *ui)
{
    // 填充9张图片=> 需要单独的图层
    lv_obj_t *map_container = lv_obj_create(map_obj);
    // 保存到ui
    ui->map_container = map_container;
    // 占满父元素
    lv_obj_set_size(map_container, LV_PCT(100), LV_PCT(100));
    // 子元素设置样式 和父对象相同 没有边距 打满
    ui_map_container_init(map_container);
    /******************************************************* */
    // 去除链式滚动
    lv_obj_remove_flag(map_container, LV_OBJ_FLAG_SCROLL_CHAIN);
    // 滚动条隐藏显示
    lv_obj_set_scrollbar_mode(map_container, LV_SCROLLBAR_MODE_OFF);
    // 关闭滚动动量！
    lv_obj_remove_flag(map_container, LV_OBJ_FLAG_SCROLL_MOMENTUM);
    // 去除弹性滚动
    lv_obj_remove_flag(map_container, LV_OBJ_FLAG_SCROLL_ELASTIC);
    // 获取屏幕的大小
    lv_obj_update_layout(map_container);
    ui->map_args.screen_width = lv_obj_get_width(map_container);
    ui->map_args.screen_height = lv_obj_get_height(map_container);

    /******************************************************* */

    // 设置9张图片的网格
    lv_obj_set_layout(map_container, LV_LAYOUT_GRID);
    // 设置图片的网格布局 3行3列
    static int32_t column_dsc[] = {TILE_SIZE, TILE_SIZE, TILE_SIZE};
    static int32_t row_dsc[] = {TILE_SIZE, TILE_SIZE, TILE_SIZE};
    lv_obj_set_grid_dsc_array(map_container, column_dsc, row_dsc);
    /******************************************************* */

    // 初始化缩放等级
    lv_subject_init_int(&ui->map_args.zoom_level, 14);
    //    核心参数计算  =>s使用经纬度计算像素点坐标

    latlng_to_pixel_float(lv_subject_get_float(&ui->data.gps_longitude),
                          lv_subject_get_float(&ui->data.gps_latitude),
                          lv_subject_get_int(&ui->map_args.zoom_level), &ui->map_args.pixel_x, &ui->map_args.pixel_y);
    printf("pixel_x: %f, pixel_y: %f\n", ui->map_args.pixel_x, ui->map_args.pixel_y);
    // 获取坐标 计算要铺的9张图片   先刷写地图 后触发回调
    ui_map_flush_img(map_container, ui);

    // 添加地图的回调函数
    lv_obj_add_event_cb(map_container, ui_map_scroll_cb, LV_EVENT_SCROLL_END, ui);

    /******************************************************* */
    // 添加地图的中心点 初始化
    ui_map_center_point(map_container, ui);
}

void ui_map_slider_cb(lv_event_t *e)
{
    if (code_slider_flag == 1)
    {
        code_slider_flag = 0;
        return;
    }

    ui_t *ui = lv_event_get_user_data(e);

    // 重新计算像素点坐标
    pixel_zoom_convert_float(ui->map_args.center_x,
                             ui->map_args.center_y, ui->map_args.old_zoom,
                             lv_subject_get_int((&ui->map_args.zoom_level)),
                             &ui->map_args.center_x, &ui->map_args.center_y);

    // 移动中心点
    pixel_zoom_convert_float(ui->map_args.pixel_x, ui->map_args.pixel_y, ui->map_args.old_zoom, lv_subject_get_int((&ui->map_args.zoom_level)), &ui->map_args.pixel_x, &ui->map_args.pixel_y);

    // 重新计算瓦片坐标
    ui->map_args.old_zoom = lv_subject_get_int((&ui->map_args.zoom_level));
    // 刷新地图
    ui_map_flush_img(ui->map_container, ui);
}
void ui_map_center_btn_cb(lv_event_t *e)
{
    ui_t *ui = lv_event_get_user_data(e);

    // 最初的缩放等级固定是14
    code_slider_flag = 1;
    lv_subject_set_int(&ui->map_args.zoom_level, 14);
    ui->map_args.old_zoom = 14;

    // 重新使用GPS坐标计算铺图
    latlng_to_pixel_float(lv_subject_get_float(&ui->data.gps_longitude), lv_subject_get_float(&ui->data.gps_latitude), lv_subject_get_int(&ui->map_args.zoom_level), &ui->map_args.pixel_x, &ui->map_args.pixel_y);
    // 重新赋值中心点坐标
    ui->map_args.center_x = ui->map_args.pixel_x;
    ui->map_args.center_y = ui->map_args.pixel_y;
    // 铺图
    ui_map_flush_img(ui->map_container, ui);
}
// 创建滑块容器  （功能组件）
static void ui_map_slider_init(lv_obj_t *map_obj, ui_t *ui)

{

    // 创建滑块容器  （功能组件）
    lv_obj_t *slider_obj = lv_slider_create(map_obj);
    ui->gpx_slider = slider_obj;
    // 设置滑块大小
    lv_obj_set_size(slider_obj, 23, LV_PCT(70));
    // 设置位置在Map_ojb的右边
    lv_obj_align(slider_obj, LV_ALIGN_RIGHT_MID, -10, 0);

    // 初始化缩放等级
    // 设置缩放等级

    ui->map_args.old_zoom = 14;
    lv_subject_init_int(&ui->map_args.zoom_level, ui->map_args.old_zoom);
    lv_slider_set_range(slider_obj, 3, 14);
    // 绑定到主题
    lv_slider_bind_value(slider_obj, &ui->map_args.zoom_level);

    // 添加滑块的回调函数
    lv_obj_add_event_cb(slider_obj, ui_map_slider_cb, LV_EVENT_VALUE_CHANGED, ui);

    // 按钮
    lv_obj_t *center_btn = lv_button_create(map_obj);
    lv_obj_set_size(center_btn, 35, 35);
    lv_obj_align(center_btn, LV_ALIGN_BOTTOM_LEFT, 10, -10);

    // 添加按钮的图标
    lv_obj_t *center_btn_label = lv_label_create(center_btn);
    lv_label_set_text(center_btn_label, LV_SYMBOL_TINT);
    lv_obj_align(center_btn_label, LV_ALIGN_CENTER, 0, 0);

    // 添加按钮的回调函数
    lv_obj_add_event_cb(center_btn, ui_map_center_btn_cb, LV_EVENT_CLICKED, ui);
}

/******************************************************* */

/******************************************************* */
void ui_gpx_tab_init(lv_obj_t *parent, ui_t *ui)
{
    // 去除整个容器的边框和圆角
    lv_obj_set_style_border_width(parent, 0, LV_PART_MAIN);
    // 圆角为0
    lv_obj_set_style_radius(parent, 0, LV_PART_MAIN);
    // 内边距为0
    lv_obj_set_style_pad_all(parent, 0, LV_PART_MAIN);
    // 行间距为0
    lv_obj_set_style_pad_row(parent, 0, LV_PART_MAIN);
    // 列间距为0
    lv_obj_set_style_pad_column(parent, 0, LV_PART_MAIN);
    // 布局为网格
    lv_obj_set_layout(parent, LV_LAYOUT_GRID);
    // 设置网格布局的行列数 5行2列
    // 初始化功能组件：记得把 ui 传给它！
    static uint32_t column_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static uint32_t row_dsc[] = {LV_GRID_FR(4), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(parent, (const int32_t *)column_dsc, (const int32_t *)row_dsc);
    /*********************************************** */
    /*********************************************** */

    // 创建地图区域
    lv_obj_t *map_obj = lv_obj_create(parent);
    // 把map_obj绑定到ui->gpx_map_obj
    ui->gpx_map_obj = map_obj;
    // 放到格子里
    lv_obj_set_grid_cell(map_obj, LV_GRID_ALIGN_STRETCH, 0, 2, LV_GRID_ALIGN_STRETCH, 0, 1);
    // 初始化地图容器
    ui_map_container_init(map_obj);
    // 填充地图
    ui_map_init(map_obj, ui);

    // 初始化功能组件
    ui_map_slider_init(map_obj, ui);
    /*********************************************** */

    // 创建速度区域
    lv_obj_t *speed_obj = lv_obj_create(parent);
    // 把speed_obj绑定到ui->gpx_speed_obj
    ui->gpx_speed_obj = speed_obj;
    // 把速度区域保存起来
    ui->gpx_speed_obj = speed_obj;
    // 放到格子里
    lv_obj_set_grid_cell(speed_obj, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
    // 初始化速度容器
    ui_map_content_container_init(speed_obj);
    // 填充速度数值到速度容器里
    lv_obj_t *speed_label = lv_label_create(speed_obj);
    // 设置速度容器子元素居中显示
    lv_obj_set_align(speed_label, LV_ALIGN_CENTER);
    // 填充速度数值到速度容器里
    lv_label_bind_text(speed_label, &ui->data.gps_speed, "%0.1f km/h");
    ui->gpx_speed_label = speed_label;
    /*********************************************** */

    // 创建里程区域
    lv_obj_t *mileage_obj = lv_obj_create(parent);
    ui->gpx_mile_obj = mileage_obj;
    // 放到格子里
    lv_obj_set_grid_cell(mileage_obj, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
    // 初始化里程容器
    ui_map_content_container_init(mileage_obj);

    /*********************************************** */

    // 创造里程容器
    lv_obj_t *mileage_label = lv_label_create(mileage_obj);
    // 把里程绑定到ui->gpx_mile_label
    ui->gpx_mile_label = mileage_label;
    // 设置里程容器子元素居中显示
    lv_obj_set_align(mileage_label, LV_ALIGN_CENTER);

    // 填充里程数值到里程容器里
    lv_label_bind_text(mileage_label, &ui->data.gps_distance, "%0.1f km");

    /*********************************************** */
    /*********************************************** */
}


// --------------------------------------------------
// 暴露给物理按键 KEY2 调用的“一键定位（水滴）”包装函数
void ui_map_center_wrapper(void *arg)
{
    ui_t *ui = (ui_t *)arg;
    if (!ui) return;

    // 跟水滴屏幕按钮一样的逻辑：锁定位、缩放设为14、重新计算位置
    extern uint8_t code_slider_flag;
    code_slider_flag = 1;
    
    lv_subject_set_int(&ui->map_args.zoom_level, 14);
    ui->map_args.old_zoom = 14;

    // 重新使用真实的GPS坐标计算铺图
    latlng_to_pixel_float(
        lv_subject_get_float(&ui->data.gps_longitude), 
        lv_subject_get_float(&ui->data.gps_latitude), 
        lv_subject_get_int(&ui->map_args.zoom_level), 
        &ui->map_args.pixel_x, &ui->map_args.pixel_y
    );
    
    // 重新赋值红点的中心点物理坐标
    ui->map_args.center_x = ui->map_args.pixel_x;
    ui->map_args.center_y = ui->map_args.pixel_y;
    
    // 刷新铺图
    ui_map_flush_img(ui->map_container, ui);
    
    printf("Map centered via KEY2!\n");
}
// --------------------------------------------------
// // ----------------- 模拟行走功能区 (10倍速 + 速度联动显示) -----------------
// static lv_timer_t * gps_sim_timer = NULL;
// static bool is_simulating = false; // 记录当前是否在模拟行走
// static int sim_step_count = 0;     // 剩余的总移动步数
// static int walk_pause_counter = 0; // 用于控制走停的节拍器

// // 定时器每次滴答要干的事（500毫秒触发一次）
// static void simulate_gps_timer_cb(lv_timer_t *timer)
// {
//     ui_t *ui = (ui_t *)lv_timer_get_user_data(timer);

//     if (sim_step_count <= 0) {
//         lv_timer_pause(timer); 
//         is_simulating = false;
//         // 🚨 步数走完彻底停下，速度归零！
//         lv_subject_set_float(&ui->data.gps_speed, 0.0f); 
//         printf("Simulation finished!\n");
//         return;
//     }

//     walk_pause_counter++;

//     // 节奏控制：走 5 步，停 3 步
//     if (walk_pause_counter % 8 >= 5) {
//         // 🚨 遇到红绿灯停下休息，速度瞬间归零！
//         lv_subject_set_float(&ui->data.gps_speed, 0.0f); 
//         return; 
//     }

//     // --- 下面是处于“移动”状态执行的代码 ---
//     sim_step_count--; 

//     // 1. 获取当前经纬度
//     float current_lon = lv_subject_get_float(&ui->data.gps_longitude);
//     float current_lat = lv_subject_get_float(&ui->data.gps_latitude);
    
//     // 🚨 速度翻 10 倍！
//     lv_subject_set_float(&ui->data.gps_longitude, current_lon + 0.0005f);
//     lv_subject_set_float(&ui->data.gps_latitude, current_lat + 0.0005f);

//     // 2. 重新计算红点的物理坐标
//     latlng_to_pixel_float(
//         lv_subject_get_float(&ui->data.gps_longitude),
//         lv_subject_get_float(&ui->data.gps_latitude),
//         lv_subject_get_int(&ui->map_args.zoom_level),
//         &ui->map_args.center_x, &ui->map_args.center_y
//     );

//     // 3. 让红点在屏幕上滑过去
//     ui_map_center_point_move(ui);
    
//     // 4. 里程表狂飙 (每次加 0.1km)
//     float current_dist = lv_subject_get_float(&ui->data.gps_distance);
//     lv_subject_set_float(&ui->data.gps_distance, current_dist + 0.1f);

//     // 5. 🚨 核心联动：给界面发送实时速度！
//     lv_subject_set_float(&ui->data.gps_speed, 45.5f); 
// }

// // 这是暴露给物理按键调用的“启停开关”（你上次丢掉的就是这个！）
// void ui_toggle_simulation_wrapper(void *arg)
// {
//     ui_t *ui = (ui_t *)arg;
//     if (!ui) return;

//     is_simulating = !is_simulating; // 切换状态

//     if (is_simulating) {
//         // 如果是从头开始跑，重置步数和节拍器
//         if (sim_step_count <= 0) {
//             sim_step_count = 100;    // 设定总共移动 100 步后自动结束
//             walk_pause_counter = 0;  // 节拍器清零
//         }

//         if (gps_sim_timer == NULL) {
//             gps_sim_timer = lv_timer_create(simulate_gps_timer_cb, 500, ui);
//         } else {
//             lv_timer_resume(gps_sim_timer);
//         }
//         printf("Simulation Started (10x Speed & Stop-and-Go)!\n");
//     } else {
//         if (gps_sim_timer != NULL) {
//             lv_timer_pause(gps_sim_timer);
//         }
//         // 🚨 强行暂停时，也要把面板上的速度归零
//         lv_subject_set_float(&ui->data.gps_speed, 0.0f); 
//         printf("Simulation Paused!\n");
//     }
// }


// ----------------- 模拟行走功能区 (10倍速 + 跑5秒即停) -----------------
static lv_timer_t * gps_sim_timer = NULL;
static bool is_simulating = false; // 记录当前是否在模拟行走
static int sim_step_count = 0;     // 剩余的总移动步数

// 定时器每次滴答要干的事（500毫秒触发一次）
static void simulate_gps_timer_cb(lv_timer_t *timer)
{
    ui_t *ui = (ui_t *)lv_timer_get_user_data(timer);

    // 🚨 判断：如果步数走完了（10步），彻底刹车！
    if (sim_step_count <= 0) {
        lv_timer_pause(timer); 
        is_simulating = false;
        // 速度表瞬间归零！
        lv_subject_set_float(&ui->data.gps_speed, 0.0f); 
        printf("Simulation finished!\n");
        return;
    }

    // --- 下面是移动状态 ---
    sim_step_count--; // 扣除一步

    // 1. 获取当前经纬度
    float current_lon = lv_subject_get_float(&ui->data.gps_longitude);
    float current_lat = lv_subject_get_float(&ui->data.gps_latitude);
    
    // 🚨 维持 10 倍速的位移差！
    lv_subject_set_float(&ui->data.gps_longitude, current_lon + 0.0005f);
    lv_subject_set_float(&ui->data.gps_latitude, current_lat + 0.0005f);

    // 2. 重新计算红点的物理坐标
    latlng_to_pixel_float(
        lv_subject_get_float(&ui->data.gps_longitude),
        lv_subject_get_float(&ui->data.gps_latitude),
        lv_subject_get_int(&ui->map_args.zoom_level),
        &ui->map_args.center_x, &ui->map_args.center_y
    );

    // 3. 让红点在屏幕上滑过去
    ui_map_center_point_move(ui);
    
    // 4. 里程表狂飙 (每次加 0.1km)
    float current_dist = lv_subject_get_float(&ui->data.gps_distance);
    lv_subject_set_float(&ui->data.gps_distance, current_dist + 0.1f);

    // 5. 核心联动：给界面发送实时速度 45.5km/h！
    lv_subject_set_float(&ui->data.gps_speed, 45.5f); 
}

// 物理按键调用的“启停开关”
void ui_toggle_simulation_wrapper(void *arg)
{
    ui_t *ui = (ui_t *)arg;
    if (!ui) return;

    is_simulating = !is_simulating; // 切换状态

    if (is_simulating) {
        // 🚨 核心修改：设定总共只移动 10 步 (刚好 5 秒)
        if (sim_step_count <= 0) {
            sim_step_count = 10;    
        }

        if (gps_sim_timer == NULL) {
            gps_sim_timer = lv_timer_create(simulate_gps_timer_cb, 500, ui);
        } else {
            lv_timer_resume(gps_sim_timer);
        }
        printf("Simulation Started (Running for 5 seconds)!\n");
    } else {
        if (gps_sim_timer != NULL) {
            lv_timer_pause(gps_sim_timer);
        }
        // 强行暂停时，也要把面板上的速度归零
        lv_subject_set_float(&ui->data.gps_speed, 0.0f); 
        printf("Simulation Paused!\n");
    }
}
// --------------------------------------------------
// --------------------------------------------------