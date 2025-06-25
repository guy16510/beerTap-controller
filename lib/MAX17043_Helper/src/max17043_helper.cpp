#include "max17043_helper.h"
#include <Arduino.h>

static const uint8_t MAX17043_ADDRESS = 0x36;
static bool _initialized = false;

// Helper to write two bytes to a register
static bool writeRegister(uint8_t reg, uint16_t value) {
    Wire.beginTransmission(MAX17043_ADDRESS);
    Wire.write(reg);
    Wire.write(value >> 8);
    Wire.write(value & 0xFF);
    return Wire.endTransmission() == 0;
}

bool MAX17043Helper::begin(int8_t sdaPin, int8_t sclPin) {
    if (_initialized) return true;
    Wire.begin(sdaPin, sclPin);
    // Quick start: write 0x4000 to MODE register (0x06)
    if (writeRegister(0x06, 0x4000)) {
        _initialized = true;
        Serial.println("MAX17043 initialized");
        return true;
    } else {
        Serial.println("MAX17043 init failed");
        return false;
    }
}

float MAX17043Helper::getVoltage() {
    if (!_initialized && !begin(MPU_SDA_PIN, MPU_SCL_PIN)) {
        return 0.0f;
    }
    Wire.beginTransmission(MAX17043_ADDRESS);
    Wire.write(0x02); // VCELL MSB
    if (Wire.endTransmission(false) != 0) return 0.0f;
    Wire.requestFrom((uint8_t)MAX17043_ADDRESS, (uint8_t)2);
    if (Wire.available() < 2) return 0.0f;
    uint16_t raw = (Wire.read() << 8) | Wire.read();
    float voltage = (raw >> 4) * 1.25f / 1000.0f; // 1.25mV per bit
    Serial.print("MAX17043 voltage: ");
    Serial.println(voltage);
    return voltage;
}

float MAX17043Helper::getPercentage() {
    if (!_initialized && !begin(MPU_SDA_PIN, MPU_SCL_PIN)) {
        return 0.0f;
    }
    Wire.beginTransmission(MAX17043_ADDRESS);
    Wire.write(0x04); // SOC MSB
    if (Wire.endTransmission(false) != 0) return 0.0f;
    Wire.requestFrom((uint8_t)MAX17043_ADDRESS, (uint8_t)2);
    if (Wire.available() < 2) return 0.0f;
    uint8_t msb = Wire.read();
    uint8_t lsb = Wire.read();
    float soc = msb + lsb / 256.0f;
    Serial.print("MAX17043 SOC: ");
    Serial.println(soc);
    return soc;
}
