#ifndef MAX17043_HELPER_H
#define MAX17043_HELPER_H

#include <Wire.h>
#include "constants.h"

class MAX17043Helper {
public:
    // Initialize MAX17043 fuel gauge
    static bool begin(int8_t sdaPin = MPU_SDA_PIN, int8_t sclPin = MPU_SCL_PIN);

    // Read battery voltage in volts, prints value via Serial
    static float getVoltage();

    // Read state of charge in percentage, prints value via Serial
    static float getPercentage();
};

#endif // MAX17043_HELPER_H
