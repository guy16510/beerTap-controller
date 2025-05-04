#include "ui.h"
#include "wifi_config_helper.h"
#include "mpu6050_helper.h"
#include "beer_pour_helper.h"
#include "wifi_status_helper.h"
#include "qr_code_helper.h"  // Add the new include
#include <lvgl.h>
#include <Arduino.h>
#include "gfx_conf.h"

extern LGFX tft;

//—— Wi-Fi manual scan (if you want a button) ——//
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

// WiFi Config screen load event handler
void on_WiFiConfigScreen_load(lv_event_t* e) {
    (void)e;
    Serial.println("[WiFi] Config screen loaded");
    // Add WiFi configuration screen setup code here if needed
}

//—— Hookup all events ——//
extern "C" void ui_init_events(void) {
    Serial.println("[UI] Initializing events");

    // Initialize helpers
    WiFiStatusHelper::begin();
    WiFiStatusHelper::initEvents();
    QRCodeHelper::begin();
    
    // Set the QR code container if available
    if (ui_QRCodeContainer) {
        QRCodeHelper::setContainer(ui_QRCodeContainer);
    }

    // QR screen
    if (ui_QRCode) {
        lv_obj_add_event_cb(ui_QRCode, QRCodeHelper::onScreenLoad, LV_EVENT_SCREEN_LOADED, nullptr);
    }

    // Wi-Fi config screen
    if (ui_WifiConfig) {
        lv_obj_add_event_cb(ui_WifiConfig, on_WiFiConfigScreen_load, LV_EVENT_SCREEN_LOADED, NULL);
    }

    // WiFi Status icon click event
    if (ui_wifiStatusIcon) {
        lv_obj_add_event_cb(ui_wifiStatusIcon, WiFiStatusHelper::onStatusIconClick, LV_EVENT_ALL, NULL);
    }

    // Brightness slider
    if (ui_screenBrightnessSlider) {
        lv_obj_add_event_cb(ui_screenBrightnessSlider,
                            changeScreenBrightness,
                            LV_EVENT_VALUE_CHANGED,
                            nullptr);
    }

    // Wi-Fi status callback
    WiFiConfigHelper::setStatusCallback([](bool connected){
        if (ui_wifiLoadingSpinner) {
            lv_obj_add_flag(ui_wifiLoadingSpinner, LV_OBJ_FLAG_HIDDEN);
        }
    });

    // Refresh labels on any screen load
    lv_obj_t* pages[] = {
        ui_Loading, ui_Home, ui_WifiConfig,
        ui_QRCode, ui_BeerPouring,
        ui_KegConfig, ui_Configuration,
        ui_WiFiStatus
    };
    
    for (auto p : pages) {
        if (!p) continue;
        lv_obj_add_event_cb(p,
            [](lv_event_t*) {
                if (ui_wifiLoadingSpinner) {
                    lv_obj_add_flag(ui_wifiLoadingSpinner, LV_OBJ_FLAG_HIDDEN);
                }
            },
            LV_EVENT_SCREEN_LOADED,
            nullptr
        );
    }
}

//—— SquareLine-generated stubs & other callbacks ——//
extern "C" {

void ui_event_wifiStatusIcon(lv_event_t* e) {
    WiFiStatusHelper::onStatusIconClick(e);
}

void checkWifiStatus(lv_event_t* e) {
    (void)e;
    bool isConnected = WiFiConfigHelper::isConnected();
    Serial.println("[WiFi] Status check");
}

void selectedWifiSSID(lv_event_t* e) {
  if (!ui_wifiNetworksAvailable || !ui_WifiNameValue) return;
  
  char buf[64];
  lv_roller_get_selected_str(ui_wifiNetworksAvailable, buf, sizeof(buf));
  
  // Don't update if "Scanning..." or "None" is selected
  if (strcmp(buf, "Scanning...") != 0 && strcmp(buf, "None") != 0) {
      lv_label_set_text(ui_WifiNameValue, buf);
  }
}

void saveWifiConfiguration(lv_event_t* e) {
  (void)e;
  if (!ui_wifiNetworksAvailable || !ui_wifiPassword || !ui_wifiLoadingSpinner) return;
  
  // Show loading spinner
  lv_obj_clear_flag(ui_wifiLoadingSpinner, LV_OBJ_FLAG_HIDDEN);
  
  // Use a timer to allow UI to update before operations
  lv_timer_t* connect_timer = lv_timer_create([](lv_timer_t* timer) {
      char ssid[64] = {0};
      const char* password = nullptr;
      
      // Get selected SSID from roller
      lv_roller_get_selected_str(ui_wifiNetworksAvailable, ssid, sizeof(ssid));
      
      // Skip connecting if "Scanning..." or "None" is selected
      if (strcmp(ssid, "Scanning...") == 0 || strcmp(ssid, "None") == 0) {
          if (ui_wifiLoadingSpinner) {
              lv_obj_add_flag(ui_wifiLoadingSpinner, LV_OBJ_FLAG_HIDDEN);
          }
          return;
      }
      
      // Get password from textarea
      if (ui_wifiPassword) {
          password = lv_textarea_get_text(ui_wifiPassword);
      }
      
      // Attempt to connect
      WiFiConfigHelper::connect(ssid, password);
      
      // Hide spinner (note: callback will also hide it)
      if (ui_wifiLoadingSpinner) {
          lv_obj_add_flag(ui_wifiLoadingSpinner, LV_OBJ_FLAG_HIDDEN);
      }
  }, 50, NULL);
  lv_timer_set_repeat_count(connect_timer, 1);
}

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
      
      // Hide spinner (note: callback will also hide it)
      if (ui_wifiLoadingSpinner) {
          lv_obj_add_flag(ui_wifiLoadingSpinner, LV_OBJ_FLAG_HIDDEN);
      }
  }, 50, NULL);
  lv_timer_set_repeat_count(delete_timer, 1);
}

void kegConfigSave(lv_event_t* e) {
    (void)e;
    // your existing logic…
}

void changeScreenBrightness(lv_event_t* e) {
    lv_obj_t* s = lv_event_get_target(e);
    int v = lv_slider_get_value(s);
    Serial.printf("[Bright] slider=%d\n", v);
    uint8_t duty = map(v, 0, 100, 0, 255);
    tft.setBrightness(duty);
}

} // extern "C"