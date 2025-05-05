// src/ui/ui_events.cpp
#include "ui.h"
#include "brightness_manager.h"
// #include "../wifi_config_helper.h"
#include <Arduino.h>
#include <lvgl.h>

// Forward declaration to avoid including gfx_conf.h
class LGFX;
extern LGFX tft;

// QR Code helper state
static lv_obj_t* ui_QRCodeCode = nullptr;
static lv_obj_t* ui_QRCodeBackground = nullptr;

// WiFi scan timer callback
static void delayed_wifi_scan(lv_timer_t* timer) {
    // WiFiConfigHelper::scanAndPopulate(ui_wifiNetworksAvailable);
    // if (ui_wifiLoadingSpinner) {
    //     lv_obj_add_flag(ui_wifiLoadingSpinner, LV_OBJ_FLAG_HIDDEN);
    // }
}

// WiFi connect timer callback
static void delayed_wifi_connect(lv_timer_t* timer) {
    // char ssid[64] = {0};
    // const char* password = nullptr;
    
    // // Get selected SSID from roller
    // lv_roller_get_selected_str(ui_wifiNetworksAvailable, ssid, sizeof(ssid));
    
    // // Skip connecting if "Scanning..." or "None" is selected
    // if (strcmp(ssid, "Scanning...") == 0 || strcmp(ssid, "None") == 0) {
    //     if (ui_wifiLoadingSpinner) {
    //         lv_obj_add_flag(ui_wifiLoadingSpinner, LV_OBJ_FLAG_HIDDEN);
    //     }
    //     return;
    // }
    
    // // Get password from textarea
    // if (ui_wifiPassword) {
    //     password = lv_textarea_get_text(ui_wifiPassword);
    // }
    
    // // Attempt to connect
    // WiFiConfigHelper::connect(ssid, password);
    
    // // Hide spinner (note: callback will also hide it)
    // if (ui_wifiLoadingSpinner) {
    //     lv_obj_add_flag(ui_wifiLoadingSpinner, LV_OBJ_FLAG_HIDDEN);
    // }
}

// WiFi delete timer callback
static void delayed_wifi_delete(lv_timer_t* timer) {
    // // Clear credentials
    // WiFiConfigHelper::clearCredentials();
    
    // // Update UI elements
    // if (ui_WifiNameValue) {
    //     lv_label_set_text(ui_WifiNameValue, "");
    // }
    
    // if (ui_wifiPassword) {
    //     lv_textarea_set_text(ui_wifiPassword, "");
    // }
    
    // // Hide spinner
    // if (ui_wifiLoadingSpinner) {
    //     lv_obj_add_flag(ui_wifiLoadingSpinner, LV_OBJ_FLAG_HIDDEN);
    // }
}

// Update WiFi status icons
static void updateWiFiStatusIcons(bool connected) {
    // const char* status = connected ? "🌐" : "❌";
    
    // // Update all WiFi status indicators in the UI
    // if (ui_wifiLabelisConnected1) lv_label_set_text(ui_wifiLabelisConnected1, status);
    // if (ui_wifiLabelisConnected3) lv_label_set_text(ui_wifiLabelisConnected3, status);
    // if (ui_wifiLabelisConnected4) lv_label_set_text(ui_wifiLabelisConnected4, status);
    // if (ui_wifiLabelisConnected5) lv_label_set_text(ui_wifiLabelisConnected5, status);
    // if (ui_wifiLabelisConnected6) lv_label_set_text(ui_wifiLabelisConnected6, status);
}

