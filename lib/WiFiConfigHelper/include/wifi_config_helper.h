#ifndef WIFI_CONFIG_HELPER_H
#define WIFI_CONFIG_HELPER_H

#include <WiFi.h>
#include <lvgl.h>
#include <Preferences.h>

class WiFiConfigHelper {
public:
  // Must be called once in setup()
  static void begin(const char* namespaceKey = "wifi");

  // Non-blocking scan - only starts the scan
  static void scanAndPopulate(lv_obj_t* roller);
  
  // Process scan results - call this periodically
  static void processScanResults(lv_obj_t* roller);

  // Attempt connection (timeoutMs default 10s)
  // Returns true on successful connection
  static bool connect(const char* ssid, const char* password, uint32_t timeoutMs = 10000);
  
  // Attempt to connect with saved credentials
  static void tryAutoConnect();

  // Save creds into NVS
  static void saveCredentials(const char* ssid, const char* password);

  // Load creds from NVS; returns false if none saved
  static bool loadCredentials(String& ssid, String& password);

  // Check if currently connected
  static bool isConnected();

  // Remove saved credentials
  static void clearCredentials(); 

  // Status-change callback signature
  using StatusCb = void(*)(bool isConnected);

  // Register a status-change callback (only one supported)
  static void setStatusCallback(StatusCb cb);

  // Public status flag - accessible by UI
  static bool _isInitialized;
  static bool _isBusy;

private:
  static Preferences prefs;
  static StatusCb _statusCb;
};

#endif // WIFI_CONFIG_HELPER_H