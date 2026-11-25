# Hardware wiring reference

This is a starter reference for the defaults in `firmware/ESP32_MFD_Flight_Info/config.h` and `extras/Setup_ESP32_ST7789.h`. Confirm the exact breakout-board pin labels and ESP32 board variant before wiring. These values are not confirmation of your final wiring.

## ESP32 signal map

| Module signal | ESP32 pin | Notes |
|---|---:|---|
| I²C SDA (MPU6050 + BMP280) | GPIO 21 | Shared I²C bus |
| I²C SCL (MPU6050 + BMP280) | GPIO 22 | Shared I²C bus |
| GPS TX | GPIO 16 (ESP32 RX2) | GPS TX to ESP32 RX |
| GPS RX | GPIO 17 (ESP32 TX2) | Optional; check logic levels |
| Page button | GPIO 27 | Button between GPIO and GND |
| Flame digital output | GPIO 26 | Set polarity in config.h |
| WS2812 data | GPIO 25 | External supply and common ground |
| Passive buzzer input | GPIO 14 | Use a driver if current requires it |
| Status LED | GPIO 4 | Active level is configurable |

## TFT_eSPI sample wiring

| TFT signal | ESP32 pin |
|---|---:|
| SCLK / CLK | GPIO 18 |
| MOSI / SDA / DIN | GPIO 23 |
| MISO / SDO | GPIO 19 (if required) |
| CS | GPIO 5 |
| DC / A0 | GPIO 2 |
| RST / RES | GPIO 15 |

The supplied profile is for a compatible ST7789 panel only. Some 2.8-inch displays use ILI9341. Confirm the controller and breakout voltage requirements.

## Power and grounding

- Use a properly regulated supply and common ground between ESP32 and modules.
- Do not connect an unregulated Li-ion cell directly to 3V3.
- Power WS2812 LEDs from a supply sized for the strip; join its ground to ESP32 ground.
- Do not power motors, relays, or high-current loads directly from ESP32 GPIO.
- Disconnect power before changing wires and verify voltages with a multimeter.

See `docs/TEST_PLAN.md` for staged bench testing.
