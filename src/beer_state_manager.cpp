// src/beer_state_manager.cpp
#include "beer_state_manager.h"
#include "mpu6050_helper.h"
#include "beer_pour_helper.h"
#include "constants.h"
#include "ui/ui.h"

bool BeerStateManager::_isPouring = false;
bool BeerStateManager::_wasPouring = false;
unsigned long BeerStateManager::_stopTime = 0;
float BeerStateManager::_currentOunces = 0.0f;
float BeerStateManager::_temperature = 0.0f;
float BeerStateManager::_zAcceleration = 0.0f;

void BeerStateManager::init() {
    _isPouring = false;
    _wasPouring = false;
    _stopTime = 0;
    _currentOunces = 0.0f;
}

void BeerStateManager::update() {
    updateSensorValues();
    updateUI();
    handleStateTransitions();
}

void BeerStateManager::updateSensorValues() {
    // Read sensor values
    _temperature = MPU6050Helper::getTemperatureF();
    _zAcceleration = MPU6050Helper::getZAccel();
    _isPouring = (_zAcceleration > BEER_ACCEL_THRESHOLD);
    
    // Update beer pour helper
    BeerPourHelper::update(_isPouring);
    _currentOunces = BeerPourHelper::currentOunces();
}

void BeerStateManager::updateUI() {
    // Update temperature display
    if (ui_temperatureValue) {
        char buf[32];
        snprintf(buf, sizeof(buf), "%.1f°F", _temperature);
        lv_label_set_text(ui_temperatureValue, buf);
    }
    
    // Update Z-axis acceleration display
    if (ui_zAxisValue) {
        char buf[32];
        snprintf(buf, sizeof(buf), "%.2f", _zAcceleration);
        lv_label_set_text(ui_zAxisValue, buf);
    }
    
    // Update beer pouring indicator on home screen
    if (ui_beerPouringValue) {
        lv_label_set_text(ui_beerPouringValue, _isPouring ? "Yes" : "No");
    }
    
    // Update pouring screen if active
    if (_isPouring && ui_ozPouredValue && ui_beerPouringBar) {
        char buf[32];
        // show ounces poured
        snprintf(buf, sizeof(buf), "%.1f oz", _currentOunces);
        lv_label_set_text(ui_ozPouredValue, buf);

        // fill bar as percent of 16 oz (1 pint)
        int pct = (int)constrain((_currentOunces / 16.0f) * 100.0f, 0.0f, 100.0f);
        lv_bar_set_value(ui_beerPouringBar, pct, LV_ANIM_OFF);
    }
}

void BeerStateManager::handleStateTransitions() {
    // Start pouring: transition to pouring screen
    if (_isPouring && !_wasPouring && ui_BeerPouring) {
        lv_scr_load(ui_BeerPouring);
        _stopTime = 0;
    }
    
    // Stop pouring: schedule return to home
    if (!_isPouring && _wasPouring) {
        _stopTime = millis();
    }
    
    // Return to home after delay
    if (_stopTime > 0 && !_isPouring && (millis() - _stopTime > 2000) && ui_Home) {
        lv_scr_load(ui_Home);
        _stopTime = 0;
    }
    
    _wasPouring = _isPouring;
}