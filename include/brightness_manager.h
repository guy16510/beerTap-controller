// include/brightness_manager.h
#ifndef BRIGHTNESS_MANAGER_H
#define BRIGHTNESS_MANAGER_H

#include <Arduino.h>
#include "constants.h"

class BrightnessManager {
public:
    static void init();
    static void setBrightness(uint8_t value);
    static uint8_t getBrightness();
    
    // For screen dimming after inactivity
    static void checkActivity(bool isPouring);
    static void resetActivityTimer();
    
private:
    static uint8_t _currentBrightness;
    static uint8_t _savedBrightness;
    static bool _isDimmed;
    static uint32_t _lastActivityTime;
};

#endif // BRIGHTNESS_MANAGER_H