#include "bh1750_helper.h"
#include <Arduino.h>

static BH1750 _lightMeter;
static bool _initialized = false;

bool BH1750Helper::begin(int8_t sdaPin, int8_t sclPin) {
    if (_initialized) return true;
    Wire.begin(sdaPin, sclPin);
    if (_lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
        Serial.println("BH1750 initialized");
        _initialized = true;
        return true;
    } else {
        Serial.println("BH1750 init failed");
        return false;
    }
}

float BH1750Helper::readLux() {
    if (!_initialized && !begin(MPU_SDA_PIN, MPU_SCL_PIN)) {
        return 0.0f;
    }
    float lux = _lightMeter.readLightLevel();
    Serial.print("BH1750 lux: ");
    Serial.println(lux);
    return lux;
}
