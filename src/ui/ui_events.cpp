// src/ui/ui_events.cpp
#include "ui.h"
#include "brightness_manager.h"
#include "wifi_config_helper.h"
#include <Arduino.h>
#include <lvgl.h>

// Forward declaration to avoid including gfx_conf.h
class LGFX;
extern LGFX tft;

// QR Code helper state
static lv_obj_t* ui_QRCodeCode = nullptr;
static lv_obj_t* ui_QRCodeBackground = nullptr;


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
// Function to check and update WiFi status on UI
// Add to src/ui/ui_events.cpp

// Function to check and update WiFi status icons
void checkWifiStatus(lv_event_t* e) {
    lv_obj_t* label = lv_event_get_target(e);
    if (!label) return;
    
    WiFiConfigHelper::updateStatusIcons(label);
}

// Function when WiFi config screen is loaded
void on_WiFiConfigScreen_load(lv_event_t* e) {
    (void)e;
    Serial.println("[WiFi] Config screen load");
    
    // Show spinner while loading
    if (ui_wifiLoadingSpinner) {
        lv_obj_clear_flag(ui_wifiLoadingSpinner, LV_OBJ_FLAG_HIDDEN);
    }
    
    // Use a timer to allow UI to update before operations
    lv_timer_t* init_timer = lv_timer_create([](lv_timer_t* timer) {
        WiFiConfigHelper::begin();
        
        // Load saved credentials to UI
        String ssid, pass;
        if (WiFiConfigHelper::loadCredentials(ssid, pass)) {
            if (ui_WifiNameValue) {
                lv_label_set_text(ui_WifiNameValue, ssid.c_str());
            }
            if (ui_wifiPassword) {
                lv_textarea_set_text(ui_wifiPassword, pass.c_str());
            }
        }
        
        // Scan for networks
        if (ui_wifiNetworksAvailable) {
            WiFiConfigHelper::scanAndPopulate(ui_wifiNetworksAvailable);
        }
        
        // Update IP address display
        if (ui_wifiIPLabel && WiFiConfigHelper::isConnected()) {
            lv_label_set_text(ui_wifiIPLabel, WiFiConfigHelper::getIPAddress().c_str());
        }
        
        // Update signal bar
        if (ui_wifiSignalBar) {
            WiFiConfigHelper::updateSignalBar(ui_wifiSignalBar);
        }
        
        // Hide spinner when done
        if (ui_wifiLoadingSpinner) {
            lv_obj_add_flag(ui_wifiLoadingSpinner, LV_OBJ_FLAG_HIDDEN);
        }
    }, 50, NULL);
    lv_timer_set_repeat_count(init_timer, 1);
}

// Function when selecting a WiFi network
void selectedWifiSSID(lv_event_t* e) {
    if (!ui_wifiNetworksAvailable || !ui_WifiNameValue) return;
    
    char buf[64];
    lv_roller_get_selected_str(ui_wifiNetworksAvailable, buf, sizeof(buf));
    
    // Don't update if "No networks found" is selected
    if (strcmp(buf, "No networks found") != 0) {
        lv_label_set_text(ui_WifiNameValue, buf);
    }
}

// Function to save WiFi configuration
void saveWifiConfiguration(lv_event_t* e) {
    (void)e;
    if (!ui_WifiNameValue || !ui_wifiPassword || !ui_wifiLoadingSpinner) return;
    
    // Show loading spinner
    lv_obj_clear_flag(ui_wifiLoadingSpinner, LV_OBJ_FLAG_HIDDEN);
    
    // Get selected SSID and password
    const char* ssid = lv_label_get_text(ui_WifiNameValue);
    const char* password = lv_textarea_get_text(ui_wifiPassword);
    
    // Use a timer to connect in a non-blocking way
    lv_timer_t* connect_timer = lv_timer_create([](lv_timer_t* timer) {
        const char* ssid = lv_label_get_text(ui_WifiNameValue);
        const char* password = lv_textarea_get_text(ui_wifiPassword);
        
        // Attempt to connect
        WiFiConfigHelper::connect(ssid, password);
        
        // Hide spinner
        if (ui_wifiLoadingSpinner) {
            lv_obj_add_flag(ui_wifiLoadingSpinner, LV_OBJ_FLAG_HIDDEN);
        }
    }, 50, NULL);
    lv_timer_set_repeat_count(connect_timer, 1);
}

