#ifndef QR_CODE_HELPER_H
#define QR_CODE_HELPER_H

#include <lvgl.h>
#include <lv_qrcode.h>
#include <qrcodegen.h>

class QRCodeHelper {
public:
    // Initialize helper
    static void begin();
    
    // Call when QR Code screen is loaded
    static void onScreenLoad(lv_event_t* e);
    
    // Generate QR code (called via timer)
    static void generate(const char* payload = "https://example.com");
    
    // Set container object reference
    static void setContainer(lv_obj_t* container);

private:
    static lv_obj_t* _container;     // Container for QR code
    static lv_obj_t* _background;    // Background object
    static lv_obj_t* _qrcode;        // QR code object
    
    // Internal method for delayed QR code generation
    static void _delayedGenerate(lv_timer_t* timer);
};

#endif // QR_CODE_HELPER_H