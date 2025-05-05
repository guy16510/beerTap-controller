// src/screen_manager.cpp
#include "screen_manager.h"

ScreenManager::Screen ScreenManager::_currentScreen = ScreenManager::LOADING;

void ScreenManager::init() {
    _currentScreen = LOADING;
}

void ScreenManager::goToScreen(Screen screen) {
    _currentScreen = screen;
    
    // Handle screen transitions
    switch (screen) {
        case LOADING:
            lv_scr_load(ui_Loading);
            break;
        case HOME:
            lv_scr_load(ui_Home);
            break;
        case BEER_POURING:
            lv_scr_load(ui_BeerPouring);
            break;
        case WIFI_CONFIG:
            lv_scr_load(ui_WifiConfig);
            break;
        case QR_CODE:
            lv_scr_load(ui_QRCode);
            break;
        case KEG_CONFIG:
            lv_scr_load(ui_KegConfig);
            break;
        case CONFIGURATION:
            lv_scr_load(ui_Configuration);
            break;
    }
}

ScreenManager::Screen ScreenManager::getCurrentScreen() {
    return _currentScreen;
}