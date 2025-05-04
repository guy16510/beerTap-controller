// src/main.cpp
#include <Wire.h>
#include <SPI.h>
#include "mpu6050_helper.h"
#include "beer_pour_helper.h"
#include <lvgl.h>
#include "ui/ui.h"
#include "gfx_conf.h"
LGFX tft;
#include "constants.h"
#include "wifi_config_helper.h"

static lv_disp_draw_buf_t draw_buf;
static lv_color_t disp_draw_buf1[screenWidth * screenHeight / 20]; // Reduced buffer size
static lv_color_t disp_draw_buf2[screenWidth * screenHeight / 20]; // Reduced buffer size
static lv_disp_drv_t disp_drv;

// Function prototypes
extern "C" void ui_init_events(void);
void process_wifi_scan(); // Declared in ui_events.cpp

// Watchdog protection - task timing
unsigned long last_button_check = 0;
unsigned long last_sensor_read = 0;
unsigned long last_wifi_check = 0;
unsigned long last_display_update = 0;

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    
    // Use standard push (not DMA) for more reliable operation
    tft.pushImage(area->x1, area->y1, w, h, (lgfx::rgb565_t *)&color_p->full);
    
    lv_disp_flush_ready(disp);
}

/* Touch input */
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    uint16_t touchX, touchY;
    bool touched = tft.getTouch(&touchX, &touchY);
    if (!touched) {
        data->state = LV_INDEV_STATE_REL;
    } else {
        data->state   = LV_INDEV_STATE_PR;
        data->point.x = touchX;
        data->point.y = touchY;
    }
}

void setup() {
    Serial.begin(115200);
    delay(100); // Allow serial to initialize
    Serial.println("\nSmart Beer Tap");

    // Display Prepare - minimal configuration
    tft.begin();
    tft.setRotation(1);
    tft.setBrightness(100); // Default brightness
    tft.fillScreen(TFT_BLACK);
    
    // LVGL initialization with smaller buffer
    lv_init();
    delay(50);

    // LVGL display buffer - reduced size to avoid memory issues
    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf1, disp_draw_buf2,
                          screenWidth * screenHeight / 20);
    
    // LVGL display driver - simplified configuration
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res       = screenHeight;
    disp_drv.ver_res       = screenWidth;
    disp_drv.flush_cb      = my_disp_flush;
    disp_drv.draw_buf      = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    // LVGL input device
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type    = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    // UI initialization first
    ui_init();
    
    // Register UI event handlers
    ui_init_events();
    
    // Hardware initialization
    if (!MPU6050Helper::begin(MPU_SDA_PIN, MPU_SCL_PIN)) {
        Serial.println("Failed to initialize MPU6050");
    }
    
    BeerPourHelper::begin();
    
    // WiFi initialization is now done asynchronously
    // Will be called from loop or when needed by UI

    Serial.println("Setup done");
}

// State for screen navigation
static bool wasPouring = false;
static unsigned long stopTime = 0;

void loop() {
    unsigned long now = millis();
    
    // Process LVGL tasks - most important
    lv_timer_handler();
    
    // Process WiFi scan results if any
    if (now - last_wifi_check > 100) { // Check WiFi every 100ms
        process_wifi_scan();
        
        // Try auto-connect if needed
        if (!WiFiConfigHelper::isConnected() && !WiFiConfigHelper::_isBusy) {
            WiFiConfigHelper::tryAutoConnect();
        }
        
        last_wifi_check = now;
        
        // Short yield to prevent watchdog issues
        delay(1);
    }
    
    // Read sensors periodically - not every loop
    if (now - last_sensor_read > 50) { // 50ms = 20 times per second
        // Read MPU6050 data
        float tempF = 0;
        float zAcc = 0;
        bool readSuccess = false;
        
        try {
            tempF = MPU6050Helper::getTemperatureF();
            zAcc = MPU6050Helper::getZAccel();
            readSuccess = true;
        } catch (...) {
            Serial.println("Error reading MPU6050 sensor");
        }
        
        if (readSuccess) {
            bool currentlyPouring = (zAcc > BEER_ACCEL_THRESHOLD);
            BeerPourHelper::update(currentlyPouring);
            
            // Update UI elements if they exist
            if (ui_temperatureValue != nullptr) {
                char buf[16];
                snprintf(buf, sizeof(buf), "%.1f°F", tempF);
                lv_label_set_text(ui_temperatureValue, buf);
            }
            
            if (ui_zAxisValue != nullptr) {
                char buf[16];
                snprintf(buf, sizeof(buf), "%.2f", zAcc);
                lv_label_set_text(ui_zAxisValue, buf);
            }
            
            // Update pouring status
            if (ui_beerPouringValue != nullptr) {
                lv_label_set_text(ui_beerPouringValue, currentlyPouring ? "Yes" : "No");
            }
            
            // Handle screen transitions
            if (currentlyPouring && !wasPouring && ui_BeerPouring != nullptr) {
                wasPouring = true;
                stopTime = 0;
                lv_scr_load(ui_BeerPouring);
            }
            
            // Update pouring display
            if (wasPouring && ui_ozPouredValue != nullptr && ui_beerPouringBar != nullptr) {
                float pouredOz = BeerPourHelper::currentOunces();
                char buf[16];
                snprintf(buf, sizeof(buf), "%.1f oz", pouredOz);
                lv_label_set_text(ui_ozPouredValue, buf);
                
                int pct = (int)(constrain(pouredOz / 16.0f, 0.0f, 1.0f) * 100.0f);
                lv_bar_set_value(ui_beerPouringBar, pct, LV_ANIM_OFF);
            }
            
            // Handle end of pouring
            if (!currentlyPouring && wasPouring) {
                wasPouring = false;
                stopTime = now;
            }
            
            if (stopTime > 0 && !currentlyPouring && (now - stopTime > 2000) && ui_Home != nullptr) {
                lv_scr_load(ui_Home);
                stopTime = 0;
            }
        }
        
        last_sensor_read = now;
        
        // Short yield to prevent watchdog issues
        delay(1);
    }
    
    // Always yield at the end of loop
    delay(5); // Small delay to prevent watchdog reset and CPU overheating
}