// Function to delete WiFi configuration
void deleteWiFiConfig(lv_event_t* e) {
    (void)e;
    
    // Show loading spinner
    if (ui_wifiLoadingSpinner) {
        lv_obj_clear_flag(ui_wifiLoadingSpinner, LV_OBJ_FLAG_HIDDEN);
    }
    
    // Use a timer to allow UI to update before operations
    lv_timer_t* delete_timer = lv_timer_create([](lv_timer_t* timer) {
        // Clear credentials
        WiFiConfigHelper::clearCredentials();
        
        // Update UI elements
        if (ui_WifiNameValue) {
            lv_label_set_text(ui_WifiNameValue, "");
        }
        
        if (ui_wifiPassword) {
            lv_textarea_set_text(ui_wifiPassword, "");
        }
        
        // Hide spinner
        if (ui_wifiLoadingSpinner) {
            lv_obj_add_flag(ui_wifiLoadingSpinner, LV_OBJ_FLAG_HIDDEN);
        }
    }, 50, NULL);
    lv_timer_set_repeat_count(delete_timer, 1);
}

// Function to start WiFi scan
void start_wifi_scan(lv_event_t* e) {
    (void)e;
    Serial.println("[WiFi] Manual scan requested");
    if (ui_wifiNetworksAvailable && ui_wifiLoadingSpinner) {
        lv_obj_clear_flag(ui_wifiLoadingSpinner, LV_OBJ_FLAG_HIDDEN);
        
        // Use a timer to allow the spinner to become visible before scan
        lv_timer_t* scan_timer = lv_timer_create([](lv_timer_t* timer) {
            WiFiConfigHelper::scanAndPopulate(ui_wifiNetworksAvailable);
            if (ui_wifiLoadingSpinner) {
                lv_obj_add_flag(ui_wifiLoadingSpinner, LV_OBJ_FLAG_HIDDEN);
            }
        }, 50, NULL);
        lv_timer_set_repeat_count(scan_timer, 1);
    }
}

// Function to update all WiFi info on status page
// Fix in updateWifiStatusDisplay() function in src/ui/ui_events.cpp
void updateWifiStatusDisplay() {
    // Update all WiFi status UI elements
    bool isConnected = WiFiConfigHelper::isConnected();
    
    // Update signal bar if it exists
    if (ui_wifiSignalBar) {
        WiFiConfigHelper::updateSignalBar(ui_wifiSignalBar);
    }
    
    // Update IP address
    if (ui_wifiIPLabel) {
        lv_label_set_text(ui_wifiIPLabel, WiFiConfigHelper::getIPAddress().c_str());
    }
    
    // Update signal quality value - using suggested name from error message
    if (ui_wifiSignalValue) {  // Changed from ui_wifiSignalLabel to ui_wifiSignalValue
        char buf[32];
        snprintf(buf, sizeof(buf), "%d%%", WiFiConfigHelper::getSignalQuality());
        lv_label_set_text(ui_wifiSignalValue, buf);
    }
    
    // Update speed estimate if it exists
    if (ui_wifiSpeedLabel) {
        char buf[32];
        snprintf(buf, sizeof(buf), "%.1f Mbps", WiFiConfigHelper::getEstimatedSpeed());
        lv_label_set_text(ui_wifiSpeedLabel, buf);
    }
}
// Speed test function
void runWifiSpeedTest(lv_event_t* e) {
    (void)e;
    if (!ui_wifiSpeedLabel) return;
    
    // Show testing message
    lv_label_set_text(ui_wifiSpeedLabel, "Testing...");
    
    // Run test in non-blocking way
    lv_timer_t* test_timer = lv_timer_create([](lv_timer_t* timer) {
        float speed = 0.0;
        bool success = WiFiConfigHelper::runSpeedTest(speed);
        
        // Update result
        if (ui_wifiSpeedLabel) {
            char buf[32];
            if (success) {
                snprintf(buf, sizeof(buf), "%.1f Mbps", speed);
            } else {
                snprintf(buf, sizeof(buf), "Test Failed");
            }
            lv_label_set_text(ui_wifiSpeedLabel, buf);
        }
    }, 50, NULL);
    lv_timer_set_repeat_count(test_timer, 1);
}

// Optional: QR code generation for WiFi credentials
// void on_QRCodeScreen_load(lv_event_t* e) {
//     (void)e;
//     Serial.println("[QR Code] Screen load event triggered");
    
//     // Get SSID and password
//     String ssid, password;
//     WiFiConfigHelper::loadCredentials(ssid, password);
    
//     // Create WiFi credential QR code format
//     // Format: WIFI:S:<SSID>;T:<WPA|WEP|>;P:<password>;;
//     String qrContent = "WIFI:S:" + ssid + ";T:WPA;P:" + password + ";;";
    
//     // Implement QR code generation here if you have a QR library
//     // For now, just display the SSID on QR screen
//     if (ui_QRImageLabel) {
//         lv_label_set_text(ui_QRImageLabel, ssid.c_str());
//     }
// }

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