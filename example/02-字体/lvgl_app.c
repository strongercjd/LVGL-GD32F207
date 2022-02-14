#include "lvgl_app.h"

LV_FONT_DECLARE(test_font);
void lvgl_app_create()
{  
    lv_obj_t * label1 = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP);     /*Break the long lines*/
    lv_obj_set_style_text_font(label1, &test_font, 0);
    lv_label_set_recolor(label1, true);                      /*通过文本中的命令启用重新着色*/
    lv_label_set_text(label1, "#0000ff 陈##ff00ff 陈##ff0000 B#CD");
    lv_obj_set_width(label1, 150);  /*设置labl宽度*/
    lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, -40);
}
