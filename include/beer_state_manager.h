// include/beer_state_manager.h
#ifndef BEER_STATE_MANAGER_H
#define BEER_STATE_MANAGER_H

#include <Arduino.h>

class BeerStateManager {
public:
    static void init();
    static void update();
    static bool isPouring() { return _isPouring; }
    static float getCurrentOunces() { return _currentOunces; }
    
private:
    static bool _isPouring;
    static bool _wasPouring;
    static unsigned long _stopTime;
    static float _currentOunces;
    static float _temperature;
    static float _zAcceleration;
    
    static void updateSensorValues();
    static void updateUI();
    static void handleStateTransitions();
};

#endif // BEER_STATE_MANAGER_H