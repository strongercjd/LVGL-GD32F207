#include "lvgl_app.h"

extern lv_indev_t * indev_keypad;
lv_obj_t * scr;
lv_style_t style1;
lv_style_t style2;
lv_obj_t * obj1 = NULL;
lv_obj_t * obj2 = NULL;

static void event_cb(lv_event_t * e)
{
  LV_LOG_USER("Clicked");//日志打印
  
  static uint32_t cnt = 1;//计数
  lv_obj_t * btn = lv_event_get_target(e);//获取点击的控件
  lv_obj_t * label = lv_obj_get_child(btn, 0);//获取点击控件的子控件，也就是labl控件
  lv_label_set_text_fmt(label, "%"LV_PRIu32, cnt);//修改labl控件上的文字
  if(cnt == 1)
  {//将obj1移动到上一层
    //    lv_obj_move_foreground(obj1);
    lv_obj_move_background(obj2);
  }
  if(cnt == 2)
  {
    lv_obj_del(obj1);
  }
  if(cnt == 3)
  {
    lv_obj_del(obj2);
  }
  
  cnt++;
}
void lvgl_app_create()
{
  scr = lv_scr_act();//获取当前活跃的屏幕对象
  lv_obj_t * btn = lv_btn_create(scr);//在当前活跃的屏幕上新建一个按钮
  lv_obj_set_size(btn, 100, 32);//设置按钮大小
  lv_obj_set_align(btn, LV_ALIGN_BOTTOM_RIGHT);//设置按钮对齐位置，也就是设置按钮的位置
  lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, NULL);//按钮添加单击事件回调函数
  lv_obj_t * label = lv_label_create(btn);//在按钮上新建labl标签
  lv_label_set_text(label, "Click me!");//在标签上显示文字
  lv_obj_center(label);//标签文字居中对齐
  
  lv_style_init(&style1);//初始化样式
  lv_style_set_bg_color(&style1, lv_palette_main(LV_PALETTE_GREEN));//设置样式的为greeen
  
  //创建一个基本对象1
  obj1 = lv_obj_create(scr);
  lv_obj_set_pos(obj1,20,20);//设置坐标位置
  lv_obj_set_size(obj1,100,100);//设置大小
  lv_obj_add_style(obj1, &style1, 0);
  
  //创建一个基本对象2,与对象1进行外部底下居中对齐,同时y轴向上偏移10个像素,
  //目的是为了让obj2有一部分压在obj1上,方便后面演示z轴层级改变的API接口
  obj2 = lv_obj_create(scr);
  lv_obj_set_size(obj2,50,50);
  lv_style_init(&style2);
  lv_style_set_bg_color(&style2, lv_palette_main(LV_PALETTE_RED));
  lv_obj_add_style(obj2,&style2,0);//设置新的样式
  lv_obj_align_to(obj2,obj1,LV_ALIGN_OUT_BOTTOM_MID,0,-20);//对齐，对象2以对象1为参考
  
  
  lv_group_t * group = lv_group_create();//创建组
  lv_group_add_obj(group, btn);//将按键添加到组
  lv_indev_set_group(indev_keypad, group);//绑定按钮到组
  
}
