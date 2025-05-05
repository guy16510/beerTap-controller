// include/screen_manager.h
#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <Arduino.h>
#include <lvgl.h>
#include "ui/ui.h"

class ScreenManager {
public:
    // Screen IDs
    enum Screen {
        LOADING,
        HOME,
        BEER_POURING,
        WIFI_CONFIG,
        QR_CODE,
        KEG_CONFIG,
        CONFIGURATION
    };
    
    static void init();
    static void goToScreen(Screen screen);
    static Screen getCurrentScreen();
    
private:
    static Screen _currentScreen;
};

#endif // SCREEN_MANAGER_H