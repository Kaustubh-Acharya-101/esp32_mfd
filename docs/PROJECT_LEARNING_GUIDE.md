# Project Learning Guide
## ESP 32 BASED MFD for Flight Info
**Author and project owner: Kaustubh Acharya**

This guide teaches the complete project from the system idea to the sensor readings on the TFT. Read it alongside the vector block diagram in the repository's first page.

---

## 1. What is an MFD?

MFD means **Multi-Function Display**. It is a display system that presents information from more than one electronic subsystem on a single screen. Instead of using separate displays for attitude, pressure altitude, navigation, and warnings, an MFD organises the information into pages.

This project is a small educational version of that idea. The ESP32 receives measurements from sensors, processes them, and draws two pages on a 320 × 240 TFT.

The ESP32 does not make the aircraft fly. It is only the display computer in this project.

## 2. Understand the complete system

Think of the project as four stages:

1. **Inputs:** sensors and the page button provide raw measurements or digital states.
2. **Controller:** the ESP32 reads the inputs and runs the program.
3. **Processing:** software turns raw values into display values, such as pitch, roll, pressure altitude, and GPS coordinates.
4. **Outputs:** the TFT, status LED, WS2812 strip, buzzer, and USB serial monitor present the result.

The block diagram shows both signal paths and power paths. A signal wire carries data or a logic level. A power wire supplies electrical energy. They are related, but one cannot replace the other.

## 3. The ESP32: the main controller

The ESP32 is the central processing unit of this MFD. The Arduino sketch runs on it and performs tasks such as:

- initialising the display and sensor libraries;
- reading the MPU6050 and BMP280 over I²C;
- receiving GPS serial bytes;
- checking the button and flame input;
- choosing the active page;
- updating the screen and warning outputs.

The ESP32 DevKit is a board containing the ESP32 microcontroller plus supporting components such as a USB interface and voltage regulation. Board variants differ, so check the exact pinout of your board.

ESP32 GPIO uses 3.3 V logic. Do not assume that a module's 5 V output is safe for an ESP32 input. Check the breakout-board schematic or datasheet.

## 4. Sensor modules — what each one measures

### 4.1 MPU6050 — inertial measurement

The MPU6050 combines:

- a **three-axis accelerometer**, which measures acceleration along X, Y, and Z;
- a **three-axis gyroscope**, which measures angular velocity around X, Y, and Z.

In this firmware it communicates over I²C at address 0x68 by default.

The accelerometer can estimate tilt while the board is still or moving gently. The code uses the acceleration components to estimate two angles:

[
\text{Roll} = \operatorname{atan2}(a_y,a_z)
]

[
\text{Pitch} = \operatorname{atan2}(-a_x,\sqrt{a_y^2+a_z^2})
]

The firmware converts the results from radians to degrees.

**Why does this work?** When the board is stationary, gravity is the dominant acceleration measured by the sensor. The direction of the gravity vector changes relative to the sensor axes when the board tilts.

**Important limitation:** during fast movement, vibration, or acceleration, the accelerometer measures both gravity and motion. The simple equations can therefore show a false tilt. The current sketch does not yet implement a validated complementary or Kalman filter.

The gyroscope measures angular velocity. The code integrates the Z-axis angular rate over time:

[
\psi_{k}=\psi_{k-1}+\omega_z\Delta t
]

This gives a relative yaw change. Small gyro bias accumulates over time, so the value drifts. The MPU6050 has no magnetometer; this project does not have an absolute magnetic heading.

### 4.2 BMP280 — pressure and temperature

The BMP280 measures atmospheric pressure and temperature. It communicates over the same I²C bus as the MPU6050, but uses a different I²C address, commonly 0x76 or 0x77.

Pressure decreases as altitude increases. The library estimates altitude using a reference sea-level pressure. A commonly used approximation is:

[
h \approx 44330\left(1-\left(\frac{p}{p_0}\right)^{1/5.255}\right)
]

where:

- (h) is estimated altitude in metres;
- (p) is measured pressure;
- (p_0) is the reference sea-level pressure.

The firmware uses 1013.25 hPa as its default reference in `config.h`. This is a standard reference value, not a live local weather reading. For better agreement with a known elevation, the reference pressure must be calibrated.

### 4.3 NEO-6M — GPS receiver

The GPS receiver obtains satellite navigation messages and provides position and related information after it has a valid fix.

The NEO-6M sends NMEA sentences over a UART serial connection. TinyGPSPlus reads the incoming characters and extracts fields such as:

- latitude;
- longitude;
- GPS altitude;
- satellite count.

UART wiring is crossed:

- GPS TX connects to ESP32 RX2 (GPIO 16 in this example).
- GPS RX connects to ESP32 TX2 (GPIO 17 in this example), when the receiver needs input.

The default firmware baud rate is 9600. GPS may take time to acquire satellites and may not obtain a fix indoors.

### 4.4 Flame sensor — digital input

The flame-sensor module's comparator can produce a digital HIGH/LOW output when its threshold is crossed. This firmware treats the signal as an input, not as a certified fire-detection system.

The active logic is controlled by `FLAME_ACTIVE_LOW` in `config.h`. Confirm the module's output behaviour before enabling alerts. Keep the optical sensor test safe and controlled.

## 5. Communication protocols — why the project uses several

### I²C: shared two-wire sensor bus

I²C uses two signal wires:

- **SDA:** serial data;
- **SCL:** serial clock.

The MPU6050 and BMP280 can share SDA and SCL because they have different I²C addresses. The default example uses GPIO 21 for SDA and GPIO 22 for SCL.

A practical learning step is to run an I²C scanner before the full application. It helps determine whether each sensor is visible on the bus.

### UART: GPS serial communication

