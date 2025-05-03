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
static lv_color_t disp_draw_buf1[screenWidth * screenHeight / 10];
static lv_color_t disp_draw_buf2[screenWidth * screenHeight / 10];
static lv_disp_drv_t disp_drv;

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
    Serial.println("Smart Beer Tap");

    // GPIO init (panels)...
    // [unchanged]

    // Display Prepare
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(2);
    delay(200);

    lv_init();
    delay(100);

    // LVGL display buffer
    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf1, disp_draw_buf2,
                          screenWidth * screenHeight / 10);
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res       = screenHeight;
    disp_drv.ver_res       = screenWidth;
    disp_drv.flush_cb      = my_disp_flush;
    disp_drv.full_refresh  = 0;    // only redraw dirty regions
    disp_drv.draw_buf      = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    // LVGL input
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type    = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    // Initialize UI first (ensures UI elements exist)
    ui_init();

    // Initialize helpers after UI
    MPU6050Helper::begin(MPU_SDA_PIN, MPU_SCL_PIN);
    BeerPourHelper::begin();
    
    // Initialize WiFi after UI is ready
    WiFiConfigHelper::begin("wifi");
    
    // Register WiFi status callback before loading credentials
    WiFiConfigHelper::setStatusCallback([](bool connected) {
        if (ui_wifiLoadingSpinner) {
            lv_obj_add_flag(ui_wifiLoadingSpinner, LV_OBJ_FLAG_HIDDEN);
        }
    });
    
    // Only update UI elements if they exist
    String ssid, pass;
    if (WiFiConfigHelper::loadCredentials(ssid, pass)) {
        if (ui_WifiNameValue) {
            lv_textarea_set_text(ui_WifiNameValue, ssid.c_str());
        }
        if (ui_wifiPassword) {
            lv_textarea_set_text(ui_wifiPassword, pass.c_str());
        }
    }

    Serial.println("Setup done");
}

// State for screen navigation
static bool wasPouring        = false;
static unsigned long stopTime = 0;

void loop() {
    lv_timer_handler();

    // 1) Read sensors
    float tempF = MPU6050Helper::getTemperatureF();
    float zAcc  = MPU6050Helper::getZAccel();
    bool currentlyPouring = (zAcc > BEER_ACCEL_THRESHOLD);
    BeerPourHelper::update(currentlyPouring);

    // Only update UI elements if they exist
    if (ui_temperatureValue && ui_zAxisValue) {
        char buf[32];
        // 2) Update UI labels for temp & accel
        snprintf(buf, sizeof(buf), "%.1f°F", tempF);
        lv_label_set_text(ui_temperatureValue, buf);
        snprintf(buf, sizeof(buf), "%.2f", zAcc);
        lv_label_set_text(ui_zAxisValue, buf);
    }

    // 3) Get current pour in ounces
    float pouredOz = BeerPourHelper::currentOunces();  // already in oz!

    // 4) Handle screen transitions
    if (currentlyPouring && !wasPouring && ui_BeerPouring) {
        wasPouring = true;
        stopTime   = 0;
        lv_scr_load(ui_BeerPouring);
    }

    // 5) While pouring: update oz label & bar%
    if (wasPouring && ui_ozPouredValue && ui_beerPouringBar) {
        char buf[32];
        // show ounces poured
        snprintf(buf, sizeof(buf), "%.1f oz", pouredOz);
        lv_label_set_text(ui_ozPouredValue, buf);

        // fill bar as percent of 16 oz (1 pint)
        int pct = (int)constrain((pouredOz / 16.0f) * 100.0f, 0.0f, 100.0f);
        lv_bar_set_value(ui_beerPouringBar, pct, LV_ANIM_OFF);
    }

    // 6) End pouring: schedule return
    if (!currentlyPouring && wasPouring) {
        wasPouring = false;
        stopTime   = millis();
    }
    if (stopTime && !currentlyPouring && (millis() - stopTime > 2000) && ui_Home) {
        lv_scr_load(ui_Home);
        stopTime = 0;
    }

    // no blocking delay
}