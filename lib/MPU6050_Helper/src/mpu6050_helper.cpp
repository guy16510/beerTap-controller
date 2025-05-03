#include "mpu6050_helper.h"
#include "constants.h"   // for any pin overrides

static Adafruit_MPU6050 _mpu;

void MPU6050Helper::begin(int8_t sdaPin, int8_t sclPin) {
  Wire.begin(sdaPin, sclPin);
  if (!_mpu.begin()) {
    Serial.println("MPU6050 not found!");
    while (1) delay(100);
  }
  // tune ranges as desired
  _mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  _mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  _mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

float MPU6050Helper::getTemperatureF() {
  sensors_event_t accel, gyro, tempEvt;
  _mpu.getEvent(&accel, &gyro, &tempEvt);
  float c = tempEvt.temperature;              // °C
  return c * 9.0f/5.0f + 32.0f;                // °F
}

float MPU6050Helper::getZAccel() {
  sensors_event_t accel, gyro, tempEvt;
  _mpu.getEvent(&accel, &gyro, &tempEvt);
  return accel.acceleration.z;                // m/s²
}