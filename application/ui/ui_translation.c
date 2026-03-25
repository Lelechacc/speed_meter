#include "ui_translation.h"



    
// 1. 严格只留 2 个语言，最后加上 NULL 保命
const char * const languages[] = {"en", "cn", NULL};

// 2. 标签定义
const char * const tags[] = {"GPX", "Genaral", "Setting", "Language", "Dark Mode", NULL};

// 3. 翻译矩阵（每一行必须严格对应 en 和 cn 两个词，最后必须带 NULL）
const char * const translations[] = {
    "GPX", "路书",
    "Genaral", "通用",
    "Setting", "设置",
    "Language", "语言",
    "Dark Mode", "黑金模式",  
    NULL // <--- 绝对不能漏掉这个！
};


void on_language_change(lv_event_t *e)
{
    lv_obj_t *label = lv_event_get_target(e);
    const char *tag = (const char *)lv_event_get_user_data(e);
    lv_label_set_text(label, lv_tr(tag));

}

void ui_translation_register_label(lv_obj_t *label, const char *tag)
{
    // 绑定标签文本
    lv_label_set_text(label, lv_tr(tag));
    // 添加回调
    // lv_obj_add_event_cb(label, on_language_change, LV_EVENT_TRANSLATION_LANGUAGE_CHANGED, tag);
    lv_obj_add_event_cb(label, on_language_change, LV_EVENT_TRANSLATION_LANGUAGE_CHANGED, (void *)tag);

} 
