// lib/WiFiStatusHelper/src/wifi_status_helper.cpp
#include "wifi_status_helper.h"

// We need this for _ui_screen_change
extern void _ui_screen_change(lv_obj_t ** target, lv_scr_load_anim_t fademode, int spd, int delay, void (*target_init)(void));
extern void _ui_screen_delete(lv_obj_t ** target);

// Initialize static member
lv_timer_t* WiFiStatusHelper::_statusTimer = nullptr;

void WiFiStatusHelper::begin() {
    // Nothing to initialize currently
}

int WiFiStatusHelper::rssiToQuality(int rssi) {
    if (rssi <= -100)
        return 0;
    if (rssi >= -50)
        return 100;
    return 2 * (rssi + 100);
}

void WiFiStatusHelper::updateStatusInfo(lv_timer_t* timer) {
    (void)timer;
    
    // Check if screen elements exist
    if (!ui_wifiSignalBar || !ui_wifiSignalValue || !ui_wifiIPValue || 
        !ui_wifiSSIDValue || !ui_wifiStatusIcon) {
        return;
    }
    
    // Check if we're connected
    if (WiFi.status() != WL_CONNECTED) {
        lv_label_set_text(ui_wifiSSIDValue, "Not connected");
        lv_label_set_text(ui_wifiIPValue, "---.---.---.---");
        lv_label_set_text(ui_wifiSignalValue, "-- dBm");
        lv_bar_set_value(ui_wifiSignalBar, 0, LV_ANIM_OFF);
        lv_label_set_text(ui_wifiStatusIcon, LV_SYMBOL_WIFI);
        return;
    }
    
    // Get RSSI and quality percentage
    int rssi = WiFi.RSSI();
    int quality = rssiToQuality(rssi);
    
    // Update SSID
    String ssid = WiFi.SSID();
    lv_label_set_text(ui_wifiSSIDValue, ssid.c_str());
    
    // Update IP address
    String ip = WiFi.localIP().toString();
    lv_label_set_text(ui_wifiIPValue, ip.c_str());
    
    // Update signal strength value
    char buf[32];
    snprintf(buf, sizeof(buf), "%d dBm (%d%%)", rssi, quality);
    lv_label_set_text(ui_wifiSignalValue, buf);
    
    // Update signal strength bar
    lv_bar_set_value(ui_wifiSignalBar, quality, LV_ANIM_ON);
    
    // Update WiFi icon based on signal strength
    if (quality > 70) {
        lv_label_set_text(ui_wifiStatusIcon, LV_SYMBOL_WIFI);  // Full signal
    } else if (quality > 30) {
        lv_label_set_text(ui_wifiStatusIcon, LV_SYMBOL_WIFI);  // Medium signal
    } else {
        lv_label_set_text(ui_wifiStatusIcon, LV_SYMBOL_WIFI);  // Low signal
    }
}

void WiFiStatusHelper::performSpeedTest(lv_event_t* e) {
    if (!ui_wifiSpeedValue) return;
    lv_label_set_text(ui_wifiSpeedValue, "Testing...");
    
    // Use a timer to prevent blocking
    lv_timer_t* speed_timer = lv_timer_create([](lv_timer_t* timer) {
        HTTPClient http;
        
        // Start download of a small file (1MB test file)
        http.begin("http://ipv4.download.thinkbroadband.com/1MB.zip");
        
        unsigned long start_time = millis();
        int httpCode = http.GET();
        
        if (httpCode == HTTP_CODE_OK) {
            // Get download size and calculate speed
            int contentLength = http.getSize();
            int bytesRead = 0;
            uint8_t buffer[512];
            WiFiClient* stream = http.getStreamPtr();
            
            while (http.connected() && (bytesRead < contentLength)) {
                size_t size = stream->available();
                if (size > 0) {
                    size_t read_bytes = stream->readBytes(buffer, min((size_t)sizeof(buffer), size));
                    bytesRead += read_bytes;
                }
                // Prevent watchdog from triggering
                yield();
            }
            
            unsigned long duration = millis() - start_time;
            float speed_kbps = (bytesRead * 8.0) / (duration); // Kilobits per second
            
            char result[64];
            if (speed_kbps > 1000) {
                snprintf(result, sizeof(result), "%.2f Mbps", speed_kbps / 1000.0f);
            } else {
                snprintf(result, sizeof(result), "%.1f Kbps", speed_kbps);
            }
            
            if (ui_wifiSpeedValue) {
                lv_label_set_text(ui_wifiSpeedValue, result);
            }
        } else {
            if (ui_wifiSpeedValue) {
                lv_label_set_text(ui_wifiSpeedValue, "Test failed");
            }
        }
        
        http.end();
    }, 50, nullptr);
    
    lv_timer_set_repeat_count(speed_timer, 1);
}

void WiFiStatusHelper::onScreenLoad(lv_event_t* e) {
    (void)e;
    Serial.println("[WiFi] Status screen loaded");
    
    // Initial update
    updateStatusInfo(nullptr);
    
    // Start periodic updates (every 2 seconds)
    if (_statusTimer) {
        lv_timer_del(_statusTimer);
    }
    _statusTimer = lv_timer_create(updateStatusInfo, 2000, nullptr);
}

void WiFiStatusHelper::onScreenUnload(lv_event_t* e) {
    (void)e;
    
    // Delete the timer when leaving the screen
    if (_statusTimer) {
        lv_timer_del(_statusTimer);
        _statusTimer = nullptr;
    }
}

void WiFiStatusHelper::onStatusIconClick(lv_event_t* e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_CLICKED) {
        updateStatusInfo(nullptr);
    }
}

void WiFiStatusHelper::initEvents() {
    // WiFi Status screen
    if (ui_WiFiStatus) {
        lv_obj_add_event_cb(ui_WiFiStatus, onScreenLoad, LV_EVENT_SCREEN_LOADED, NULL);
        lv_obj_add_event_cb(ui_WiFiStatus, onScreenUnload, LV_EVENT_SCREEN_UNLOADED, NULL);
    }

    // Speed Test button
    if (ui_wifiSpeedTestBtn) {
        lv_obj_add_event_cb(ui_wifiSpeedTestBtn, performSpeedTest, LV_EVENT_CLICKED, NULL);
    }
}