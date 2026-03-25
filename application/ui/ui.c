#include "ui.h"
ui_t *ui; // 全局指针！

// 最小化边距和圆角  只要是父对象是obj的  就可以用 删除边框和圆角
static void ui_container_style_init(lv_obj_t *obj)
{
    // 删除内边距
    lv_obj_set_style_pad_all(obj, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_gap(obj, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN);
    // 删除边框和圆角
    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN);
}
static void init_data(ui_t *ui)
{
    // 卫星数量
    lv_subject_init_int(&ui->data.gps_sat_num, 7);
    // 运动状态
    lv_subject_init_pointer(&ui->data.run_status, (void *)run_status_text[0]);
    // 电池电量
    lv_subject_init_pointer(&ui->data.battery_level, (void *)LV_SYMBOL_CHARGE);
    /*********************************************************** */
    // 初始化general_tab数据
    // 初始化浮点数的速度
    lv_subject_init_float(&ui->data.gps_speed, 66.6f);
    // 初始化整数的小时
    lv_subject_init_int(&ui->data.gps_hour, 10);
    // 初始化整数的分钟
    lv_subject_init_int(&ui->data.gps_minute, 29);
    // 初始化整数的秒
    lv_subject_init_int(&ui->data.gps_second, 12);
    // 初始化浮点数的距离
    lv_subject_init_float(&ui->data.gps_distance, 123.0f);
    // 初始化浮点数的坡度
    lv_subject_init_float(&ui->data.gps_slope, 32.0f);
    // 初始化浮点数的海拔
    lv_subject_init_float(&ui->data.gps_altitude, 721.0f);
    // 初始化浮点数的纬度
    lv_subject_init_float(&ui->data.gps_longitude, 113.835042f);
    // 初始化浮点数的经度
    lv_subject_init_float(&ui->data.gps_latitude, 22.628743f);
    // 初始化浮点数的方向
    lv_subject_init_float(&ui->data.gps_course, 73.0f);
    // 初始化整数的电池电量百分比
    lv_subject_init_int(&ui->data.battery_percentage, 100);
}
static lv_obj_t *init_screen(void)
{
    // 创建屏幕
    lv_obj_t *screen = lv_screen_active();

    // 弹性布局
    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
    // 删除内边距
    lv_obj_set_style_pad_all(screen, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_gap(screen, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_column(screen, 0, LV_PART_MAIN);
    return screen;
}

/**
 * @brief 初始化状态栏
 *
 * @param ui
 * @param screen
 * @return lv_obj_t*
 * */
static lv_obj_t *init_status_bar(ui_t *ui, lv_obj_t *screen)
{
    /************************************************** */
    // 1. 创建状态栏
    ui->staus_bar = lv_obj_create(screen);
    // 设置状态栏高度
    lv_obj_set_size(ui->staus_bar, LV_PCT(100), 24);
    // 设置状态栏布局打满
    lv_obj_set_flex_grow(ui->staus_bar, 1);
    // 按照设定好的样式规定组件
    ui_container_style_init(ui->staus_bar);

    // --- 【修改点：背景颜色】 ---
    // 设置状态栏颜色 - 夏日清爽：薄荷冰蓝 (极浅青色，清爽打底)
    // lv_obj_set_style_bg_color(ui->staus_bar, lv_color_hex(0xE2CFFE), LV_PART_MAIN);

    // --- 【新增：糖果效果边框】 ---
    // 设置边框颜色：深海青 (强对比，防止界面太“素”)
    // lv_obj_set_style_border_color(ui->staus_bar, lv_color_hex(0xA06CD5), LV_PART_MAIN);
    // 设置边框宽度：3 (加粗边框才有糖果那种厚实的包边感)
    lv_obj_set_style_border_width(ui->staus_bar, 0, LV_PART_MAIN);
    // 设置圆角：12 (马卡龙/糖果风的核心，一定要圆润才可爱)
    lv_obj_set_style_radius(ui->staus_bar, 0, LV_PART_MAIN);
    // 设置背景透明度：全显
    lv_obj_set_style_bg_opa(ui->staus_bar, LV_OPA_COVER, LV_PART_MAIN);
    // --- 重点：开启自动排队 (Flex) ---
    lv_obj_set_flex_flow(ui->staus_bar, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui->staus_bar, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(ui->staus_bar, 5, 0); // 每个图标之间只空5像素
    lv_obj_set_style_pad_hor(ui->staus_bar, 8, 0);    // 左右两边留点边距
    /************************************************** */
    // 2.1.1 添加内容  GPS卫星数量  运动状态 电池电量
    lv_obj_t *GPS_label = lv_label_create(ui->staus_bar);
    // 把GPS_label绑定到ui->gps_label
    ui->gps_label = GPS_label;
    // 2.1.2绑定状态栏左侧的卫星数量
    lv_label_bind_text(GPS_label, &ui->data.gps_sat_num, LV_SYMBOL_GPS "  %d");
    // 2.1.3左对齐
    lv_obj_set_style_text_font(ui->gps_label, &lv_font_montserrat_12, 0); // 缩小字体

    // --- 【修改点：GPS标签颜色】 ---
    // 设置GPS颜色 - 深青色 (保持色调统一，且在浅色底上清晰可见)
    // lv_obj_set_style_text_color(GPS_label, lv_color_hex(0x2D1B4E), LV_PART_MAIN);
    //
    /************************************************** */
    // 2.2.1添加内容 运动状态
    lv_obj_t *run_status_label = lv_label_create(ui->staus_bar);
    // 把run_status_label绑定到ui->run_status_label
    ui->run_status_label = run_status_label;
    // 2.2.2绑定状态栏中间的运行状态
    lv_label_bind_text(run_status_label, &ui->data.run_status, " %s");

    // 2.2.3把绑定的运行状态标签中间对齐
    lv_obj_set_flex_grow(ui->run_status_label, 1);
    lv_obj_set_style_text_align(ui->run_status_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(ui->run_status_label, &lv_font_montserrat_12, 0);
    // --- 【修改点：运动状态颜色】 ---
    // 设置运动状态颜色 - 亮蓝色 (夏日海浪感，突出中间信息)
    // lv_obj_set_style_text_color(run_status_label, lv_color_hex(0x7A4CBA), LV_PART_MAIN);

    /************************************************** */
    // 2.3.1 添加内容 电池电量
    lv_obj_t *battery_label = lv_label_create(ui->staus_bar);
    // 把battery_label绑定到ui->battery_label
    ui->battery_label = battery_label;
    // 2.3.2绑定状态栏右侧的电量
    lv_label_bind_text(battery_label, &ui->data.battery_level, "  %s");
    // 2.3.3把绑定的电量标签对齐
    lv_obj_set_style_text_font(ui->battery_label, &lv_font_montserrat_12, 0);

    // 创建百分比数字标签
    // 4. 电池数字 (右侧)
    ui->battery_label_num = lv_label_create(ui->staus_bar);
    lv_label_bind_text(ui->battery_label_num, &ui->data.battery_percentage, "%d%%");
    lv_obj_set_style_text_font(ui->battery_label_num, &lv_font_montserrat_12, 0);
    return ui->staus_bar;
}

static void init_tableview(ui_t *ui, lv_obj_t *screen)
{
    // 1. 创建tableview
    ui->tableview = lv_tabview_create(screen);

    // 设置tableview的宽度 是屏幕的100%
    lv_obj_set_width(ui->tableview, LV_PCT(100));
    // 设置tableview在屏幕布局里的高度占9份
    lv_obj_set_flex_grow(ui->tableview, 9);
    // 设置tableview里的标签tabar的高度
    lv_tabview_set_tab_bar_size(ui->tableview, LV_PCT(10));

    // 设置tableview里的标签tabar的背景位置
    lv_tabview_set_tab_bar_position(ui->tableview, LV_DIR_BOTTOM);
    // 初始化边框样式
    ui_container_style_init(ui->tableview);
    // 添加三个标签
    lv_obj_t *GPX_table = lv_tabview_add_tab(ui->tableview, "GPX");
    lv_obj_t *Genaral_table = lv_tabview_add_tab(ui->tableview, "Genaral");
    lv_obj_t *Setting_table = lv_tabview_add_tab(ui->tableview, "Setting");

    // //将table_bar的文本添加到翻译中
    // lv_label_set_text(GPX_table,lv_tr(tags[0]));
    // lv_label_set_text(Genaral_table,lv_tr(tags[1]));
    // lv_label_set_text(Setting_table,lv_tr(tags[2]));

    // 静态翻译模块
    // 1. 获取底部的标签栏大容器
    lv_obj_t *tab_bar = lv_tabview_get_tab_bar(ui->tableview);

    // 2.   给整个标签栏套上你的中文字体，解决豆腐块！
    lv_obj_set_style_text_font(tab_bar, &EN, 0);

    // 3.   使用动态注册函数，解决语言切换不变的问题
    lv_obj_t *GPX_label = lv_obj_get_child(lv_obj_get_child(tab_bar, 0), 0);
    ui_translation_register_label(GPX_label, tags[0]); // 动态绑定 "GPX"

    lv_obj_t *Genaral_label = lv_obj_get_child(lv_obj_get_child(tab_bar, 1), 0);
    ui_translation_register_label(Genaral_label, tags[1]); // 动态绑定 "General"

    lv_obj_t *Setting_label = lv_obj_get_child(lv_obj_get_child(tab_bar, 2), 0);
    ui_translation_register_label(Setting_label, tags[2]); // 动态绑定 "
    /****************************************************************/

    ui_general_tab_init(Genaral_table, ui);
    // 初始化设置页面
    ui_setting_tab_init(Setting_table, ui);
    // 初始化gpx页面
    ui_gpx_tab_init(GPX_table, ui);
}

void ui_create(void)
{

    // 🟡 第一步：先刷个纯黄色背景！
    // 如果烧录后屏幕是黄色，说明成功进入了函数，没在别的地方死掉。
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0xFFFF00), 0);
    lv_display_flush_ready(lv_display_get_default()); // 强制刷新一下

    // 1. 尝试分配内存（这里极容易触发单片机内存不足）
    ui = lv_malloc_core(sizeof(ui_t));
    if (ui == NULL)
    {
        // 🔴 变红！如果屏幕变成大红色，说明单片机 LVGL 的内存池不够了！分配失败！
        lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0xFF0000), 0);
        return; // 终止往下运行，防止死机
    }
    // 【关键】防止结构体里有随机垃圾数据导致野指针
    lv_memzero(ui, sizeof(ui_t));

    lv_obj_t *screen = init_screen();
    lv_obj_set_style_bg_color(screen, lv_color_hex(0xF4F0FF), 0);
    // 2. 静态文本与数据初始化
    lv_translation_add_static((const char **)languages, (const char **)tags, (const char **)translations);
    lv_translation_set_language(languages[0]);
    init_data(ui);

    // 🟣 第二步：数据初始化通过，屏幕变紫色！
    // 如果死机停在紫色，说明下一步的“状态栏”代码有毒。
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x800080), 0);

    // 3. 初始化状态栏
    init_status_bar(ui, screen);

    // 🔵 第三步：状态栏通过，屏幕变纯蓝色！
    // 如果死机停在蓝色，说明下一步的“TabView 和三个页面”代码有毒。
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x0000FF), 0);

    // 4. 初始化三个页面（这里最容易因为内存爆满而死机）
    init_tableview(ui, screen);

    // 🟢 终极胜利：全都活下来了，没有死机！屏幕变纯绿色！
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x00FF00), 0);
    ui_apply_theme(ui, 0);
    // 强制刷新屏幕样式
    lv_obj_invalidate(screen);
}

