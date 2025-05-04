#include "qr_code_helper.h"
#include <Arduino.h>

// Initialize static members
lv_obj_t* QRCodeHelper::_container = nullptr;
lv_obj_t* QRCodeHelper::_background = nullptr;
lv_obj_t* QRCodeHelper::_qrcode = nullptr;

void QRCodeHelper::begin() {
    // Nothing needed for initialization
}

void QRCodeHelper::setContainer(lv_obj_t* container) {
    _container = container;
}

void QRCodeHelper::onScreenLoad(lv_event_t* e) {
    (void)e;
    Serial.println("[QR Code] Screen load event triggered");
    
    // Create a timer to generate the QR code after a brief delay
    // This allows the screen to load first
    lv_timer_t* qr_timer = lv_timer_create(_delayedGenerate, 50, NULL);
    lv_timer_set_repeat_count(qr_timer, 1);
}

void QRCodeHelper::generate(const char* payload) {
    if (!_container) {
        Serial.println("[QR Code] Error: No container set");
        return;
    }

    // Get container size, fallback to defaults
    lv_coord_t w = lv_obj_get_width(_container);
    lv_coord_t h = lv_obj_get_height(_container);
    if (w <= 0 || h <= 0) { 
        w = 480; 
        h = 560; 
    }

    // Remove previous QR code if it exists
    if (_background) lv_obj_del(_background);
    if (_qrcode) lv_obj_del(_qrcode);

    // Create a white background
    _background = lv_obj_create(_container);
    lv_obj_remove_style_all(_background);
    lv_obj_set_size(_background, w, h);
    lv_obj_set_style_bg_color(_background, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_center(_background);

    // Create the QR code
    _qrcode = lv_qrcode_create(_container, w, lv_color_black(), lv_color_white());
    lv_obj_center(_qrcode);
    lv_qrcode_update(_qrcode, payload, strlen(payload));

    Serial.printf("[QR Code] Generated %dx%d QR\n", w, w);
}

void QRCodeHelper::_delayedGenerate(lv_timer_t* timer) {
    (void)timer;
    // Default payload if none is provided
    generate();
}