// QR code generation
static void delayed_qr_code_gen(lv_timer_t* timer) {
    const char *payload = "https://example.com";

    if (!ui_QRCodeContainer) return;

    // Get container size, fallback to defaults
    lv_coord_t w = lv_obj_get_width(ui_QRCodeContainer);
    lv_coord_t h = lv_obj_get_height(ui_QRCodeContainer);
    if (w <= 0 || h <= 0) { w = 480; h = 560; }

    // Remove previous
    if (ui_QRCodeBackground) {
        lv_obj_del(ui_QRCodeBackground);
        ui_QRCodeBackground = nullptr;
    }
    if (ui_QRCodeCode) {
        lv_obj_del(ui_QRCodeCode);
        ui_QRCodeCode = nullptr;
    }

    // Background
    ui_QRCodeBackground = lv_obj_create(ui_QRCodeContainer);
    lv_obj_remove_style_all(ui_QRCodeBackground);
    lv_obj_set_size(ui_QRCodeBackground, w, h);
    lv_obj_set_style_bg_color(ui_QRCodeBackground, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_center(ui_QRCodeBackground);

    // QR code
    ui_QRCodeCode = lv_qrcode_create(ui_QRCodeContainer, w, lv_color_black(), lv_color_white());
    lv_obj_center(ui_QRCodeCode);
    lv_qrcode_update(ui_QRCodeCode, payload, strlen(payload));

    Serial.printf("[QR Code] Generated %dx%d QR\n", w, w);
}

// SquareLine-generated event handlers
extern "C" {

void checkWifiStatus(lv_event_t* e) {
    // (void)e;
    // bool isConnected = WiFiConfigHelper::isConnected();
    // updateWiFiStatusIcons(isConnected);
    // Serial.println("[WiFi] Status check");
}

void selectedWifiSSID(lv_event_t* e) {
    // if (!ui_wifiNetworksAvailable || !ui_WifiNameValue) return;
    
    // char buf[64];
    // lv_roller_get_selected_str(ui_wifiNetworksAvailable, buf, sizeof(buf));
    
    // // Don't update if "Scanning..." or "None" is selected
    // if (strcmp(buf, "Scanning...") != 0 && strcmp(buf, "None") != 0) {
    //     lv_label_set_text(ui_WifiNameValue, buf);
    // }
}

void saveWifiConfiguration(lv_event_t* e) {
    // (void)e;
    // if (!ui_wifiNetworksAvailable || !ui_wifiPassword || !ui_wifiLoadingSpinner) return;
    
    // // Show loading spinner
    // lv_obj_clear_flag(ui_wifiLoadingSpinner, LV_OBJ_FLAG_HIDDEN);
    
    // // Use a timer to allow UI to update before operations
    // lv_timer_t* connect_timer = lv_timer_create(delayed_wifi_connect, 50, NULL);
    // lv_timer_set_repeat_count(connect_timer, 1);
}

void deleteWiFiConfig(lv_event_t* e) {
    // (void)e;
    
    // // Show loading spinner
    // if (ui_wifiLoadingSpinner) {
    //     lv_obj_clear_flag(ui_wifiLoadingSpinner, LV_OBJ_FLAG_HIDDEN);
    // }
    
    // // Use a timer to allow UI to update before operations
    // lv_timer_t* delete_timer = lv_timer_create(delayed_wifi_delete, 50, NULL);
    // lv_timer_set_repeat_count(delete_timer, 1);
}

void start_wifi_scan(lv_event_t* e) {
    // (void)e;
    // Serial.println("[WiFi] Manual scan requested");
    // if (ui_wifiNetworksAvailable && ui_wifiLoadingSpinner) {
    //     lv_obj_clear_flag(ui_wifiLoadingSpinner, LV_OBJ_FLAG_HIDDEN);
        
    //     // Use a timer to allow the spinner to become visible before scan
    //     lv_timer_t* scan_timer = lv_timer_create(delayed_wifi_scan, 50, NULL);
    //     lv_timer_set_repeat_count(scan_timer, 1);
    // }
}

void on_QRCodeScreen_load(lv_event_t* e) {
    (void)e;
    Serial.println("[QR Code] Screen load event triggered");
    lv_timer_t* qr_timer = lv_timer_create(delayed_qr_code_gen, 50, NULL);
    lv_timer_set_repeat_count(qr_timer, 1);
}

void kegConfigSave(lv_event_t* e) {
    (void)e;
    // Save keg configuration settings
    // Specific implementation depends on your keg config requirements
}

void event_wifiStatusIcon(lv_event_t * e){

};
void event_wifiSpeedTestBtn(lv_event_t * e){
    
};

void changeScreenBrightness(lv_event_t* e) {
    lv_obj_t* s = lv_event_get_target(e);
    int v = lv_slider_get_value(s);
    Serial.printf("[Bright] slider=%d\n", v);
    
    // Set display brightness
    BrightnessManager::setBrightness(v);
    
    // Update slider text if available
    if (ui_screenBrightnessLabelValue) {
        char buf[8];
        snprintf(buf, sizeof(buf), "%d", v);
        lv_label_set_text(ui_screenBrightnessLabelValue, buf);
    }
}

} // extern "C"