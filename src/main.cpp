// src/main.cpp
#include <Wire.h>
#include <SPI.h>
#include "mpu6050_helper.h"
#include "beer_pour_helper.h"
#include <lvgl.h>
#include "ui/ui.h"
#include "gfx_conf.h"
#include "constants.h"

LGFX tft;

// Flag to track initial startup vs wake from sleep
static bool isInitialStartup = true;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t disp_draw_buf1[screenWidth * screenHeight / 10];
static lv_color_t disp_draw_buf2[screenWidth * screenHeight / 10];
static lv_disp_drv_t disp_drv;

// Flag to track initialization status
static bool ui_initialized = false;
static bool sensors_initialized = false;

// Screen timeout settings
#define SCREEN_TIMEOUT_MS      10000   // 10 seconds timeout
#define SCREEN_DIM_BRIGHTNESS  0       // Display off value
static uint8_t screenOnBrightness = 200;  // Normal brightness - can be modified by UI

// Screen state variables
static uint32_t lastActivityTime = 0;
static bool isScreenOn = true;

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    tft.pushImageDMA(area->x1, area->y1, w, h, (lgfx::rgb565_t *)&color_p->full);
    lv_disp_flush_ready(disp);
}

/* Touch input */
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    uint16_t touchX, touchY;
    bool touched = tft.getTouch(&touchX, &touchY);
    
    // Wake screen on touch, but don't trigger Loading screen
    if (touched) {
        lastActivityTime = millis();
        if (!isScreenOn) {
            tft.setBrightness(screenOnBrightness);
            isScreenOn = true;
            
            // If this is a wake-up touch (not initial startup), 
            // skip sending the touch event to UI to avoid loading screens
            if (!isInitialStartup) {
                // Reset the touch event but don't pass it to UI
                data->state = LV_INDEV_STATE_REL;
                return;
            }
        }
    }
    
    // Initial startup is complete once we handle a touch
    if (touched) {
        isInitialStartup = false;
    }
    
    // Normal touch handling
    if (!touched) {
        data->state = LV_INDEV_STATE_REL;
    } else {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = touchX;
        data->point.y = touchY;
    }
}

// Check and update screen state
void updateScreenTimeout(bool activityDetected) {
    uint32_t now = millis();
    
    // Reset timer on activity
    if (activityDetected) {
        lastActivityTime = now;
        if (!isScreenOn) {
            tft.setBrightness(screenOnBrightness);
            isScreenOn = true;
        }
        return;
    }
    
    // Check for timeout
    if (isScreenOn && (now - lastActivityTime > SCREEN_TIMEOUT_MS)) {
        // Turn off the screen
        tft.setBrightness(SCREEN_DIM_BRIGHTNESS);
        isScreenOn = false;
    }
}

// Function to update brightness from UI
void setBrightness(uint8_t brightness) {
    screenOnBrightness = brightness;
    if (isScreenOn) {
        tft.setBrightness(screenOnBrightness);
    }
    lastActivityTime = millis(); // Reset timeout when changing brightness
}

// Safe initialization of MPU sensor
bool init_mpu() {
    try {
        MPU6050Helper::begin(MPU_SDA_PIN, MPU_SCL_PIN);
        Serial.println("MPU6050 initialized successfully");
        return true;
    } catch (...) {
        Serial.println("Failed to initialize MPU6050");
        return false;
    }
}


void setup() {
    Serial.begin(115200);
    delay(100); // Short delay for serial to stabilize
    Serial.println("Smart Beer Tap");

    // Display Prepare
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(2);
    tft.setBrightness(screenOnBrightness);
    lastActivityTime = millis();
    isScreenOn = true;
    delay(100);

    // Initialize LVGL
    lv_init();
    delay(50);

    // LVGL display buffer
    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf1, disp_draw_buf2,
                         screenWidth * screenHeight / 10);
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenHeight;
    disp_drv.ver_res = screenWidth;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.full_refresh = 0;    // only redraw dirty regions
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    // LVGL input
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    // Safe UI initialization
    ui_init();
    ui_initialized = true;
    isInitialStartup = true; // Set initial startup flag
    delay(50);
    
    // Initialize BeerPourHelper
    BeerPourHelper::begin();
    
    // Safe MPU6050 initialization
    sensors_initialized = init_mpu();
    
    Serial.println("Setup done");
}

void loop() {
    // Handle LVGL tasks
    lv_timer_handler();
    
    // Only process sensor data if initialized
    if (sensors_initialized && ui_initialized) {
        // Read sensors
        float tempF = MPU6050Helper::getTemperatureF();
        float zAcc = MPU6050Helper::getZAccel();
        bool currentlyPouring = (zAcc > BEER_ACCEL_THRESHOLD);
        
        // Update screen timeout - wake on pouring
        updateScreenTimeout(currentlyPouring);
        
        // Update beer pour helper
        BeerPourHelper::update(currentlyPouring);
        
        // Get current pour in ounces
        float pouredOz = BeerPourHelper::currentOunces();
        
        // Handle screen transitions
        static bool wasPouring = false;
        
        // When pouring starts, switch to pouring screen
        if (currentlyPouring && !wasPouring && ui_BeerPouring) {
            Serial.println("Pouring started - switching to pouring screen");
            wasPouring = true;
            
            // Wake the screen if it was off
            lastActivityTime = millis();
            if (!isScreenOn) {
                tft.setBrightness(screenOnBrightness);
                isScreenOn = true;
            }
            
            // Load the beer pouring screen
            lv_scr_load(ui_BeerPouring);
        }
        
        // Only update UI if screen is on to save processing
        if (isScreenOn) {
            // Update home screen values if they exist and we're on home screen
            if (lv_scr_act() == ui_Home && ui_temperatureValue && ui_zAxisValue && ui_beerPouringValue) {
                char buf[32];
                // Update temperature
                snprintf(buf, sizeof(buf), "%.1f°F", tempF);
                lv_label_set_text(ui_temperatureValue, buf);
                
                // Update z-axis acceleration
                snprintf(buf, sizeof(buf), "%.2f", zAcc);
                lv_label_set_text(ui_zAxisValue, buf);
                
                // Update beer pouring indicator
                lv_label_set_text(ui_beerPouringValue, currentlyPouring ? "Yes" : "No");
            }
            
            // Update pouring screen values if active
            if (lv_scr_act() == ui_BeerPouring && ui_ozPouredValue && ui_beerPouringBar) {
                char buf[32];
                snprintf(buf, sizeof(buf), "%.1f oz", pouredOz);
                lv_label_set_text(ui_ozPouredValue, buf);
                
                // Update the beer filling bar
                int pct = (int)constrain((pouredOz / 16.0f) * 100.0f, 0.0f, 100.0f);
                lv_bar_set_value(ui_beerPouringBar, pct, LV_ANIM_OFF);
            }
        }
        
        // When pouring stops, update status
        if (!currentlyPouring && wasPouring) {
            wasPouring = false;
            // The values will be maintained by BeerPourHelper for a few seconds
        }
        
        // Return to home after retention period ends and value is reset to 0
        if (!currentlyPouring && !wasPouring && pouredOz == 0 && 
            !BeerPourHelper::isRetaining() && lv_scr_act() == ui_BeerPouring && ui_Home) {
            Serial.println("Retention period ended - returning to home screen");
            lv_scr_load(ui_Home);
            _ui_screen_delete(&ui_BeerPouring); // Delete screen from memory
        }
    }
    
    // Small delay to yield to background tasks
    delay(5);
}

// Function declaration to be called from ui_events.cpp
extern "C" void app_setBrightness(uint8_t value) {
    setBrightness(value);
}