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

// IMPORTANT: Static buffer management
static char *roller_opts_buf = nullptr;

void WiFiConfigHelper::begin(const char* namespaceKey) {
    if (_isInitialized) return;

    Serial.println("WiFiConfigHelper initializing...");
    
    // Always open preferences with false (read/write mode)
    if (!prefs.begin(namespaceKey, false)) {
        Serial.println("Failed to initialize Preferences");
    }
    
    // Set WiFi mode to combined AP+STA - prevents some hanging
    WiFi.mode(WIFI_STA);
    
    // Register event handler with non-blocking implementation
    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
        // Keep callbacks minimal to avoid watchdog issues
        if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP) {
            Serial.println("WiFi connected, got IP");
            if (WiFiConfigHelper::_statusCb) {
                WiFiConfigHelper::_statusCb(true);
            }
        }
        else if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
            Serial.println("WiFi disconnected");
            if (WiFiConfigHelper::_statusCb) {
                WiFiConfigHelper::_statusCb(false);
            }
        }
    });

    // Don't auto-connect here - can block too long
    _isInitialized = true;
    Serial.println("WiFiConfigHelper initialized");
}

// Non-blocking WiFi connection attempt
bool WiFiConfigHelper::connect(const char* ssid, const char* password, uint32_t timeoutMs) {
    if (!_isInitialized) {
        Serial.println("ERROR: WiFiConfigHelper not initialized");
        return false;
    }

    if (_isBusy) {
        Serial.println("WiFi connect already in progress");
        return false;
    }
    
    _isBusy = true;

    // Make sure inputs are valid
    if (ssid == nullptr || strlen(ssid) == 0) {
        Serial.println("Invalid SSID");
        _isBusy = false;
        return false;
    }

    // Empty password is valid (open networks)
    const char* pass = (password == nullptr) ? "" : password;
    
    Serial.printf("Connecting to: %s\n", ssid);
    WiFi.disconnect();  // Disconnect first to avoid hanging
    delay(10);          // Short delay to process disconnect
    WiFi.begin(ssid, pass);
    
    // Set a very short timeout - we won't block for long
    unsigned long start = millis();
    uint32_t shortTimeout = min(timeoutMs, (uint32_t)500); // Max 500ms to prevent watchdog
    
    while (millis() - start < shortTimeout && WiFi.status() != WL_CONNECTED) {
        delay(10); // Yield to allow other processes
    }

    bool connected = (WiFi.status() == WL_CONNECTED);
    
    if (connected) {
        Serial.println("WiFi connected successfully");
        saveCredentials(ssid, pass);
    } else {
        Serial.println("Initial connection attempt incomplete - continuing in background");
        // Connection will continue in background
    }

    _isBusy = false;
    return connected;
}

void WiFiConfigHelper::scanAndPopulate(lv_obj_t* roller) {
    if (roller == nullptr) {
        Serial.println("Invalid roller target");
        return;
    }

    // Free previous buffer if any
    if (roller_opts_buf != nullptr) {
        lv_mem_free(roller_opts_buf);
        roller_opts_buf = nullptr;
    }
    
    // Set temporary text while scanning
    const char* scanning_text = "Scanning...";
    roller_opts_buf = (char*)lv_mem_alloc(strlen(scanning_text) + 1);
    if (roller_opts_buf != nullptr) {
        strcpy(roller_opts_buf, scanning_text);
        lv_roller_set_options(roller, roller_opts_buf, LV_ROLLER_MODE_NORMAL);
    }

    // Start non-blocking scan
    Serial.println("Starting WiFi scan (non-blocking)");
    WiFi.scanNetworks(true); // true = async scan
    
    // Actual population will happen when scan completes
    // This can be checked with WiFi.scanComplete()
}

