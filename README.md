# ESP 32 BASED MFD for Flight Info

**An ESP32-powered, two-page multi-function display prototype for supplementary flight information.**
**GitHub:** [Kaustubh-Acharya-101](https://github.com/Kaustubh-Acharya-101)

> **Safety notice:** Educational hardware-integration prototype for bench testing and supplementary telemetry only. It is not a certified aircraft instrument, flight controller, primary navigation source, or safety-critical system. Never rely on it as the sole source for flight decisions.

## Features

- **Page 1 — Flight information:** MPU6050 pitch/roll estimate, relative yaw-rate integration, and BMP280 pressure/altitude.
- **Page 2 — Navigation and status:** NEO-6M GPS coordinates, satellite count, barometric data, and flame-sensor state.
- Physical pushbutton for page switching.
- Optional WS2812 status LEDs and passive-buzzer alert.
- Central GPIO configuration in `firmware/ESP32_MFD_Flight_Info/config.h`.

## Hardware baseline

| Component | Function | Interface |
|---|---|---|
| ESP32 DevKit (30-pin) | Main controller | — |
| 2.8-inch SPI TFT, 320 × 240 | Display; ST7789 sample setup provided | SPI |
| MPU6050 | Pitch/roll estimate and relative yaw rate | I²C |
| BMP280 | Pressure and estimated altitude | I²C |
| NEO-6M | GPS coordinates and satellite data | UART2 |
| BY-029 / KY-026 | Flame indication | Digital GPIO |
| WS2812 strip | Status indication | GPIO data |
| Passive buzzer | Audible warning | GPIO |

The ST7789 profile is a sample only. Confirm the TFT controller, pinout, voltage limits, and ESP32 board variant before connecting hardware.

## Quick start

1. Install Arduino IDE 2.x and ESP32 board support.
2. Install TFT_eSPI, Adafruit MPU6050, Adafruit BMP280 Library, Adafruit Unified Sensor, TinyGPSPlus, and Adafruit NeoPixel.
3. Configure TFT_eSPI using `extras/Setup_ESP32_ST7789.h` and follow `docs/INSTALLATION.md`.
4. Open `firmware/ESP32_MFD_Flight_Info/ESP32_MFD_Flight_Info.ino`.
5. Check every GPIO in `config.h` against your actual wiring.
6. Select the correct ESP32 board and COM port. Verify the sketch before uploading.
7. Follow `docs/TEST_PLAN.md` for staged bench validation.

## Repository contents

- `firmware/` — ESP32 Arduino sketch and GPIO configuration.
- `docs/HARDWARE_WIRING.md` — starter wiring reference.
- `docs/INSTALLATION.md` — Arduino IDE and library setup.
- `docs/TEST_PLAN.md` — bench test checklist.
- `docs/KNOWN_LIMITATIONS.md` — known limitations and next steps.
- `extras/` — sample TFT_eSPI ST7789 profile.
- `CONTRIBUTING.md`, `CHANGELOG.md`, `CITATION.cff`, and `LICENSE`.

## Important engineering limitations

- MPU6050 has no magnetometer. Integrated yaw is relative and drifts; it is not compass heading.
- Accelerometer-only pitch/roll are affected by aircraft acceleration and vibration.
- BMP280 altitude is approximate and depends on reference pressure and weather.
- GPS requires a valid satellite fix; indoor acquisition may fail.
- GPIO defaults are examples, not confirmation of your physical wiring.
- Use a properly regulated supply, verify common ground, and do not power high-current loads from ESP32 GPIO pins.

Read `docs/KNOWN_LIMITATIONS.md` and `docs/HARDWARE_WIRING.md` before bench testing.

## Maintainer

**Kaustubh Acharya**  
ESP 32 BASED MFD for Flight Info · SRM–IST
