// lib/WiFiConfigHelper/include/wifi_config_helper.h
#ifndef WIFI_CONFIG_HELPER_H
#define WIFI_CONFIG_HELPER_H

#include <WiFi.h>
#include <lvgl.h>
#include <Preferences.h>

class WiFiConfigHelper {
public:
  // Must be called once in setup()
  static void begin(const char* namespaceKey = "wifi");

  // Scan and load SSIDs into the roller
  static void scanAndPopulate(lv_obj_t* roller);

  static void updateSignalBar(lv_obj_t* bar);

  // Attempt connection (timeoutMs default 10s)
  // Returns true on successful connection
  static bool connect(const char* ssid, const char* password, uint32_t timeoutMs = 10000);

  // Save creds into NVS
  static void saveCredentials(const char* ssid, const char* password);

  // Load creds from NVS; returns false if none saved
  static bool loadCredentials(String& ssid, String& password);

  // Check if currently connected
  static bool isConnected();

  // Get RSSI value (-30 to -90 dBm, or 0 if not connected)
  static int getSignalStrength();
  
  // Get signal quality as a percentage (0-100%)
  static int getSignalQuality();

  // Get current IP address as string
  static String getIPAddress();

  // Get connection speed (estimated based on signal strength)
  static float getEstimatedSpeed(); // in Mbps

  // Run a simple speed test by downloading test data
  static bool runSpeedTest(float& downloadMbps);

  // Remove saved credentials
  static void clearCredentials(); 

  // Status-change callback signature
  using StatusCb = void(*)(bool isConnected);

  // Register a status-change callback (only one supported)
  static void setStatusCallback(StatusCb cb);

  // Update WiFi status icons on UI
  static void updateStatusIcons(lv_obj_t* wifiLabel);

private:
  static Preferences prefs;
  static StatusCb _statusCb;
  static bool _isBusy;         // Flag to prevent concurrent operations
  static bool _isInitialized;  // Track if begin() has been called
  
  // Helper to safely call the status callback
  static void notifyStatusChange(bool connected);
};

#endif // WIFI_CONFIG_HELPER_H