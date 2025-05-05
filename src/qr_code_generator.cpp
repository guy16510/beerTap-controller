// src/qr_code_generator.cpp
#include "qr_code_generator.h"
#include <lv_qrcode.h>

lv_obj_t* QRCodeGenerator::_qrCodeObj = nullptr;
lv_obj_t* QRCodeGenerator::_backgroundObj = nullptr;
lv_obj_t* QRCodeGenerator::_container = nullptr;

void QRCodeGenerator::init(lv_obj_t* container) {
    _container = container;
    if (_container) {
        lv_timer_t* qr_timer = lv_timer_create(delayedGenerate, 50, NULL);
        lv_timer_set_repeat_count(qr_timer, 1);
    }
}

void QRCodeGenerator::cleanup() {
    if (_qrCodeObj) {
        lv_obj_del(_qrCodeObj);
        _qrCodeObj = nullptr;
    }
    
    if (_backgroundObj) {
        lv_obj_del(_backgroundObj);
        _backgroundObj = nullptr;
    }
}

void QRCodeGenerator::delayedGenerate(lv_timer_t* timer) {
    if (!_container) return;
    
    const char *payload = "https://example.com";
    
    // Get container size, fallback to defaults
    lv_coord_t w = lv_obj_get_width(_container);
    lv_coord_t h = lv_obj_get_height(_container);
    if (w <= 0 || h <= 0) { w = 480; h = 560; }
    
    // Remove previous
    cleanup();
    
    // Background
    _backgroundObj = lv_obj_create(_container);
    lv_obj_remove_style_all(_backgroundObj);
    lv_obj_set_size(_backgroundObj, w, h);
    lv_obj_set_style_bg_color(_backgroundObj, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_center(_backgroundObj);
    
    // QR code
    _qrCodeObj = lv_qrcode_create(_container, w, lv_color_black(), lv_color_white());
    lv_obj_center(_qrCodeObj);
    lv_qrcode_update(_qrCodeObj, payload, strlen(payload));
}