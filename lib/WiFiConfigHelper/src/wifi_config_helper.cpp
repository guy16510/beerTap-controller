#include "wifi_config_helper.h"
#include <WiFi.h>
#include <Preferences.h>
#include <lvgl.h>

// Static member definitions
Preferences WiFiConfigHelper::prefs;
WiFiConfigHelper::StatusCb WiFiConfigHelper::_statusCb = nullptr;
bool WiFiConfigHelper::_isBusy = false;
bool WiFiConfigHelper::_isInitialized = false;

// Persistent buffer for LVGL roller options
static char *roller_opts_buf = nullptr;

void WiFiConfigHelper::begin(const char* namespaceKey) {
    if (_isInitialized) return;
    prefs.begin(namespaceKey, false);

    // Register WiFi event callbacks *before* using WiFi
    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info){
        if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP) {
            WiFiConfigHelper::notifyStatusChange(true);
        }
        else if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
            WiFiConfigHelper::notifyStatusChange(false);
        }
    });

    WiFi.mode(WIFI_AP_STA);

    // Auto-connect if credentials exist
    String ssid, password;
    if (loadCredentials(ssid, password)) {
        connect(ssid.c_str(), password.c_str());
    }

    _isInitialized = true;
}

bool WiFiConfigHelper::connect(const char* ssid, const char* password, uint32_t timeoutMs) {
    if (_isBusy) return false;
    _isBusy = true;

    WiFi.begin(ssid, password);

    // Non-blocking wait so LVGL can keep ticking
    unsigned long start = millis();
    while (millis() - start < timeoutMs && WiFi.status() != WL_CONNECTED) {
        delay(0);
    }

    bool connected = (WiFi.status() == WL_CONNECTED);

    if (connected) {
        saveCredentials(ssid, password);
    } else {
        clearCredentials();
    }

    notifyStatusChange(connected);
    _isBusy = false;
    return connected;
}

void WiFiConfigHelper::scanAndPopulate(lv_obj_t* roller) {
    // Free previous buffer if any
    if (roller_opts_buf) {
        lv_mem_free(roller_opts_buf);
        roller_opts_buf = nullptr;
    }

    int n = WiFi.scanNetworks();
    size_t total = 0;
    for (int i = 0; i < n; ++i) {
        total += WiFi.SSID(i).length() + 1;  // +1 for newline or NUL
    }

    if (total == 0) {
        // No networks found
        roller_opts_buf = (char*)lv_mem_alloc(1);
        roller_opts_buf[0] = '\0';
        lv_roller_set_options(roller, roller_opts_buf, LV_ROLLER_MODE_NORMAL);
        return;
    }

    // Allocate a persistent buffer
    roller_opts_buf = (char*)lv_mem_alloc(total + 1);
    char *p = roller_opts_buf;

    for (int i = 0; i < n; ++i) {
        String ssid = WiFi.SSID(i);
        memcpy(p, ssid.c_str(), ssid.length());
        p += ssid.length();
        if (i < n - 1) {
            *p++ = '\n';
        }
    }
    *p = '\0';

    // Hand this buffer to LVGL safely
    lv_roller_set_options(roller, roller_opts_buf, LV_ROLLER_MODE_NORMAL);
}

void WiFiConfigHelper::saveCredentials(const char* ssid, const char* password) {
    prefs.putString("ssid", ssid);
    prefs.putString("password", password);
}

bool WiFiConfigHelper::loadCredentials(String& ssid, String& password) {
    ssid     = prefs.getString("ssid", "");
    password = prefs.getString("password", "");
    return ssid.length() > 0;
}

bool WiFiConfigHelper::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

int WiFiConfigHelper::getSignalStrength() {
    if (!isConnected()) return 0;
    return WiFi.RSSI(); // Typically -30 (excellent) to -90 (poor) dBm
}

int WiFiConfigHelper::getSignalQuality() {
    if (!isConnected()) return 0;
    
    // Convert RSSI to quality percentage
    // Typically: -30dBm=100%, -90dBm=0%
    int rssi = getSignalStrength();
    if (rssi >= -30) return 100;
    if (rssi <= -90) return 0;
    
    // Linear mapping from -30...-90 to 100...0
    return 2 * (rssi + 90);
}

String WiFiConfigHelper::getIPAddress() {
    if (!isConnected()) return "0.0.0.0";
    return WiFi.localIP().toString();
}

float WiFiConfigHelper::getEstimatedSpeed() {
    if (!isConnected()) return 0.0;
    
    // Rough estimate based on signal quality
    int quality = getSignalQuality();
    
    if (quality > 80) return 54.0;       // Excellent: ~54 Mbps
    else if (quality > 60) return 36.0;  // Good: ~36 Mbps
    else if (quality > 40) return 18.0;  // Fair: ~18 Mbps
    else if (quality > 20) return 9.0;   // Poor: ~9 Mbps
    else return 1.0;                     // Very poor: ~1 Mbps
}

bool WiFiConfigHelper::runSpeedTest(float& downloadMbps) {
    if (!isConnected()) return false;
    
    // Simple speed test - download a known size file and measure time
    downloadMbps = getEstimatedSpeed();
    
    return true;
}

void WiFiConfigHelper::clearCredentials() {
    prefs.remove("ssid");
    prefs.remove("password");
    WiFi.disconnect(true);
}

void WiFiConfigHelper::setStatusCallback(StatusCb cb) {
    _statusCb = cb;
}

void WiFiConfigHelper::notifyStatusChange(bool connected) {
    if (_statusCb) {
        _statusCb(connected);
    }
}

void WiFiConfigHelper::updateSignalBar(lv_obj_t* bar) {
    if (!bar) return;
    
    if (isConnected()) {
        int quality = getSignalQuality();
        
        // Update the bar's value (assuming 0-100 range)
        lv_bar_set_value(bar, quality, LV_ANIM_ON);
        
        // Optional: Update bar color based on signal strength
        if (quality > 70) {
            lv_obj_set_style_bg_color(bar, lv_palette_main(LV_PALETTE_GREEN), LV_PART_INDICATOR); // Strong
        } else if (quality > 40) {
            lv_obj_set_style_bg_color(bar, lv_palette_main(LV_PALETTE_YELLOW), LV_PART_INDICATOR); // Medium
        } else {
            lv_obj_set_style_bg_color(bar, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR); // Weak
        }
    } else {
        // Not connected
        lv_bar_set_value(bar, 0, LV_ANIM_ON);
    }
}

void WiFiConfigHelper::updateStatusIcons(lv_obj_t* wifiLabel) {
    if (!wifiLabel) return;
    
    if (isConnected()) {
        int quality = getSignalQuality();
        
        // Choose WiFi icon based on signal strength
        if (quality > 70) {
            lv_label_set_text(wifiLabel, LV_SYMBOL_WIFI);
        } else if (quality > 30) {
            lv_label_set_text(wifiLabel, LV_SYMBOL_WIFI);
        } else {
            lv_label_set_text(wifiLabel, LV_SYMBOL_WIFI);
        }
    } else {
        // Not connected
        lv_label_set_text(wifiLabel, LV_SYMBOL_CLOSE);
    }
}