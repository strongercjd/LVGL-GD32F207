#include "lvgl_app.h"

extern lv_indev_t * indev_keypad;

static void event_cb(lv_event_t * e)
{
  LV_LOG_USER("Clicked");
  
  static uint32_t cnt = 1;
  lv_obj_t * btn = lv_event_get_target(e);
  lv_obj_t * label = lv_obj_get_child(btn, 0);
  lv_label_set_text_fmt(label, "%"LV_PRIu32, cnt);
  cnt++;
}
void lvgl_app_create()
{
  
  lv_obj_t * btn = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn, 100, 50);
  lv_obj_center(btn);
  lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_t * label = lv_label_create(btn);
  lv_label_set_text(label, "Click me!");
  lv_obj_center(label);
  
  lv_obj_t * btn2 = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn2, 100, 50);
  lv_obj_add_event_cb(btn2, event_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_t * label2 = lv_label_create(btn2);
  lv_label_set_text(label2, "Click me!");
  lv_obj_center(label2);
  
  lv_group_t * group = lv_group_create();
  lv_group_add_obj(group, btn);
  lv_group_add_obj(group, btn2);
  lv_indev_set_group(indev_keypad, group);
  
}
