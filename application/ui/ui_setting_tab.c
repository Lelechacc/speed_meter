#include "ui_setting_tab.h"
/* ========================================================== */
/* ============== 夏日薄荷糖风格强化 ================= */
/* ========================================================== */
// 下拉列表的语言选项
static char *language_list = "English\n"
                             "中文\n"
                            ;

/********************************************************* */

static void ui_dark_mode_switch_event(lv_event_t *e)
{
    ui_t *ui_ptr = (ui_t *)lv_event_get_user_data(e); // 获取传进来的全局ui指针
    lv_obj_t *swuth_t = lv_event_get_target_obj(e);

    // 获取开关状态
    bool checked = lv_obj_has_state(swuth_t, LV_STATE_CHECKED);

    if (checked)
    {
        ui_apply_theme(ui_ptr, 1); // 1 代表黑金模式
        printf("Mode: Extreme Black Gold\n");
    }
    else
    {
        ui_apply_theme(ui_ptr, 0); // 0 代表原版薄荷
        printf("Mode: Summer Mint\n");
    }
}
/********************************************************* */
void Dark_mode_switch_event(lv_event_t *e)
{
    // 获取当前开关对象
    lv_obj_t *swuth_t = lv_event_get_target_obj(e);
    // 获取当前开关对象的状态
    bool checked = lv_obj_has_state(swuth_t, LV_STATE_CHECKED);
    // 获取user_data
    ui_t *ui = lv_event_get_user_data(e);
    if (checked)
    {
        printf("ok checked\n");
        // // 添加暗色模式
        // lv_obj_add_state(ui->tableview, LV_STATE_CHECKED);
        // // 添加暗色模式
        // lv_obj_add_state(ui->staus_bar, LV_STATE_CHECKED);
        // // 添加暗色模式
    }
    else
    {
        printf("off\n");
        // // 移除暗色模式
        // lv_obj_remove_state(ui->tableview, LV_STATE_CHECKED);
        // // 移除暗色模式
        // lv_obj_remove_state(ui->staus_bar, LV_STATE_CHECKED);
    }
}

void ui_language_dropdown_event_cb(lv_event_t *e)
{
    lv_obj_t *dropdown = lv_event_get_target_obj(e);
    // 1. 获取选中的索引（数字），别用那个报错的 get_text
    uint16_t selected_index = lv_dropdown_get_selected(dropdown);    
    
    // 2.  必须加这行声明，否则编译器不认识 languages！
    extern const char * const languages[]; 
    
    // 3. 应用语言
    lv_translation_set_language(languages[selected_index]);
}

void ui_setting_tab_init(lv_obj_t *parent, ui_t *ui)

