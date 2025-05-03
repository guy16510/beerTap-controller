// File: lib/WiFiConfigHelper/src/wifi_config_helper.cpp

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

void WiFiConfigHelper::clearCredentials() {
    prefs.remove("ssid");
    prefs.remove("password");
}

void WiFiConfigHelper::setStatusCallback(StatusCb cb) {
    _statusCb = cb;
}

void WiFiConfigHelper::notifyStatusChange(bool connected) {
    if (_statusCb) {
        _statusCb(connected);
    }
}