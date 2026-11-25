#pragma once

// ESP 32 BASED MFD for Flight Info — example configuration.
// Verify every pin against the physical wiring before powering the project.

// I2C: MPU6050 + BMP280
static constexpr int PIN_I2C_SDA = 21;
static constexpr int PIN_I2C_SCL = 22;

// NEO-6M GPS on ESP32 UART2
static constexpr int PIN_GPS_RX = 16; // ESP32 RX <- GPS TX
static constexpr int PIN_GPS_TX = 17; // ESP32 TX -> GPS RX (optional)
static constexpr uint32_t GPS_BAUD = 9600;

// User interface and indicators
static constexpr int PIN_PAGE_BUTTON = 27; // momentary button from GPIO to GND
static constexpr int PIN_STATUS_LED = 4;
static constexpr bool STATUS_LED_ACTIVE_HIGH = true;
static constexpr int PIN_BUZZER = 14;
static constexpr int PIN_FLAME_SENSOR = 26;
static constexpr bool FLAME_ACTIVE_LOW = true;

// WS2812 strip
static constexpr int PIN_NEOPIXEL = 25;
static constexpr uint16_t NEOPIXEL_COUNT = 8;
static constexpr uint8_t NEOPIXEL_BRIGHTNESS = 32;

// Display / timing
static constexpr uint8_t TFT_ROTATION = 1; // landscape; logical size 320 x 240
static constexpr uint32_t DISPLAY_REFRESH_MS = 200;
static constexpr uint32_t SENSOR_REFRESH_MS = 50;
static constexpr uint32_t BUTTON_DEBOUNCE_MS = 45;

// Environmental reference pressure
static constexpr float SEA_LEVEL_HPA = 1013.25f;

// Enable only after confirming each module exists and is wired correctly.
static constexpr bool ENABLE_FLAME_INPUT = true;
static constexpr bool ENABLE_NEOPIXEL = true;
static constexpr bool ENABLE_BUZZER = true;
