// lib/BeerPourHelper/src/beer_pour_helper.cpp
#include "beer_pour_helper.h"
#include <esp_timer.h> // for esp_timer_get_time()

uint64_t BeerPourHelper::_lastMicros  = 0;
float    BeerPourHelper::_accumOunces = 0.0f;
bool     BeerPourHelper::_wasPouring  = false;
uint64_t BeerPourHelper::_stopTimerUs = 0;
const uint64_t BeerPourHelper::_RETAIN_TIME_US = 1000000; // 3 seconds in microseconds

void BeerPourHelper::begin() {
    _lastMicros   = esp_timer_get_time();
    _accumOunces  = 0.0f;
    _wasPouring   = false;
    _stopTimerUs  = 0;
}

void BeerPourHelper::update(bool pouring) {
    uint64_t now = esp_timer_get_time();
    float dt     = (now - _lastMicros) / 1e6f; // convert microseconds to seconds
    _lastMicros  = now;

    if (pouring) {
        // accumulate volume by calibrated flow rate
        _accumOunces += FLOW_OZ_PER_SEC * dt;
        _wasPouring = true;
        _stopTimerUs = 0; // Reset the stop timer while pouring
    } else {
        // Start retention timer when pouring stops
        if (_wasPouring && _stopTimerUs == 0) {
            _stopTimerUs = now;
        }
        
        // Only reset the ounces if the retention time has passed
        if (_stopTimerUs > 0 && (now - _stopTimerUs > _RETAIN_TIME_US)) {
            _accumOunces = 0.0f;
            _wasPouring = false;
            _stopTimerUs = 0;
        }
    }
}

float BeerPourHelper::currentOunces() {
    return _accumOunces;
}

bool BeerPourHelper::isRetaining() {
    return _stopTimerUs > 0;
}