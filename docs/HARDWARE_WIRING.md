# Hardware Wiring Reference

**Project author and owner: Kaustubh Acharya**

This is a reference for the example defaults in `firmware/ESP32_MFD_Flight_Info/config.h` and `extras/Setup_ESP32_ST7789.h`. The pin values are not a confirmation of the wiring on your particular board. Check the board pinout and module documentation before connecting anything.

## 1. ESP32 signal map

| Module signal | ESP32 pin | Interface / explanation |
|---|---:|---|
| MPU6050 SDA and BMP280 SDA | GPIO 21 | Shared I²C data line |
| MPU6050 SCL and BMP280 SCL | GPIO 22 | Shared I²C clock line |
| GPS TX | GPIO 16 (ESP32 RX2) | UART data into ESP32 |
| GPS RX | GPIO 17 (ESP32 TX2) | Optional UART data from ESP32 |
| Page button | GPIO 27 | Button between GPIO and GND; INPUT_PULLUP |
| Flame digital output | GPIO 26 | Polarity configured in config.h |
| WS2812 data | GPIO 25 | Data signal only; external supply may be needed |
| Passive buzzer | GPIO 14 | Check current; use transistor/driver if needed |
| Status LED | GPIO 4 | Onboard or external status indicator |

## 2. TFT_eSPI sample connections

| TFT signal | ESP32 pin |
|---|---:|
| SCLK / CLK | GPIO 18 |
| MOSI / SDA / DIN | GPIO 23 |
| MISO / SDO | GPIO 19, if required by panel |
| CS | GPIO 5 |
| DC / A0 | GPIO 2 |
| RST / RES | GPIO 15 |

The supplied profile is for an ST7789-compatible panel. Some 2.8-inch screens use ILI9341 or another controller. The dimensions, offsets, backlight wiring, and voltage tolerance depend on the exact display breakout.

## 3. How the buses are wired

### I²C sensors

Connect SDA to SDA, SCL to SCL, and GND to GND. Both sensors share the same bus. The MPU6050 is initialised at 0x68; the BMP280 code tries 0x76 and 0x77. Do not connect two devices with the same address unless the bus is designed to handle it.

### GPS UART

Connect GPS TX to ESP32 RX2 (GPIO 16). Connect GPS RX to ESP32 TX2 (GPIO 17) only if configuration is required. TX and RX are crossed because one device's output must feed the other device's input. Confirm the GPS breakout's supply and logic-level requirements.

### Page button

The firmware uses INPUT_PULLUP. Wire one switch terminal to GPIO 27 and the other terminal to GND. Do not connect the switch to 3.3 V for this arrangement.

### WS2812 LEDs

Connect the ESP32 data output to the strip's DIN, not DOUT. Power the strip from a supply sized for the strip's voltage and current. Connect the supply ground to ESP32 ground. A suitable level shifter may be required depending on the strip and supply voltage.

## 4. Power distribution

The block diagram shows power separately from data signals. Design the rails for the actual parts:

- A Li-ion charger must match the cell chemistry and charging current.
- Charger modules are not necessarily voltage regulators or power-path controllers.
- Use suitable regulated rails for the ESP32 board, sensors, display, and any LED strip.
- Do not connect an unregulated cell directly to the ESP32 3V3 pin.
- Do not assume every ESP32 board's VIN pin accepts the same input range.
- Do not draw WS2812 strip current from an ESP32 GPIO.
- Check voltage and polarity with a multimeter before attaching the modules.

## 5. Safe wiring sequence

1. Disconnect the battery and USB.
2. Wire only the ESP32 and TFT; verify the display controller and profile.
3. Add one I²C sensor at a time and scan the bus.
4. Add GPS and check Serial Monitor.
5. Add the button and flame input.
6. Add the WS2812 and buzzer only after their power requirements are verified.

Keep motors and propellers disconnected during bench testing.
