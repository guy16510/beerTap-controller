// lib/WiFiStatusHelper/include/wifi_status_helper.h
#ifndef WIFI_STATUS_HELPER_H
#define WIFI_STATUS_HELPER_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <lvgl.h>

// Forward declarations for UI elements - these are defined in ui.h
extern lv_obj_t *ui_WiFiStatus;
extern lv_obj_t *ui_wifiSignalBar;
extern lv_obj_t *ui_wifiSignalValue;
extern lv_obj_t *ui_wifiIPValue;
extern lv_obj_t *ui_wifiSSIDValue;
extern lv_obj_t *ui_wifiStatusIcon;
extern lv_obj_t *ui_wifiSpeedTestBtn;
extern lv_obj_t *ui_wifiSpeedValue;
extern lv_obj_t *ui_Configuration;
extern lv_obj_t *ui_LabelBack;

// Forward declare UI screen init functions
void ui_Configuration_screen_init(void);

class WiFiStatusHelper {
public:
    // Initialize WiFi status helper (call once in setup)
    static void begin();
    
    // Convert RSSI to signal quality percentage (0-100%)
    static int rssiToQuality(int rssi);
    
    // Updates WiFi status UI elements (call from timer or directly)
    static void updateStatusInfo(lv_timer_t* timer);
    
    // Perform speed test (call from event handler)
    static void performSpeedTest(lv_event_t* e);
    
    // Screen load handler (sets up timer)
    static void onScreenLoad(lv_event_t* e);
    
    // Screen unload handler (cleans up timer)
    static void onScreenUnload(lv_event_t* e);
    
    // Status icon click handler
    static void onStatusIconClick(lv_event_t* e);
    
    // Register all event handlers (call from ui_init_events)
    static void initEvents();

private:
    // Timer for periodic updates
    static lv_timer_t* _statusTimer;
};

#endif // WIFI_STATUS_HELPER_H