/*
 * @brief 更新电池状态
 *
 * @param ui  ui结构体指针
 * @param isCharging  是否充电
 * @param percent  百分比
 * */

// 电池图标数组
static const char *battery_symbols[] = {
    LV_SYMBOL_BATTERY_EMPTY,
    LV_SYMBOL_BATTERY_1,
    LV_SYMBOL_BATTERY_2,
    LV_SYMBOL_BATTERY_3,
    LV_SYMBOL_BATTERY_FULL,
    LV_SYMBOL_CHARGE // 充电标志
};

/**
 * @brief 更新电池状态
 * @param ui         UI结构体指针
 * @param isCharging 是否正在充电
 * @param percentage 电量百分比 (0-100)
 */
void ui_update_battery(ui_t *ui, bool isCharging, uint8_t percentage)
{
    // 1. 🚀 预留：更新百分比数字仓库 (哪怕现在还没绑定 Label)
    // 只要这一行在，以后你加了数字显示，它会自动同步
    lv_subject_set_int(&ui->data.battery_percentage, percentage);

    // 2. 核心：图标分层与充放电逻辑
    if (isCharging)
    {
        // 充电中：强制显示闪电图标
        lv_subject_set_pointer(&ui->data.battery_level, (void *)battery_symbols[5]);
    }
    else
    {
        // 根据百分比切换 0-4 级图标
        uint8_t index = percentage / 25;
        if (index > 4)
            index = 4;

        lv_subject_set_pointer(&ui->data.battery_level, (void *)battery_symbols[index]);
    }
}
// 主题切换
// 主题切换 (黑金模式保留，原版薄荷绿全部爆改为马卡龙紫)// 主题切换 (黑金模式 & 马卡龙紫完美适配版)
// 终极优化：深邃星空紫主题 (针对实体屏高对比度调校)// 终极主题控制：解决色差不统一、文字不可见、对比度不足问题
// 终极优化：深邃星空紫主题 (彻底消除白块 + 增强文字可见度)
// 终极优化：深邃星空紫主题 (针对实体屏高对比度调校)
// 终极优化：深邃星空紫主题 (彻底消除白块 + 增强文字可见度)
void ui_apply_theme(ui_t *ui, int mode)
{
    // ---------------------------------------------------------
    // 1. 核心色卡 (保持你喜欢的颜色)
    // ---------------------------------------------------------
    lv_color_t BG_COL = (mode == 1) ? lv_color_hex(0x000000) : lv_color_hex(0x1E1B2E);
    lv_color_t CARD_COL = (mode == 1) ? lv_color_hex(0x1A1A1A) : lv_color_hex(0x2D2844);
    lv_color_t BORDER_MAIN = (mode == 1) ? lv_color_hex(0xD4AF37) : lv_color_hex(0x9D4EDD);
    lv_color_t TEXT_MAIN = (mode == 1) ? lv_color_hex(0xFFFFFF) : lv_color_hex(0xE0AAFF);
    lv_color_t TEXT_ACCENT = (mode == 1) ? lv_color_hex(0xD4AF37) : lv_color_hex(0x00E5FF);
    // --- 电池专属色卡 (确保与背景有高对比度) ---
    // 电池图标颜色：黑金->琥珀金 | 深紫->薄荷绿
    lv_color_t BATT_ICON_COL = (mode == 1) ? lv_color_hex(0xFFBF00) : lv_color_hex(0x00FFAB);

    // 电池数字颜色：黑金->明黄色 | 深紫->冰晶蓝 (Cyan)
    lv_color_t BATT_TEXT_COL = (mode == 1) ? lv_color_hex(0xFFFF00) : lv_color_hex(0x00E5FF);
    // ---------------------------------------------------------
    // 2. 状态栏配置 (StatusBar)
    // ---------------------------------------------------------
    if (ui->staus_bar)
    {
        lv_obj_set_style_bg_color(ui->staus_bar, (mode == 1) ? lv_color_hex(0x121212) : lv_color_hex(0x12101B), 0);
        lv_obj_set_style_border_color(ui->staus_bar, BORDER_MAIN, 0);
        lv_obj_set_style_border_width(ui->staus_bar, 2, 0);

        if (ui->gps_label)
            lv_obj_set_style_text_color(ui->gps_label, TEXT_MAIN, 0);
        if (ui->run_status_label)
            lv_obj_set_style_text_color(ui->run_status_label, TEXT_ACCENT, 0);
        // 1. 刷电池图标的颜色 (Icon)
        if (ui->battery_label)
        {
            lv_obj_set_style_text_color(ui->battery_label, BATT_ICON_COL, 0);
        }

        // 2. 刷电池百分比数字的颜色 (Text)
        if (ui->battery_label_num)
        {
            lv_obj_set_style_text_color(ui->battery_label_num, BATT_TEXT_COL, 0);
        }
    }

    // ---------------------------------------------------------
    // 3. 通用界面 (General Tab)
    // ---------------------------------------------------------
    lv_obj_t *gen_cards[] = {ui->gen_speed_obj, ui->gen_dist_obj, ui->gen_slope_obj, ui->gen_time_obj, ui->gen_sat_obj};
    for (int i = 0; i < 5; i++)
    {
        if (gen_cards[i])
        {
            lv_obj_set_style_bg_color(gen_cards[i], CARD_COL, 0);
            lv_obj_set_style_border_color(gen_cards[i], BORDER_MAIN, 0);
            lv_obj_set_style_border_width(gen_cards[i], 2, 0);
            lv_obj_set_style_radius(gen_cards[i], 10, 0);
        }
    }
    // 统一 General 标签颜色，确保不突兀
    if (ui->gen_speed_label)
        lv_obj_set_style_text_color(ui->gen_speed_label, TEXT_ACCENT, 0);
    if (ui->gen_dist_label)
        lv_obj_set_style_text_color(ui->gen_dist_label, TEXT_MAIN, 0);
    if (ui->gen_slope_label)
        lv_obj_set_style_text_color(ui->gen_slope_label, TEXT_MAIN, 0);
    if (ui->gen_hour_label)
        lv_obj_set_style_text_color(ui->gen_hour_label, TEXT_MAIN, 0);
    if (ui->gen_min_label)
        lv_obj_set_style_text_color(ui->gen_min_label, TEXT_MAIN, 0);
    if (ui->gen_long_label)
        lv_obj_set_style_text_color(ui->gen_long_label, TEXT_MAIN, 0);
    if (ui->gen_lat_label)
        lv_obj_set_style_text_color(ui->gen_lat_label, TEXT_MAIN, 0);
    if (ui->gen_second_label)
    {
        // 跟着分钟和小时一起变色
        lv_obj_set_style_text_color(ui->gen_second_label, TEXT_MAIN, 0);
    }

    // ---------------------------------------------------------
    // 4. 路书界面 (GPX Tab) - 【重点修复：里程方块对齐】
    // ---------------------------------------------------------
    // 速度方块与里程方块样式完全一致化
    lv_obj_t *gpx_boxes[] = {ui->gpx_speed_obj, ui->gpx_mile_obj};
    lv_obj_t *gpx_labels[] = {ui->gpx_speed_label, ui->gpx_mile_label};

    for (int i = 0; i < 2; i++)
    {
        if (gpx_boxes[i])
        {
            lv_obj_set_style_bg_color(gpx_boxes[i], CARD_COL, 0);
            lv_obj_set_style_border_color(gpx_boxes[i], BORDER_MAIN, 0);
            lv_obj_set_style_border_width(gpx_boxes[i], 3, 0);
            lv_obj_set_style_radius(gpx_boxes[i], 12, 0); // 统一圆角
        }
        if (gpx_labels[i])
        {
            lv_obj_set_style_text_color(gpx_labels[i], TEXT_ACCENT, 0);
        }
    }

    if (ui->gpx_map_obj)
        lv_obj_set_style_bg_color(ui->gpx_map_obj, BG_COL, 0);
    if (ui->map_args.center_point)
        lv_obj_set_style_bg_color(ui->map_args.center_point, TEXT_ACCENT, 0);
    if (ui->gpx_slider)
    {
        lv_obj_set_style_bg_color(ui->gpx_slider, CARD_COL, LV_PART_MAIN);
        lv_obj_set_style_bg_color(ui->gpx_slider, BORDER_MAIN, LV_PART_INDICATOR);
    }

    // ---------------------------------------------------------
    // 5. 设置界面 (Setting Tab) - 【重点修复：文字亮度】
    // ---------------------------------------------------------
    lv_obj_t *set_cons[] = {ui->set_lang_con, ui->set_dark_con};
    lv_obj_t *set_labels[] = {ui->set_lang_label, ui->set_dark_label};

    for (int i = 0; i < 2; i++)
    {
        if (set_cons[i])
        {
            lv_obj_set_style_bg_color(set_cons[i], CARD_COL, 0);
            lv_obj_set_style_border_color(set_cons[i], BORDER_MAIN, 0);
            lv_obj_set_style_border_width(set_cons[i], 2, 0);
        }
        // 核心修复：强制设置标签颜色为 TEXT_MAIN (亮淡紫色)，彻底解决看不清的问题
        if (set_labels[i])
        {
            lv_obj_set_style_text_color(set_labels[i], TEXT_MAIN, 0);
        }
    }

    if (ui->set_dropdown)
    {
        lv_obj_set_style_bg_color(ui->set_dropdown, CARD_COL, 0);
        lv_obj_set_style_border_color(ui->set_dropdown, BORDER_MAIN, 0);
        lv_obj_set_style_text_color(ui->set_dropdown, TEXT_ACCENT, 0);
        // 下拉列表展开后的背景
        lv_obj_t *list = lv_dropdown_get_list(ui->set_dropdown);
        if (list)
        {
            lv_obj_set_style_bg_color(list, CARD_COL, 0);
            lv_obj_set_style_text_color(list, TEXT_MAIN, 0);
        }
    }

    // ---------------------------------------------------------
    // 6. 底部导航栏 (Tab Bar)
    // ---------------------------------------------------------
    if (ui->tableview)
    {
        lv_obj_set_style_bg_color(ui->tableview, BG_COL, 0);
        lv_obj_t *tab_bar = lv_tabview_get_tab_bar(ui->tableview);
        if (tab_bar)
        {
            lv_color_t tab_bg = (mode == 1) ? lv_color_hex(0x0D0D0D) : lv_color_hex(0x12101B);
            lv_obj_set_style_bg_color(tab_bar, tab_bg, 0);
            lv_obj_set_style_border_side(tab_bar, LV_BORDER_SIDE_TOP, 0);
            lv_obj_set_style_border_color(tab_bar, BORDER_MAIN, 0);
            lv_obj_set_style_border_width(tab_bar, 2, 0);

            uint32_t btn_cnt = lv_obj_get_child_count(tab_bar);
            for (uint32_t i = 0; i < btn_cnt; i++)
            {
                lv_obj_t *btn = lv_obj_get_child(tab_bar, i);
                lv_obj_set_style_bg_color(btn, tab_bg, 0);
                lv_obj_set_style_text_color(btn, lv_color_hex(0x5A537D), 0); // 未选中：深灰紫
                lv_obj_set_style_text_color(btn, TEXT_ACCENT, LV_STATE_CHECKED);
            }
        }
    }
}
/* --- 在 ui.c 文件末尾添加 --- */
int current_theme_mode = 0; // 定义全局变量：0-深紫，1-黑金

// 这个函数是给 LVGL 任务跑的，用来执行真正的换肤
void ui_toggle_theme_wrapper(void * arg)
{
    current_theme_mode = !current_theme_mode; // 翻转模式
    ui_apply_theme(ui, current_theme_mode);   // 调用你之前的颜色切换函数
}