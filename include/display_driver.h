// include/display_driver.h
#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#include <lvgl.h>

class DisplayDriver {
public:
    static void init();
    
private:
    static lv_disp_draw_buf_t _draw_buf;
    static lv_disp_drv_t _disp_drv;
};

#endif // DISPLAY_DRIVER_H