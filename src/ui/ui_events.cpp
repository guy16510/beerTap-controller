// src/ui/ui_events.cpp
#include "ui.h"
#include "mpu6050_helper.h"
#include "beer_pour_helper.h"
#include <lvgl.h>
#include <Arduino.h>
#include "gfx_conf.h"
#include <lv_qrcode.h>

extern LGFX tft;

//—— QR Code widget handle & generator ——//
static lv_obj_t* ui_QRCodeCode = nullptr;
static lv_obj_t* ui_QRCodeBackground = nullptr;
static void delayed_qr_code_gen(lv_timer_t* timer);

// Safe UI element access with null checks
#define UI_SAFE_SET_TEXT(obj, text) do { if(obj != nullptr) lv_label_set_text(obj, text); } while(0)
#define UI_SAFE_SET_VALUE(obj, val, anim) do { if(obj != nullptr) lv_bar_set_value(obj, val, anim); } while(0)

void on_QRCodeScreen_load(lv_event_t* e) {
    (void)e;
    Serial.println("[QR Code] Screen load event triggered");
    lv_timer_t* qr_timer = lv_timer_create(delayed_qr_code_gen, 50, NULL);
    lv_timer_set_repeat_count(qr_timer, 1);
}

static void delayed_qr_code_gen(lv_timer_t* timer) {
    (void)timer;
    const char *payload = "https://example.com/beertap";

    if (!ui_QRCodeContainer) {
        Serial.println("[QR Code] Container not found, skipping");
        return;
    }

    // Get container size, fallback to defaults
    lv_coord_t w = lv_obj_get_width(ui_QRCodeContainer);
    lv_coord_t h = lv_obj_get_height(ui_QRCodeContainer);
    if (w <= 0 || h <= 0) { 
        w = 240; 
        h = 240; 
    }

    // Remove previous
    if (ui_QRCodeBackground) lv_obj_del(ui_QRCodeBackground);
    if (ui_QRCodeCode) lv_obj_del(ui_QRCodeCode);

    // Background
    ui_QRCodeBackground = lv_obj_create(ui_QRCodeContainer);
    if (!ui_QRCodeBackground) {
        Serial.println("[QR Code] Failed to create background");
        return;
    }
    
    lv_obj_remove_style_all(ui_QRCodeBackground);
    lv_obj_set_size(ui_QRCodeBackground, w, h);
    lv_obj_set_style_bg_color(ui_QRCodeBackground, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_center(ui_QRCodeBackground);

    // QR code
    ui_QRCodeCode = lv_qrcode_create(ui_QRCodeContainer, w - 20, lv_color_black(), lv_color_white());
    if (!ui_QRCodeCode) {
        Serial.println("[QR Code] Failed to create QR code");
        return;
    }
    
    lv_obj_center(ui_QRCodeCode);
    lv_qrcode_update(ui_QRCodeCode, payload, strlen(payload));

    Serial.printf("[QR Code] Generated %dx%d QR\n", w, h);
}

// WiFi stubs (since WiFi is removed)
void start_wifi_scan(lv_event_t* e) {
    Serial.println("[WiFi] Manual scan requested (stub)");
}

void selectedWifiSSID(lv_event_t* e) {
    Serial.println("[WiFi] Selected SSID (stub)");
}

void saveWifiConfiguration(lv_event_t* e) {
    Serial.println("[WiFi] Save configuration (stub)");
}

void deleteWiFiConfig(lv_event_t* e) {
    Serial.println("[WiFi] Delete configuration (stub)");
}

void checkWifiStatus(lv_event_t* e) {
    Serial.println("[WiFi] Status check (stub)");
    
    // Display a placeholder status symbol for WiFi icons
    const char* status_symbol = "○"; // Disconnected symbol
    
    // Update all status indicators safely
    UI_SAFE_SET_TEXT(ui_wifiLabelisConnected1, status_symbol);
    UI_SAFE_SET_TEXT(ui_wifiLabelisConnected3, status_symbol);
    UI_SAFE_SET_TEXT(ui_wifiLabelisConnected4, status_symbol);
    UI_SAFE_SET_TEXT(ui_wifiLabelisConnected5, status_symbol);
    UI_SAFE_SET_TEXT(ui_wifiLabelisConnected6, status_symbol);
}

// Screen brightness control
void changeScreenBrightness(lv_event_t* e) {
    lv_obj_t* slider = lv_event_get_target(e);
    if (!slider) return;
    
    int value = lv_slider_get_value(slider);
    
    // Update brightness label if available
    if (ui_screenBrightnessLabelValue) {
        char buf[8];
        snprintf(buf, sizeof(buf), "%d", value);
        lv_label_set_text(ui_screenBrightnessLabelValue, buf);
    }
    
    // Set brightness (map 0-100 to 0-255)
    uint8_t brightness = map(value, 0, 100, 2, 255); // min 2 to avoid complete blackout
    tft.setBrightness(brightness);
    
    Serial.printf("[Brightness] Set to %d%%\n", value);
}

// Keg configuration save
void kegConfigSave(lv_event_t* e) {
    (void)e;
    
    // Safely get selected barrel type
    uint16_t selected = 0;
    if (ui_Dropdown1) {
        selected = lv_dropdown_get_selected(ui_Dropdown1);
    }
    
    // Safely get PSI value
    int psi = 15; // Default
    if (ui_psiSlider) {
        psi = lv_slider_get_value(ui_psiSlider);
    }
    
    Serial.printf("[Keg] Saving config: type=%d, psi=%d\n", selected, psi);
    
    // Would save to preferences or EEPROM here
}

// Initialize all UI events
extern "C" void ui_init_events(void) {
    Serial.println("[UI] Initializing events");
    
    // Register QR Code screen events safely
    if (ui_QRCode) {
        lv_obj_add_event_cb(ui_QRCode, on_QRCodeScreen_load, LV_EVENT_SCREEN_LOADED, NULL);
    }
    
    // Register brightness slider events
    if (ui_screenBrightnessSlider) {
        lv_obj_add_event_cb(ui_screenBrightnessSlider, 
                           changeScreenBrightness,
                           LV_EVENT_VALUE_CHANGED, 
                           NULL);
    }
}

void event_wifiStatusIcon(lv_event_t * e)
{
	// Your code here
}

void event_wifiSpeedTestBtn(lv_event_t * e)
{
	// Your code here
}