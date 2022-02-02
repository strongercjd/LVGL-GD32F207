#include "lvgl_app.h"


extern lv_indev_t * indev_keypad;

static void my_event_cb(lv_event_t * event)
{
    printf("lab1 Clicked\n");
}
static void my_event_cb1(lv_event_t * event)
{
    printf("labe2 Clicked\n");
}
void lvgl_app_create()
{
  lv_obj_t * obj = lv_obj_create(lv_scr_act());
  lv_obj_set_size(obj, LV_PCT(40), LV_PCT(10));
  lv_obj_align(obj, LV_ALIGN_TOP_LEFT, 0, 0);
  
  lv_obj_t * label = lv_label_create(obj);
  lv_label_set_text(label, "HELLO,LVGL!");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_add_event_cb(label, my_event_cb, LV_EVENT_CLICKED, NULL);
  
  lv_obj_t * obj1 = lv_obj_create(lv_scr_act());
  lv_obj_set_size(obj1, LV_PCT(20), LV_PCT(10));
  lv_obj_align(obj1, LV_ALIGN_CENTER, 0, 0);
  lv_obj_t * labe2 = lv_label_create(obj1);
  lv_label_set_text(labe2, "123456!");
  lv_obj_align(labe2, LV_ALIGN_CENTER, 0, 0);
  lv_obj_add_event_cb(labe2, my_event_cb1, LV_EVENT_CLICKED, NULL);
  
  lv_group_t * group = lv_group_create();
  lv_group_add_obj(group, label);
  lv_group_add_obj(group, labe2);
  lv_indev_set_group(indev_keypad, group);
}
