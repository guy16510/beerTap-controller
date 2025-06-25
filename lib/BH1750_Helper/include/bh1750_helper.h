#ifndef BH1750_HELPER_H
#define BH1750_HELPER_H

#include <BH1750.h>
#include <Wire.h>
#include "constants.h"

class BH1750Helper {
public:
    // Initialize BH1750 sensor on the given I2C pins
    static bool begin(int8_t sdaPin = MPU_SDA_PIN, int8_t sclPin = MPU_SCL_PIN);

    // Read light level in lux, prints value via Serial
    static float readLux();
};

#endif // BH1750_HELPER_H
