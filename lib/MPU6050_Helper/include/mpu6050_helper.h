#ifndef MPU6050_HELPER_H
#define MPU6050_HELPER_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

class MPU6050Helper {
public:
  // Call once in setup()
  static void begin(int8_t sdaPin = 38, int8_t sclPin = 18);

  // Reads sensor and returns temperature in °F
  static float getTemperatureF();

  // Reads sensor and returns Z-axis accel in m/s²
  static float getZAccel();
};

#endif // MPU6050_HELPER_H