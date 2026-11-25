# Installation and First Upload

**Project:** ESP 32 BASED MFD for Flight Info  
**Author:** Kaustubh Acharya

## 1. Install Arduino IDE

Install Arduino IDE 2.x. Open Preferences and add the ESP32 Boards Manager URL:

```text
https://espressif.github.io/arduino-esp32/package_esp32_index.json
```

Open Boards Manager, search for `esp32`, and install **esp32 by Espressif Systems**. Select the exact board variant when known. `ESP32 Dev Module` is a common starting profile, not a universal setting.

## 2. Install the libraries

Use **Sketch → Include Library → Manage Libraries** and install:

- TFT_eSPI by Bodmer
- Adafruit MPU6050
- Adafruit BMP280 Library
- Adafruit Unified Sensor
- TinyGPSPlus by Mikal Hart
- Adafruit NeoPixel

Restart Arduino IDE after installing libraries if the IDE does not detect them.

## 3. Configure the TFT_eSPI library

The repository includes `extras/Setup_ESP32_ST7789.h`, a sample profile for an ST7789 display.

1. Locate the Arduino libraries folder, often `Documents/Arduino/libraries/TFT_eSPI/`.
2. Copy the setup file into `TFT_eSPI/User_Setups/`.
3. Open `TFT_eSPI/User_Setup_Select.h`.
4. Comment out the active default setup include and add:

```cpp
#include <User_Setups/Setup_ESP32_ST7789.h>
```

5. Save and restart Arduino IDE.

Do not select this profile unless the display is confirmed to use ST7789 and the pinout matches. A display using ILI9341 requires a different driver configuration. Some TFTs also need different offsets or dimensions.

## 4. Open the sketch

Open:

```text
firmware/ESP32_MFD_Flight_Info/ESP32_MFD_Flight_Info.ino
```

Keep `config.h` in the same sketch folder. Compare all GPIO assignments in that file against the actual wiring. Change the constants before powering the circuit if the pins differ.

## 5. Select board and COM port

1. Connect the ESP32 using a known-good USB data cable.
2. Select the correct ESP32 board in **Tools → Board**.
3. Select the COM port in **Tools → Port**.
4. Click **Verify** to compile.
5. If compilation succeeds, click **Upload**.
6. Open Serial Monitor at 115200 baud.

The GPS UART defaults to 9600 baud, independently of the USB Serial Monitor baud rate.

## 6. First boot expectation

The TFT should show the project title and author, then Page 1. Serial Monitor should report whether the MPU6050 and BMP280 were detected. GPS coordinates may remain in a waiting-for-fix state until the receiver gets a valid outdoor fix.

A successful upload only proves that the firmware was transferred. It does not prove that every sensor is wired or operating correctly.