UART uses a transmit line and a receive line. The transmitter of one device connects to the receiver of the other. The GPS receiver sends a continuous stream of NMEA text sentences. TinyGPSPlus parses that stream.

### SPI: TFT display communication

SPI commonly uses clock, controller-out data, chip-select, and data/command signals. TFT_eSPI manages the low-level communication and drawing functions. The exact pins and display driver are configured in the library's setup file.

### GPIO: buttons and simple outputs

GPIO pins can read a digital state or drive a logic output. The page button uses `INPUT_PULLUP`, so the normal state is HIGH and pressing the switch connects the input to GND, producing LOW.

The firmware uses a debounce interval. A mechanical switch can change state several times very quickly while its contacts settle. Debouncing prevents those transitions from being interpreted as many presses.

## 6. The two display pages

### Page 1 — attitude and pressure

This is the primary flight-information page. It shows the artificial-horizon-style graphic, pitch, roll, relative yaw, barometric altitude, pressure, and IMU availability.

It is a learning display, not a certified primary flight display. The artificial horizon is currently drawn from simple accelerometer estimates; it is not a validated attitude reference.

### Page 2 — navigation and system status

This page displays latitude and longitude when GPS reports a valid location, GPS altitude, satellite count, barometric altitude, temperature, and flame-input state.

A missing GPS fix is shown as a waiting state rather than pretending that a valid coordinate exists.

## 7. Outputs and alerts

- **TFT:** presents the selected page.
- **Status LED:** gives a simple indication that the required sensors are detected and no flame input is active.
- **WS2812 strip:** shows a simple colour status. It requires a suitable power supply for the strip.
- **Passive buzzer:** generates a short warning tone when the flame input is active.
- **USB serial monitor:** reports sensor initialisation and diagnostic information.

An ESP32 GPIO is a logic signal source, not a general-purpose power supply. Use a transistor or suitable driver if the buzzer or another load draws more current than the pin can safely provide.

## 8. Read the firmware structure

Open `firmware/ESP32_MFD_Flight_Info/ESP32_MFD_Flight_Info.ino`.

| Function or object | Purpose |
|---|---|
| `setup()` | Runs once; configures peripherals and detects sensors |
| `loop()` | Repeats continuously; handles GPS, button, alerts, and refresh timing |
| `updateGps()` | Feeds serial bytes into the GPS parser |
| `updateSensors()` | Reads sensor values and updates derived variables |
| `drawPfdPage()` | Draws Page 1 |
| `drawNavigationPage()` | Draws Page 2 |
| `handlePageButton()` | Debounces the physical button and changes pages |
| `handleBuzzer()` | Activates the alert tone when required |
| `config.h` | Keeps pin numbers and settings in one place |

The code uses `millis()` to schedule repeated work. This is a common embedded-programming technique because it avoids blocking the whole program with long delays.

## 9. Hardware wiring and power

Start with the wiring table in [HARDWARE_WIRING.md](HARDWARE_WIRING.md). It lists example GPIO assignments. Confirm the actual ESP32 board, module pin labels, and display controller before connecting wires.

Power design must be checked separately:

1. Identify the battery chemistry and cell count.
2. Select a charger and protection arrangement that matches the cell.
3. Select regulators that provide the correct voltage and current for the ESP32 board, sensors, display, and LED strip.
4. Join grounds where signal communication requires a common reference.
5. Check every rail with a multimeter before plugging modules in.

A TP4056-type charger is a charging circuit; it does not automatically replace a boost/buck regulator or a power-path design. Confirm the exact board variant and its protection features.

## 10. Build and test in stages

Do not assemble every module at once. A staged bring-up makes faults easier to locate:

1. ESP32 only — confirm USB connection and upload.
2. TFT only — confirm controller, driver profile, rotation, and display output.
3. I²C sensors — detect MPU6050 and BMP280; check Serial Monitor.
4. GPS — confirm UART wiring and test outdoors for a fix.
5. Button — confirm a single page change per press.
6. Flame input — verify active polarity.
7. WS2812 and buzzer — add them after checking the supply and current requirements.
8. Full integration — monitor for resets, overheating, flicker, and unstable readings.

Use [TEST_PLAN.md](TEST_PLAN.md) as a checklist. Keep motors and propellers disconnected during bench work.

## 11. Troubleshooting concepts

| Symptom | First checks |
|---|---|
| TFT stays white or blank | Confirm controller, TFT_eSPI setup selection, CS/DC/RST pins, and display power |
| MPU6050 not detected | Check SDA/SCL, common ground, supply, and address 0x68 |
| BMP280 not detected | Check I²C scan result; try the actual address 0x76 or 0x77 |
| GPS keeps waiting for fix | Test outdoors, verify baud rate, cross TX/RX, and inspect antenna |
| Button does not switch page | Check GPIO 27, switch-to-GND wiring, and pull-up configuration |
| Flame warning is inverted | Change `FLAME_ACTIVE_LOW` after measuring the module output |
| WS2812 stays off or flickers | Check external supply, common ground, data direction, and data-level requirements |
| ESP32 restarts | Check regulator current capability, wiring shorts, brownout, and LED load |

## 12. Learning roadmap

After the basic build is stable, useful engineering improvements are:

1. Calibrate accelerometer offsets while the unit is stationary.
2. Implement and test a complementary filter for attitude estimation.
3. Add a magnetometer and calibration if a magnetic heading is required.
4. Detect stale GPS data and report fix quality.
5. Add sensor timeout handling and watchdog diagnostics.
6. Improve display clipping, update only changed regions, and measure refresh rate.
7. Validate electrical noise, vibration, temperature, and power behaviour.

Every improvement should be documented with the hardware tested and repeatable test results.

---

**Author and project owner: Kaustubh Acharya**  
This repository is an educational embedded-systems project and is not flight-certified avionics.