// Call this from your main loop to process scan results
void WiFiConfigHelper::processScanResults(lv_obj_t* roller) {
    if (roller == nullptr) return;
    
    int scanStatus = WiFi.scanComplete();
    
    if (scanStatus == WIFI_SCAN_RUNNING) {
        // Still scanning, do nothing
        return;
    }
    
    if (scanStatus < 0) {
        // Error or no scan in progress
        return;
    }
    
    // Free previous buffer
    if (roller_opts_buf != nullptr) {
        lv_mem_free(roller_opts_buf);
        roller_opts_buf = nullptr;
    }

    int n = scanStatus; // Number of networks found
    Serial.printf("WiFi scan complete, found %d networks\n", n);
    
    if (n <= 0) {
        // No networks found
        const char* default_text = "No networks found";
        roller_opts_buf = (char*)lv_mem_alloc(strlen(default_text) + 1);
        if (roller_opts_buf != nullptr) {
            strcpy(roller_opts_buf, default_text);
            lv_roller_set_options(roller, roller_opts_buf, LV_ROLLER_MODE_NORMAL);
        }
        WiFi.scanDelete(); // Clean up scan results
        return;
    }

    // Calculate buffer size needed
    size_t total = 0;
    for (int i = 0; i < n; ++i) {
        String ssid = WiFi.SSID(i);
        if (ssid.length() > 0) {
            total += ssid.length() + 1; // +1 for newline or null
        }
    }
    
    // Allocate buffer with extra safety margin
    roller_opts_buf = (char*)lv_mem_alloc(total + 10);
    if (roller_opts_buf == nullptr) {
        Serial.println("Failed to allocate memory for WiFi list");
        WiFi.scanDelete();
        return;
    }
    
    // Fill buffer
    char *p = roller_opts_buf;
    *p = '\0'; // Start with empty string
    
    for (int i = 0; i < n; ++i) {
        String ssid = WiFi.SSID(i);
        if (ssid.length() > 0) {
            strncpy(p, ssid.c_str(), ssid.length());
            p += ssid.length();
            
            // Add newline except for last entry
            if (i < n - 1) {
                *p++ = '\n';
            }
            *p = '\0';
        }
    }
    
    // Update roller
    lv_roller_set_options(roller, roller_opts_buf, LV_ROLLER_MODE_NORMAL);
    
    // Clean up scan results to free memory
    WiFi.scanDelete();
}

void WiFiConfigHelper::saveCredentials(const char* ssid, const char* password) {
    if (!_isInitialized) {
        Serial.println("WiFiConfigHelper not initialized");
        return;
    }
    
    // Safety check
    if (ssid == nullptr) {
        Serial.println("Cannot save null SSID");
        return;
    }
    
    const char* safeSSID = ssid;
    const char* safePass = (password != nullptr) ? password : "";
    
    prefs.putString("ssid", safeSSID);
    prefs.putString("password", safePass);
    Serial.printf("Saved WiFi credentials for: %s\n", safeSSID);
}

bool WiFiConfigHelper::loadCredentials(String& ssid, String& password) {
    if (!_isInitialized) {
        Serial.println("WiFiConfigHelper not initialized");
        return false;
    }
    
    ssid = prefs.getString("ssid", "");
    password = prefs.getString("password", "");
    
    if (ssid.length() > 0) {
        Serial.printf("Loaded saved credentials for: %s\n", ssid.c_str());
    }
    
    return (ssid.length() > 0);
}

bool WiFiConfigHelper::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void WiFiConfigHelper::clearCredentials() {
    if (!_isInitialized) {
        Serial.println("WiFiConfigHelper not initialized");
        return;
    }
    
    prefs.remove("ssid");
    prefs.remove("password");
    Serial.println("WiFi credentials cleared");
}

void WiFiConfigHelper::setStatusCallback(StatusCb cb) {
    _statusCb = cb;
}

void WiFiConfigHelper::tryAutoConnect() {
    if (!_isInitialized) {
        begin(); // Auto-initialize if needed
    }
    
    if (_isBusy) return;
    
    String ssid, password;
    if (loadCredentials(ssid, password) && ssid.length() > 0) {
        connect(ssid.c_str(), password.c_str());
    }
}