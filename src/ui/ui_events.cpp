#include "ui.h"
#include "mpu6050_helper.h"
#include "beer_pour_helper.h"
#include <lvgl.h>
#include <Arduino.h>
#include "gfx_conf.h"
#include <lv_qrcode.h>
#include <qrcodegen.h>

extern LGFX tft;

// Fix for WiFi scanning
void start_wifi_scan(lv_event_t* e) {
    Serial.println("[WiFi] Manual scan requested");
}

// Fix for WiFi network selection
void selectedWifiSSID(lv_event_t* e) {
    (void)e;
    Serial.println("[WiFi] Selected SSID configuration");
}

// Fix for deleting WiFi config
void deleteWiFiConfig(lv_event_t* e) {
    (void)e;
    Serial.println("[WiFi] Deleting configuration");
}

// ---- Save WiFi Config ----
void saveWifiConfiguration(lv_event_t* e) {
    (void)e;
    Serial.println("[WiFi] saveWifiConfiguration");
}


// ---- Check WiFi Status ----
void checkWifiStatus(lv_event_t* e) {
    (void)e;
    Serial.println("[WiFi] Check Wifi status");
}

// --- QR Code Generation ---
static lv_obj_t* ui_QRCodeCode = nullptr;
static lv_obj_t* ui_QRCodeBackground = nullptr;

static void delayed_qr_code_gen(lv_timer_t* timer) {
    (void)timer;
    const char *payload = "https://example.com/my-beer-tap";

    if (!ui_QRCodeContainer) return;

    // Get container size, fallback to defaults
    lv_coord_t w = lv_obj_get_width(ui_QRCodeContainer);
    lv_coord_t h = lv_obj_get_height(ui_QRCodeContainer);
    if (w <= 0 || h <= 0) { w = 240; h = 240; }

    // Remove previous QR code if exists
    if (ui_QRCodeBackground) lv_obj_del(ui_QRCodeBackground);
    if (ui_QRCodeCode) lv_obj_del(ui_QRCodeCode);

    // Create white background
    ui_QRCodeBackground = lv_obj_create(ui_QRCodeContainer);
    lv_obj_remove_style_all(ui_QRCodeBackground);
    lv_obj_set_size(ui_QRCodeBackground, w, h);
    lv_obj_set_style_bg_color(ui_QRCodeBackground, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_center(ui_QRCodeBackground);

    // Create QR code
    ui_QRCodeCode = lv_qrcode_create(ui_QRCodeContainer, w - 20, lv_color_black(), lv_color_white());
    lv_obj_center(ui_QRCodeCode);
    lv_qrcode_update(ui_QRCodeCode, payload, strlen(payload));

    Serial.printf("[QR Code] Generated %dx%d QR\n", w, h);
}

void on_QRCodeScreen_load(lv_event_t* e) {
    (void)e;
    Serial.println("[QR Code] Screen load event triggered");
    lv_timer_t* qr_timer = lv_timer_create(delayed_qr_code_gen, 50, NULL);
    lv_timer_set_repeat_count(qr_timer, 1);
}

// --- Brightness Control ---
void changeScreenBrightness(lv_event_t* e) {
    lv_obj_t* slider = lv_event_get_target(e);
    int value = lv_slider_get_value(slider);
    
    // Update brightness label if available
    if (ui_screenBrightnessLabelValue) {
        char buf[8];
        snprintf(buf, sizeof(buf), "%d", value);
        lv_label_set_text(ui_screenBrightnessLabelValue, buf);
    }
    
    // Set brightness (map 0-100 to 0-255)
    uint8_t brightness = map(value, 0, 100, 0, 255);
    tft.setBrightness(brightness);
}

// --- Keg Config Save ---
void kegConfigSave(lv_event_t* e) {
    (void)e;
    // Get selected barrel type
    uint16_t selected = lv_dropdown_get_selected(ui_Dropdown1);
    // Get PSI value
    int psi = lv_slider_get_value(ui_psiSlider);
    
    // Here you would save to preferences or EEPROM
    Serial.printf("[Keg] Saving config: type=%d, psi=%d\n", selected, psi);
    
    // Return to home screen
    lv_scr_load(ui_Home);
}

// --- Initialize all events ---
extern "C" void ui_init_events(void) {
    Serial.println("[UI] Initializing events");
    
    // Register QR Code screen events
    if (ui_QRCode) {
        lv_obj_add_event_cb(ui_QRCode, on_QRCodeScreen_load, LV_EVENT_SCREEN_LOADED, NULL);
    }

    
    // Update status on any screen load
    lv_obj_t* screens[] = {
        ui_Home, ui_WifiConfig, ui_QRCode, 
        ui_BeerPouring, ui_KegConfig, ui_Configuration
    };
    
}

 void event_wifiStatusIcon(lv_event_t * e)
{
	// Your code here
}

void event_wifiSpeedTestBtn(lv_event_t * e)
{
	// Your code here
}