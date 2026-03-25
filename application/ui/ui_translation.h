#ifndef __UI_TRANSLATION_H__
#define __UI_TRANSLATION_H__

#include "ui_conf.h"
#include "lvgl.h"


extern const char * const languages[];
extern const char * const tags[];
extern const char * const translations[];
 //注册回调函数
 
void ui_translation_register_label(lv_obj_t *label,const char *tag);


#endif /* __UI_TRANSLATION_H__ */