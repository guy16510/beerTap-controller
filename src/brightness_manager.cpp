// src/brightness_manager.cpp
#include "brightness_manager.h"

// Define static variables
uint8_t BrightnessManager::_currentBrightness = SCREEN_NORMAL_BRIGHTNESS;
uint8_t BrightnessManager::_savedBrightness = SCREEN_NORMAL_BRIGHTNESS;
bool BrightnessManager::_isDimmed = false;
uint32_t BrightnessManager::_lastActivityTime = 0;

void BrightnessManager::init() {
    setBrightness(SCREEN_NORMAL_BRIGHTNESS);
    resetActivityTimer();
}

void BrightnessManager::setBrightness(uint8_t value) {
    _currentBrightness = value;
    
    // Use function that's defined in main.cpp
    extern void setDisplayBrightness(uint8_t value);
    setDisplayBrightness(value);
}

uint8_t BrightnessManager::getBrightness() {
    return _currentBrightness;
}

void BrightnessManager::checkActivity(bool isPouring) {
    // If beer is pouring, wake up the screen and reset the timer
    if (isPouring && _isDimmed) {
        resetActivityTimer();
        return;
    }
    
    // If already dimmed, wait for touch to reset
    if (_isDimmed) return;
    
    // Check if timeout reached
    uint32_t now = millis();
    if (now - _lastActivityTime > (SCREEN_TIMEOUT_SECONDS * 1000)) {
        // Save current brightness
        _savedBrightness = _currentBrightness;
        
        // Dim the screen
        setBrightness(SCREEN_DIM_BRIGHTNESS);
        _isDimmed = true;
    }
}

void BrightnessManager::resetActivityTimer() {
    _lastActivityTime = millis();
    
    // If screen was dimmed, restore previous brightness
    if (_isDimmed) {
        setBrightness(_savedBrightness);
        _isDimmed = false;
    }
}