{
    // 设为弹性布局
    lv_obj_set_layout(parent, LV_LAYOUT_FLEX);
    // 设置弹性布局的子元素排列方式为垂直
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    // 布局格式设置
    //  缩小父容器边距，让小框扩容
    // 内边距
    lv_obj_set_style_pad_all(parent, 4, 0);
    //  行间距
    lv_obj_set_style_pad_row(parent, 10, 0);
    // 列间距
    lv_obj_set_style_pad_column(parent, 10, 0);
    // 边框宽度
    lv_obj_set_style_border_width(parent, 0, 0);
    // 圆角
    lv_obj_set_style_radius(parent, 0, 0);
    // 设置 parent 背景为奶油薄荷色 (夏日清爽底色)
    // lv_obj_set_style_bg_color(parent, lv_color_hex(0xF4F0FF), 0);
    /************************************** */
    // 对其方式左对齐，子元素也左对齐
    lv_obj_set_flex_align(parent, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

    /************************************** */
    /************************************** */

    // 1.创建一个语言容器
    lv_obj_t *language_container = lv_obj_create(parent);
    ui->set_lang_con = language_container;
    lv_obj_clear_flag(language_container, LV_OBJ_FLAG_SCROLLABLE); // 禁止滑动

    // 设为弹性布局
    lv_obj_set_layout(language_container, LV_LAYOUT_FLEX);
    // 子元素排列方式为水平
    lv_obj_set_flex_flow(language_container, LV_FLEX_FLOW_ROW);

    // 布局格式设置
    // 内边距
    lv_obj_set_style_pad_all(language_container, 10, 0);
    // 行间距
    lv_obj_set_style_pad_row(language_container, 3, 0);
    // 列间距
    lv_obj_set_style_pad_column(language_container, 3, 0);
    // // 边框宽度
    lv_obj_set_style_border_width(language_container, 3, 0);

    // 圆角
    lv_obj_set_style_radius(language_container, 12, 0);

    // // 增加糖果感的
    // lv_obj_set_style_border_color(language_container, lv_color_hex(0xA06CD5), 0); // 深青色边框
    // lv_obj_set_style_bg_color(language_container, lv_color_hex(0xFFFFFF), 0);     // 纯白卡片底，更醒目

    // // 设置背景颜色
    // lv_obj_set_style_bg_color(language_container, lv_color_hex(0xF8EDEB), 0);
    // 设置容器大小
    lv_obj_set_size(language_container, LV_PCT(100), LV_SIZE_CONTENT);

    /************************************** */
    // 填写语言容器的标签内容
    lv_obj_t *language_label = lv_label_create(language_container);
    ui->set_lang_label = language_label;
    // 设置标签内容
    // lv_label_set_text(language_label, "Language");
    ui_translation_register_label(language_label, tags[3]);
    // 设置标签字体
    
    // lv_obj_set_style_text_font(language_label, &lv_font_montserrat_16, 0);
    // // 强化文本颜色 (深海蓝，提高阅读对比度)
    // lv_obj_set_style_text_color(language_label, lv_color_hex(0x2D1B4E), 0);
    // ：不抢空间，文字多宽它就多宽
    lv_obj_set_width(language_label, LV_SIZE_CONTENT);
    lv_obj_set_flex_grow(language_label, 0);
    //  把文字往下拉一点（推荐）
    lv_obj_set_style_translate_y(language_label, 8, 0);
    //  使用EN字体
    lv_obj_set_style_text_font(language_label, &EN, 0);
    /************************************** */
    // 创建下拉列表
    lv_obj_t *language_dropdown = lv_dropdown_create(language_container);
    ui->set_dropdown = language_dropdown;
    // 设置下拉列表的选项为语言列表
    lv_dropdown_set_options(language_dropdown, language_list);
    // 设置下拉列表的显示方向为向下
    lv_dropdown_set_dir(language_dropdown, LV_DIR_BOTTOM);
    // 设置下拉列表的符号为向下
    lv_dropdown_set_symbol(language_dropdown, LV_SYMBOL_DOWN);

    // // 下拉列表(Dropdown)美化
    // lv_obj_set_style_border_color(language_dropdown, lv_color_hex(0xA06CD5), 0); // 边框一致
    // lv_obj_set_style_text_color(language_dropdown, lv_color_hex(0x7A4CBA), 0);   // 选项文字用亮蓝色
    // 如果下拉框里面的文字也要改，别忘了它
    // lv_obj_set_style_text_font(language_dropdown, &EN, 0);
    // 设置下拉列表打开后的高亮色 (选中项变蓝)
    // lv_obj_set_style_bg_color(language_dropdown, lv_color_hex(0x9BF6FF), LV_PART_SELECTED | LV_STATE_CHECKED);
    // 下拉框：抢占所有剩下的地盘
    lv_obj_set_flex_grow(language_dropdown, 1);
    /************************************** */
    //添加下拉菜单的回调
    lv_obj_add_event_cb(language_dropdown, ui_language_dropdown_event_cb, LV_EVENT_VALUE_CHANGED, ui);

    /************************************** */

    /************************************** */
    /************************************** */

    // 创建显示模式的容器
    lv_obj_t *display_mode_container = lv_obj_create(parent);
    ui->set_dark_con = display_mode_container;
    // 设为弹性布局

    // 修正布局：确保该容器也是水平排列 (ROW)
    lv_obj_set_layout(display_mode_container, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(display_mode_container, LV_FLEX_FLOW_ROW);
    // 【核心对齐】：左右推开，垂直居中
    lv_obj_set_flex_align(display_mode_container, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    // 布局格式设置
    // 糖果感样式
    // lv_obj_set_style_border_color(display_mode_container, lv_color_hex(0xA06CD5), 0);
    // 行间距
    lv_obj_set_style_pad_row(display_mode_container, 10, 0);
    // 列间距
    lv_obj_set_style_pad_column(display_mode_container, 10, 0);
    // 边框宽度
    lv_obj_set_style_border_width(display_mode_container, 3, 0);

    // 圆角
    lv_obj_set_style_radius(display_mode_container, 12, 0);

    // 设置背景颜色
    // lv_obj_set_style_bg_color(display_mode_container, lv_color_hex(0xFFFFFF), 0);

    // 文字颜色
    // lv_obj_set_style_text_color(display_mode_container, lv_color_hex(0x2D1B4E), 0);
    // 设置容器大小
    lv_obj_set_size(display_mode_container, LV_PCT(100), LV_SIZE_CONTENT);

    /************************************** */
    // 从Display页面创建模式显示设置容器的创建显示模式的标签
    lv_obj_t *Dark_mode_label = lv_label_create(display_mode_container);
    ui->set_dark_label = Dark_mode_label;
    // 填写显示模式的标签内容
    // lv_label_set_text(Dark_mode_label, "Dark Mode");
    lv_label_set_text(Dark_mode_label,lv_tr(tags[4]));
    // 设置标签字体
    // lv_obj_set_style_text_font(Dark_mode_label, &lv_font_montserrat_16, 0);
    /************************************** */
    // 创建一个开关按钮
    lv_obj_t *Dark_mode_switch = lv_switch_create(display_mode_container);
    // 开关(Switch)样式强化 ---
    // 给黑夜模式标签设置字体
    // lv_obj_set_style_text_font(Dark_mode_label, &EN, 0);
    ui_translation_register_label(Dark_mode_label, tags[4]);
    
    //  文字颜色
    // lv_obj_set_style_text_color(Dark_mode_label, lv_color_hex(0x2D1B4E), 0);
    // 设置开关开启时的颜色 (海蓝色)
    // lv_obj_set_style_bg_color(Dark_mode_switch, lv_color_hex(0x219EBC), LV_PART_INDICATOR | LV_STATE_CHECKED);
    // 设置开关中间圆钮的颜色 (奶油黄，增加多巴胺色彩感)
    // lv_obj_set_style_bg_color(Dark_mode_switch, lv_color_hex(0xFDFFB6), LV_PART_KNOB);

    /************************************** */
    // 深色模式
    // 添加开关的回调函数处理
    lv_obj_add_event_cb(Dark_mode_switch, ui_dark_mode_switch_event, LV_EVENT_VALUE_CHANGED, ui);
    lv_obj_add_flag(Dark_mode_switch, LV_OBJ_FLAG_EVENT_BUBBLE);
}

