#include "mpu6050_helper.h"
#include "constants.h"   // for any pin overrides

static Adafruit_MPU6050 _mpu;
static bool _initialized = false;
static unsigned long _lastErrorTime = 0;
static const unsigned long _errorThrottle = 5000; // Only log errors every 5 seconds

bool MPU6050Helper::begin(int8_t sdaPin, int8_t sclPin) {
  if (_initialized) return true;
  
  Wire.begin(sdaPin, sclPin);
  
  // Try multiple times to initialize the sensor
  int retries = 3;
  while (retries > 0) {
    if (_mpu.begin()) {
      // Success - configure the sensor
      _mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
      _mpu.setGyroRange(MPU6050_RANGE_500_DEG);
      _mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
      
      _initialized = true;
      Serial.println("MPU6050 initialized successfully");
      return true;
    }
    
    Serial.println("Failed to initialize MPU6050, retrying...");
    delay(100); // Short delay between attempts
    retries--;
  }
  
  Serial.println("MPU6050 initialization failed after multiple attempts");
  return false;
}

float MPU6050Helper::getTemperatureF() {
  if (!_initialized) {
    // Try to initialize if not already
    if (!begin(MPU_SDA_PIN, MPU_SCL_PIN)) {
      return 0.0f; // Default temperature if sensor not available
    }
  }
  
  sensors_event_t accel, gyro, tempEvt;
  
  // Read with error handling
  try {
    _mpu.getEvent(&accel, &gyro, &tempEvt);
  } catch (...) {
    // Only log errors occasionally to avoid serial spam
    unsigned long now = millis();
    if (now - _lastErrorTime > _errorThrottle) {
      Serial.println("Error reading MPU6050 temperature");
      _lastErrorTime = now;
    }
    return 0.0f; // Default value on error
  }
  
  float c = tempEvt.temperature;              // °C
  return c * 9.0f/5.0f + 32.0f;               // °F
}

float MPU6050Helper::getZAccel() {
  if (!_initialized) {
    // Try to initialize if not already
    if (!begin(MPU_SDA_PIN, MPU_SCL_PIN)) {
      return 0.0f; // Default acceleration if sensor not available
    }
  }
  
  sensors_event_t accel, gyro, tempEvt;
  
  // Read with error handling
  try {
    _mpu.getEvent(&accel, &gyro, &tempEvt);
  } catch (...) {
    // Only log errors occasionally to avoid serial spam
    unsigned long now = millis();
    if (now - _lastErrorTime > _errorThrottle) {
      Serial.println("Error reading MPU6050 acceleration");
      _lastErrorTime = now;
    }
    return 0.0f; // Default value on error
  }
  
  return accel.acceleration.z;                // m/s²
}