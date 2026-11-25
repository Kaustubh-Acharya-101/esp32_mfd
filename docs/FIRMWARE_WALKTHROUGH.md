# Firmware Walkthrough
**Project:** ESP 32 BASED MFD for Flight Info  
**Author:** Kaustubh Acharya

This page explains how the Arduino sketch is organised. It is intended to be read while the source code is open.

## 1. Header files

The sketch includes:

- `Arduino.h` for Arduino core types and GPIO functions.
- `Wire.h` for the I²C bus.
- `TFT_eSPI.h` for drawing to the TFT.
- Adafruit MPU6050 and BMP280 libraries for sensor access.
- `TinyGPSPlus.h` for parsing GPS NMEA data.
- `Adafruit_NeoPixel.h` for WS2812 LEDs.
- `config.h` for all project pin and timing constants.

Keeping hardware settings in a separate header means you can change the GPIO map without searching through every display function.

## 2. Global objects and variables

The sketch creates one object for each hardware library: `tft`, `mpu`, `bmp`, `gps`, and `pixels`.

Variables such as `imuReady` and `baroReady` record whether sensor initialisation succeeded. The display uses these flags to show an offline or warning state instead of treating unavailable measurements as valid.

The attitude values, pressure, altitude, and temperature are stored so that the acquisition code and display code can use the same latest sample.

## 3. setup(): initialise before using

Arduino calls `setup()` once after reset. This project:

1. configures the status LED, page button, flame input, and buzzer pins;
2. starts the optional WS2812 strip;
3. starts USB serial and GPS UART2;
4. initialises the TFT and draws a startup screen;
5. starts I²C on the configured SDA/SCL pins;
6. attempts to initialise the MPU6050 and BMP280;
7. configures sensor ranges and BMP280 sampling;
8. reports sensor detection results to Serial Monitor.

A successful upload does not prove that all sensors are wired correctly. Always read the startup diagnostics.

## 4. updateGps(): parse the GPS stream

The NEO-6M sends serial bytes. `updateGps()` reads available bytes from `Serial2` and passes them to `gps.encode()`.

TinyGPSPlus builds a parsed representation from the NMEA stream. The display should only show a location when `gps.location.isValid()` returns true.

For a robust system, also check when the last valid fix was updated. A location can become stale if the receiver stops sending valid data.

## 5. updateSensors(): acquire and interpret measurements

The IMU provides accelerometer and gyroscope events. Accelerometer X/Y/Z values are used in the current sketch for a simple pitch/roll estimate. Gyroscope Z is integrated over elapsed time to give relative yaw change.

The barometer provides pressure and temperature. `readAltitude(SEA_LEVEL_HPA)` estimates altitude using the reference pressure configured in `config.h`.

The flame input is sampled as a digital signal. The active-low setting in `config.h` determines whether LOW means active.

This function does not perform a full calibration routine or validated sensor fusion. Treat the derived attitude and altitude as experimental display values.

## 6. Drawing functions

The UI is separated into drawing functions:

- `drawHeader()` draws the page header.
- `drawFooter()` draws the project identity and system status.
- `drawLabelValue()` renders a label with its current value.
- `drawArtificialHorizon()` draws the attitude-style symbol.
- `drawPfdPage()` renders Page 1.
- `drawNavigationPage()` renders Page 2.

The sketch refreshes the selected page periodically. The screen is cleared and redrawn; a future optimisation could redraw only the fields that changed.

## 7. Button and buzzer logic

The page button is configured with `INPUT_PULLUP`. The button normally reads HIGH. A press connects the pin to GND and reads LOW.

Mechanical buttons can bounce. The code waits until the input has remained stable for the configured debounce time before changing the page.

The buzzer handler checks the flame state and triggers a short tone at a limited interval. A real warning design should also consider false triggers, sensor diagnostics, and safe operating procedures.

## 8. loop(): repeat without long blocking delays

The main loop:

- feeds GPS data to the parser;
- checks the page button;
- handles buzzer alerts;
- runs sensor acquisition when its interval expires;
- redraws the selected page when its display interval expires.

Using `millis()` lets several activities progress without a long blocking delay for each one. Keep time intervals short enough for the data rate, but do not refresh the TFT unnecessarily.

## 9. Suggested code-reading exercise

1. Find `PIN_I2C_SDA` in `config.h`.
2. Find where `Wire.begin()` uses it in `setup()`.
3. Find `mpu.getEvent()` in `updateSensors()`.
4. Follow `pitchDeg` into `drawPfdPage()`.
5. Find the page-toggle code in `handlePageButton()`.
6. Follow `pageTwo` into the branch in `loop()`.

This traces a complete path from a physical pin, to a sensor reading, to a calculated variable, to a pixel on the display.
