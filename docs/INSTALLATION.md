# Installation and first upload

## 1. Install Arduino IDE and ESP32 support

Install Arduino IDE 2.x. In Preferences, add this Boards Manager URL:

```text
https://espressif.github.io/arduino-esp32/package_esp32_index.json
```

Open Boards Manager, search for `esp32`, and install **esp32 by Espressif Systems**. Select the board matching your module (the common ESP32 Dev Module is only a starting selection).

## 2. Install libraries

In **Sketch → Include Library → Manage Libraries**, install:

- TFT_eSPI by Bodmer
- Adafruit MPU6050
- Adafruit BMP280 Library
- Adafruit Unified Sensor
- TinyGPSPlus by Mikal Hart
- Adafruit NeoPixel

## 3. Configure TFT_eSPI

The repository includes `extras/Setup_ESP32_ST7789.h`, a sample ST7789 profile.

1. Find your Arduino libraries folder, commonly `Documents/Arduino/libraries/TFT_eSPI/`.
2. Copy the supplied setup header into `TFT_eSPI/User_Setups/`.
3. Edit `TFT_eSPI/User_Setup_Select.h`: comment out the active `#include <User_Setup.h>` line and add:

```cpp
#include <User_Setups/Setup_ESP32_ST7789.h>
```

4. Save and restart Arduino IDE.

Do not use this profile unless your display is ST7789 with matching pinout and dimensions.

## 4. Open and configure firmware

Open `firmware/ESP32_MFD_Flight_Info/ESP32_MFD_Flight_Info.ino`. Keep `config.h` in the same sketch folder. Compare every GPIO against your actual wiring before power-up.

Select the correct ESP32 board and COM port. Click **Verify** before **Upload**. Open Serial Monitor at 115200 baud.

## 5. First boot

The TFT should show the project startup screen and then Page 1. Missing I²C sensors are reported as offline. GPS can take time to acquire a valid fix and may not work indoors.
