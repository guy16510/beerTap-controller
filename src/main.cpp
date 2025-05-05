// src/main.cpp
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "mpu6050_helper.h"
#include "beer_pour_helper.h"
#include <lvgl.h>
#include "ui/ui.h"
#include "gfx_conf.h"
#include "constants.h"
#include "wifi_config_helper.h"
#include "beer_state_manager.h"
#include "brightness_manager.h"
#include "display_driver.h"

// Display instance
LGFX tft;

// Function to set display brightness used by BrightnessManager
void setDisplayBrightness(uint8_t value) {
    tft.setBrightness(value);
}

// Display flush callback for LVGL
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    tft.pushImageDMA(area->x1, area->y1, w, h, (lgfx::rgb565_t *)&color_p->full);
    lv_disp_flush_ready(disp);
}

// Touchpad read callback for LVGL
// In the my_touchpad_read function, add:

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    uint16_t touchX, touchY;
    bool touched = tft.getTouch(&touchX, &touchY);
    
    if (!touched) {
        data->state = LV_INDEV_STATE_REL;
    } else {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = touchX;
        data->point.y = touchY;
        
        // Reset screen dim timer on any touch
        BrightnessManager::resetActivityTimer();
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("Smart Beer Tap");

    // GPIO init (panels)...
    // [unchanged]

    // Display Prepare
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(2);
    delay(200);

    // Initialize display and LVGL
    DisplayDriver::init();
    
    // Initialize UI
    ui_init();

    // Initialize managers
    MPU6050Helper::begin(MPU_SDA_PIN, MPU_SCL_PIN);
    BeerPourHelper::begin();
    BeerStateManager::init();
    BrightnessManager::init();
    
    // Initialize WiFi after UI is ready
    WiFiConfigHelper::begin("wifi");
    
    // Register WiFi status callback before loading credentials
    WiFiConfigHelper::setStatusCallback([](bool connected) {
        if (ui_wifiLoadingSpinner) {
            lv_obj_add_flag(ui_wifiLoadingSpinner, LV_OBJ_FLAG_HIDDEN);
        }
        
        // Update status icons on all screens with wifi labels
        if (ui_wifiLabelisConnected1) WiFiConfigHelper::updateStatusIcons(ui_wifiLabelisConnected1);
        if (ui_wifiLabelisConnected3) WiFiConfigHelper::updateStatusIcons(ui_wifiLabelisConnected3);
        if (ui_wifiLabelisConnected4) WiFiConfigHelper::updateStatusIcons(ui_wifiLabelisConnected4);
        if (ui_wifiLabelisConnected5) WiFiConfigHelper::updateStatusIcons(ui_wifiLabelisConnected5);
        if (ui_wifiLabelisConnected6) WiFiConfigHelper::updateStatusIcons(ui_wifiLabelisConnected6);
    });
    
    // Load saved credentials to UI elements if they exist
    String ssid, pass;
    if (WiFiConfigHelper::loadCredentials(ssid, pass)) {
        if (ui_WifiNameValue) {
            lv_label_set_text(ui_WifiNameValue, ssid.c_str());
        }
        if (ui_wifiPassword) {
            lv_textarea_set_text(ui_wifiPassword, pass.c_str());
        }
    }

    Serial.println("Setup done");
}

void loop() {
    // Update LVGL
    lv_timer_handler();
    
    // Update beer state
    BeerStateManager::update();

    BrightnessManager::checkActivity(BeerStateManager::isPouring());
    
    static unsigned long lastWiFiUpdate = 0;
    if (millis() - lastWiFiUpdate > 2000) {
        lastWiFiUpdate = millis();
        // updateWiFiStatusDisplay();

        // Update WiFi signal bar on current screen
        if (ui_wifiSignalBar) {
            WiFiConfigHelper::updateSignalBar(ui_wifiSignalBar);
        }
        
        // Update WiFi icons on various screens as needed
        lv_obj_t* curScreen = lv_scr_act();
        if (curScreen == ui_Home && ui_wifiLabelisConnected6) {
            WiFiConfigHelper::updateStatusIcons(ui_wifiLabelisConnected6);
        }
        // Add other screens as needed
        
    }
}