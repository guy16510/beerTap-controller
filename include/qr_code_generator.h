// include/qr_code_generator.h
#ifndef QR_CODE_GENERATOR_H
#define QR_CODE_GENERATOR_H

#include <lvgl.h>

class QRCodeGenerator {
public:
    static void init(lv_obj_t* container);
    static void cleanup();
    
private:
    static lv_obj_t* _qrCodeObj;
    static lv_obj_t* _backgroundObj;
    static lv_obj_t* _container;
    static void delayedGenerate(lv_timer_t* timer);
};

#endif // QR_CODE_GENERATOR_H