# Bench test and validation plan

Perform these checks with motors/propellers disconnected. This MFD is supplementary telemetry, not flight-certified avionics.

## Stage 1 — power and inspection

- [ ] Inspect polarity, connectors, solder joints, and exposed conductors.
- [ ] Verify regulated voltages with a multimeter before connecting the ESP32.
- [ ] Verify common ground.
- [ ] Keep WS2812 strip disconnected for the first display test.

## Stage 2 — display

- [ ] Connect only ESP32 and TFT.
- [ ] Confirm TFT controller and TFT_eSPI profile match.
- [ ] Verify startup screen and landscape orientation.
- [ ] Confirm all text fits inside 320 × 240.

## Stage 3 — I²C sensors

- [ ] Connect MPU6050 and BMP280.
- [ ] Confirm both are detected in Serial Monitor.
- [ ] Tilt the stationary board slowly and check pitch/roll directions.
- [ ] Treat pressure altitude as approximate.

## Stage 4 — GPS

- [ ] Connect GPS TX to ESP32 RX2 (GPIO 16 by default).
- [ ] Confirm baud rate (9600 by default).
- [ ] Test outdoors with clear sky view.
- [ ] Wait for a valid fix before relying on displayed coordinates.

## Stage 5 — controls and alerts

- [ ] Connect button between GPIO 27 and GND (or adjust config.h).
- [ ] Confirm one page change per press.
- [ ] Verify flame input polarity using a safe module test method.
- [ ] Add WS2812 only after checking supply and current budget.

## Stage 6 — integration

- [ ] Run at least 15 minutes on the bench.
- [ ] Check resets, brownouts, display corruption, and component heating.
- [ ] Record measured voltages and actual wiring.
- [ ] Repeat after code, wiring, or library changes.

Do not use this prototype for flight-critical decisions.
