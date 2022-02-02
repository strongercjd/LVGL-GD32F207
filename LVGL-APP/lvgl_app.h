#ifndef __LVGL_APP_H 
#define __LVGL_APP_H
#include <stdio.h>
#ifdef GD32F207debug
  #include "lvgl.h"
  #include "lv_port_disp.h"
  #include "lv_port_indev.h"
#else
  #include "../Third_Party/LVGL/lvgl/lvgl.h"
  #include "lv_drivers/display/monitor.h"
  #include "lv_drivers/indev/mouse.h"
  #include "lv_drivers/indev/mousewheel.h"
  #include "lv_drivers/indev/keyboard.h"
#endif

void lvgl_app_create();

#endif
