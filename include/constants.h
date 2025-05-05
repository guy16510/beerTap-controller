#pragma once

// Screen timeout configuration
#define SCREEN_TIMEOUT_SECONDS    10    // Seconds of inactivity before screen turns off
#define SCREEN_DIM_BRIGHTNESS     0     // Brightness level when screen is "off" (0 = completely off)
#define SCREEN_NORMAL_BRIGHTNESS  200   // Normal brightness level when screen is on

// ─── Beer pour configuration ────────────────────────────────────────────────
#define BEER_SECONDS_PER_PINT   10   // 1 pint every 10 seconds
#define BEER_POUR_INPUT_PIN     4    // digital pin you use to sense POURING?
                                         // (e.g. a flow sensor or switch)

// ─── MPU6050 pins (override defaults) ────────────────────────────────────────
#define MPU_SDA_PIN             38
#define MPU_SCL_PIN             18

// ─── Beer-pour accel threshold ───────────────────────────────────────────────
#define BEER_ACCEL_THRESHOLD   2.0f   // if z-axis accel > this, we consider a pour