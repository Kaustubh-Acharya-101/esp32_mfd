# Bench Test and Validation Plan

**Project owner: Kaustubh Acharya**

Perform these tests in stages. Keep motors and propellers disconnected. Do not use this prototype for flight-critical decisions.

## Stage 1 — visual and power inspection

- [ ] Confirm module labels, pinout, and voltage requirements.
- [ ] Inspect polarity, connectors, solder joints, and exposed conductors.
- [ ] Verify each regulated rail with a multimeter before plugging in the ESP32.
- [ ] Verify common ground where signals cross between modules.
- [ ] Leave the WS2812 strip disconnected during first display bring-up.

## Stage 2 — ESP32 and TFT

- [ ] Confirm the USB cable supports data.
- [ ] Select the correct ESP32 board and COM port.
- [ ] Verify sketch compilation before upload.
- [ ] Confirm TFT controller and TFT_eSPI profile match.
- [ ] Confirm landscape orientation and screen bounds.
- [ ] Check that the startup title and both page layouts are visible.

## Stage 3 — I²C sensors

- [ ] Connect the MPU6050 and scan the I²C bus.
- [ ] Confirm the MPU6050 at the expected address.
- [ ] Connect the BMP280 and confirm its address.
- [ ] Confirm Serial Monitor reports the sensors online.
- [ ] Tilt the stationary board slowly and check pitch/roll direction.
- [ ] Treat pressure altitude as approximate.

## Stage 4 — GPS

- [ ] Connect GPS TX to ESP32 RX2 (GPIO 16 by default).
- [ ] Confirm the configured baud rate (9600 by default).
- [ ] Inspect incoming serial data or parser updates.
- [ ] Test outdoors with a clear sky view.
- [ ] Wait for a valid fix before interpreting coordinates.

## Stage 5 — button and alert inputs

- [ ] Connect the page button between GPIO 27 and GND.
- [ ] Confirm one page change per press.
- [ ] Confirm the switch does not cause repeated page changes.
- [ ] Verify flame sensor output polarity before enabling buzzer output.
- [ ] Use a safe, controlled test method for the flame sensor.

## Stage 6 — LEDs and buzzer

- [ ] Confirm the WS2812 supply voltage and current capacity.
- [ ] Confirm common ground and correct DIN direction.
- [ ] Use a suitable logic-level interface if required by the LED strip.
- [ ] Confirm the buzzer's current requirement; add a driver if necessary.
- [ ] Check component temperature during operation.

## Stage 7 — integration observation

- [ ] Run the complete system on a bench supply for at least 15 minutes.
- [ ] Observe resets, brownouts, screen corruption, and component heating.
- [ ] Record actual measured voltages and confirmed wiring.
- [ ] Repeat after firmware, wiring, or library changes.

Document the board model, library versions, compiler output, and test observations for each revision.
