// src/display_driver.cpp
#include "display_driver.h"
#include <Arduino.h>

// Forward declaration to avoid including gfx_conf.h directly
class LGFX;
extern LGFX tft;
extern int screenWidth;
extern int screenHeight;
extern void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
extern void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);

// Define the static members
lv_disp_draw_buf_t DisplayDriver::_draw_buf;
lv_disp_drv_t DisplayDriver::_disp_drv;

void DisplayDriver::init() {
    // Initialize LVGL
    lv_init();
    delay(100);
    
    // Allocate display buffers
    static lv_color_t disp_draw_buf1[800 * 480 / 10]; // Hardcoded for safety
    static lv_color_t disp_draw_buf2[800 * 480 / 10]; // Hardcoded for safety

    // LVGL display buffer
    lv_disp_draw_buf_init(&_draw_buf, disp_draw_buf1, disp_draw_buf2, 800 * 480 / 10);
    
    // Configure display driver
    lv_disp_drv_init(&_disp_drv);
    _disp_drv.hor_res       = 480; // Hardcoded for safety
    _disp_drv.ver_res       = 800; // Hardcoded for safety
    _disp_drv.flush_cb      = my_disp_flush;
    _disp_drv.full_refresh  = 0;    // only redraw dirty regions
    _disp_drv.draw_buf      = &_draw_buf;
    lv_disp_drv_register(&_disp_drv);

    // Configure input driver
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type    = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);
}