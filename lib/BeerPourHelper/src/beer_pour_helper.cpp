
// src/beer_pour_helper.cpp
#include "beer_pour_helper.h"
#include <esp_timer.h> // for esp_timer_get_time()

uint64_t BeerPourHelper::_lastMicros  = 0;
float    BeerPourHelper::_accumOunces = 0.0f;

void BeerPourHelper::begin() {
    _lastMicros   = esp_timer_get_time();
    _accumOunces  = 0.0f;
}

void BeerPourHelper::update(bool pouring) {
    uint64_t now = esp_timer_get_time();
    float dt     = (now - _lastMicros) / 1e6f; // convert microseconds to seconds
    _lastMicros  = now;

    if (pouring) {
        // accumulate volume by calibrated flow rate
        _accumOunces += FLOW_OZ_PER_SEC * dt;
    } else {
        // reset volume when pouring stops
        _accumOunces = 0.0f;
    }
}

float BeerPourHelper::currentOunces() {
    return _accumOunces;
}
