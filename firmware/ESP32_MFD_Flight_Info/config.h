#pragma once

// ESP 32 BASED MFD for Flight Info — example configuration.
// Project author and owner: Kaustubh Acharya.
// Verify every setting against the physical hardware before powering up.

// Shared I²C bus: MPU6050 + BMP280
static constexpr int PIN_I2C_SDA = 21;
static constexpr int PIN_I2C_SCL = 22;

// NEO-6M GPS on ESP32 UART2. GPS TX connects to ESP32 RX.
static constexpr int PIN_GPS_RX = 16;
static constexpr int PIN_GPS_TX = 17;
static constexpr uint32_t GPS_BAUD = 9600;

// User interface and digital inputs / outputs
static constexpr int PIN_PAGE_BUTTON = 27; // momentary switch from GPIO to GND
static constexpr int PIN_STATUS_LED = 4;
static constexpr bool STATUS_LED_ACTIVE_HIGH = true;
static constexpr int PIN_BUZZER = 14;
static constexpr int PIN_FLAME_SENSOR = 26;
static constexpr bool FLAME_ACTIVE_LOW = true;

// WS2812 strip: power the strip from a suitable supply, not the GPIO pin.
static constexpr int PIN_NEOPIXEL = 25;
static constexpr uint16_t NEOPIXEL_COUNT = 8;
static constexpr uint8_t NEOPIXEL_BRIGHTNESS = 32;

// Display and scheduling
static constexpr uint8_t TFT_ROTATION = 1; // landscape when profile matches panel
static constexpr uint32_t DISPLAY_REFRESH_MS = 200;
static constexpr uint32_t SENSOR_REFRESH_MS = 50;
static constexpr uint32_t BUTTON_DEBOUNCE_MS = 45;

// Pressure reference for approximate barometric altitude.
static constexpr float SEA_LEVEL_HPA = 1013.25f;

// Disable optional outputs until their hardware and power are verified.
static constexpr bool ENABLE_FLAME_INPUT = true;
static constexpr bool ENABLE_NEOPIXEL = true;
static constexpr bool ENABLE_BUZZER = true;
