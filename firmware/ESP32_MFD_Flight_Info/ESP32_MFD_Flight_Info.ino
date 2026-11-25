/*
 * ESP 32 BASED MFD for Flight Info
 * Maintainer: Kaustubh Acharya
 * Team credits: Souvik Mahata | Kaustubh Acharya | SRM-IST
 *
 * ESP32 + 2.8-inch SPI TFT (TFT_eSPI) + MPU6050 + BMP280 + NEO-6M.
 * Two-page supplementary avionics display prototype.
 *
 * This is not a certified instrument or flight-control system.
 * Check config.h and the wiring guide before powering the hardware.
 */
#include <Arduino.h>
#include <Wire.h>
#include <math.h>
#include <TFT_eSPI.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>
#include <TinyGPSPlus.h>
#include <Adafruit_NeoPixel.h>
#include "config.h"

TFT_eSPI tft = TFT_eSPI();
Adafruit_MPU6050 mpu;
Adafruit_BMP280 bmp;
TinyGPSPlus gps;
Adafruit_NeoPixel pixels(NEOPIXEL_COUNT, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

bool imuReady = false, baroReady = false, flameDetected = false, pageTwo = false;
float pitchDeg = 0.0f, rollDeg = 0.0f, yawRelativeDeg = 0.0f;
float pressureHpa = NAN, barometricAltitudeM = NAN, temperatureC = NAN;
uint32_t lastSensorUpdate = 0, lastDisplayUpdate = 0, lastImuUpdate = 0;
uint32_t lastBuzzerUpdate = 0, lastButtonChange = 0;
bool lastButtonReading = HIGH, stableButtonState = HIGH;

uint16_t statusColour() {
  if (flameDetected) return TFT_RED;
  if (!imuReady || !baroReady) return TFT_ORANGE;
  return TFT_GREEN;
}

void setStatusLed(bool on) {
  bool level = STATUS_LED_ACTIVE_HIGH ? on : !on;
  digitalWrite(PIN_STATUS_LED, level ? HIGH : LOW);
}

void updatePixelStatus() {
  if (!ENABLE_NEOPIXEL) return;
  uint32_t colour = flameDetected ? pixels.Color(255, 0, 0) :
                    ((!imuReady || !baroReady) ? pixels.Color(255, 90, 0) :
                                                   pixels.Color(0, 100, 20));
  for (uint16_t i = 0; i < NEOPIXEL_COUNT; ++i) pixels.setPixelColor(i, colour);
  pixels.show();
}

void drawHeader(const char *pageName) {
  tft.fillRect(0, 0, 320, 31, TFT_NAVY);
  tft.setTextColor(TFT_WHITE, TFT_NAVY);
  tft.setTextDatum(TL_DATUM);
  tft.drawString("ESP32 FLIGHT MFD", 8, 7, 2);
  tft.setTextDatum(TR_DATUM);
  tft.drawString(pageName, 312, 7, 2);
  tft.drawFastHLine(0, 31, 320, TFT_DARKGREY);
}

void drawFooter() {
  tft.fillRect(0, 224, 320, 16, TFT_BLACK);
  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
  tft.drawString("KAUSTUBH ACHARYA  |  SRM-IST", 7, 226, 1);
  tft.setTextDatum(TR_DATUM);
  tft.setTextColor(statusColour(), TFT_BLACK);
  tft.drawString(flameDetected ? "FIRE ALERT" : "SYSTEM STATUS", 313, 226, 1);
}

void drawLabelValue(const char *label, const String &value, int x, int y,
                    uint16_t valueColour = TFT_WHITE) {
  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawString(label, x, y, 2);
  tft.setTextColor(valueColour, TFT_BLACK);
  tft.drawString(value, x, y + 19, 2);
}

String fixedValue(float value, uint8_t decimals = 1) {
  if (!isfinite(value)) return String("--");
  return String(value, decimals);
}

void updateGps() {
  while (Serial2.available() > 0) gps.encode(Serial2.read());
}

void updateSensors() {
  updateGps();
  if (ENABLE_FLAME_INPUT) {
    int raw = digitalRead(PIN_FLAME_SENSOR);
    flameDetected = FLAME_ACTIVE_LOW ? (raw == LOW) : (raw == HIGH);
  }

  uint32_t now = millis();
  if (imuReady && (now - lastImuUpdate >= SENSOR_REFRESH_MS)) {
    float dt = (lastImuUpdate == 0) ? 0.0f : (now - lastImuUpdate) / 1000.0f;
    lastImuUpdate = now;
    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);

    // Accelerometer-only attitude estimate; dynamic acceleration affects it.
    float ax = accel.acceleration.x, ay = accel.acceleration.y, az = accel.acceleration.z;
    rollDeg = atan2f(ay, az) * 180.0f / PI;
    pitchDeg = atan2f(-ax, sqrtf(ay * ay + az * az)) * 180.0f / PI;

    // MPU6050 has no magnetometer: this relative yaw estimate will drift.
    if (dt > 0.0f && dt < 0.25f) {
      yawRelativeDeg += gyro.gyro.z * dt * 180.0f / PI;
      if (yawRelativeDeg > 180.0f) yawRelativeDeg -= 360.0f;
      if (yawRelativeDeg < -180.0f) yawRelativeDeg += 360.0f;
    }
    temperatureC = temp.temperature;
  }

  if (baroReady) {
    pressureHpa = bmp.readPressure() / 100.0f;
    barometricAltitudeM = bmp.readAltitude(SEA_LEVEL_HPA);
    temperatureC = bmp.readTemperature();
  }
  setStatusLed(!flameDetected && imuReady && baroReady);
  updatePixelStatus();
}

// Page 1: attitude display
void drawArtificialHorizon() {
  const int cx = 160, cy = 130, halfWidth = 86;
  float angle = rollDeg * PI / 180.0f;
  float dx = halfWidth * cosf(angle), dy = halfWidth * sinf(angle);
  float horizonY = cy + pitchDeg * 2.1f;
  tft.drawRoundRect(61, 54, 198, 140, 9, TFT_DARKGREY);
  tft.drawLine((int)(cx - dx), (int)(horizonY - dy),
               (int)(cx + dx), (int)(horizonY + dy), TFT_YELLOW);
  for (int mark = -20; mark <= 20; mark += 10) {
    if (mark == 0) continue;
    float markY = horizonY + mark * 2.1f;
    int tickHalf = (mark % 20 == 0) ? 19 : 12;
    float tx = tickHalf * cosf(angle), ty = tickHalf * sinf(angle);
    tft.drawLine((int)(cx - tx), (int)(markY - ty),
                 (int)(cx + tx), (int)(markY + ty), TFT_WHITE);
  }
  tft.drawLine(cx - 28, cy, cx - 8, cy, TFT_CYAN);
  tft.drawLine(cx + 8, cy, cx + 28, cy, TFT_CYAN);
  tft.drawLine(cx - 8, cy, cx - 4, cy + 7, TFT_CYAN);
  tft.drawLine(cx + 8, cy, cx + 4, cy + 7, TFT_CYAN);
  tft.drawCircle(cx, cy, 3, TFT_CYAN);
}

void drawPfdPage() {
  tft.fillScreen(TFT_BLACK);
  drawHeader("PAGE 1 / ATTITUDE");
  drawArtificialHorizon();
  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_CYAN, TFT_BLACK); tft.drawString("PITCH", 8, 55, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); tft.drawString(fixedValue(pitchDeg) + " deg", 8, 75, 2);
  tft.setTextColor(TFT_CYAN, TFT_BLACK); tft.drawString("ROLL", 8, 111, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); tft.drawString(fixedValue(rollDeg) + " deg", 8, 131, 2);
  tft.setTextColor(TFT_CYAN, TFT_BLACK); tft.drawString("YAW REL.", 8, 166, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); tft.drawString(fixedValue(yawRelativeDeg) + " deg", 8, 186, 2);
  drawLabelValue("ALT", fixedValue(barometricAltitudeM, 0) + " m", 268, 66, baroReady ? TFT_WHITE : TFT_ORANGE);
  drawLabelValue("PRES", fixedValue(pressureHpa, 1) + " hPa", 268, 119, baroReady ? TFT_WHITE : TFT_ORANGE);
  drawLabelValue("IMU", imuReady ? "ONLINE" : "OFFLINE", 268, 172, imuReady ? TFT_GREEN : TFT_ORANGE);
  drawFooter();
}

// Page 2: navigation and sensor status
void drawNavigationPage() {
  tft.fillScreen(TFT_BLACK);
  drawHeader("PAGE 2 / NAV + STATUS");
  if (gps.location.isValid()) {
    drawLabelValue("LATITUDE", String(gps.location.lat(), 6), 10, 47);
    drawLabelValue("LONGITUDE", String(gps.location.lng(), 6), 10, 98);
  } else {
    drawLabelValue("LATITUDE", "WAITING FOR FIX", 10, 47, TFT_ORANGE);
    drawLabelValue("LONGITUDE", "WAITING FOR FIX", 10, 98, TFT_ORANGE);
  }
  drawLabelValue("GPS ALT", gps.altitude.isValid() ? fixedValue(gps.altitude.meters(), 1) + " m" : "--",
                 10, 149, gps.altitude.isValid() ? TFT_WHITE : TFT_ORANGE);
  drawLabelValue("SATELLITES", gps.satellites.isValid() ? String(gps.satellites.value()) : "--",
                 170, 47, gps.satellites.isValid() ? TFT_WHITE : TFT_ORANGE);
  drawLabelValue("BARO ALT", fixedValue(barometricAltitudeM, 1) + " m", 170, 98,
                 baroReady ? TFT_WHITE : TFT_ORANGE);
  drawLabelValue("TEMP", fixedValue(temperatureC, 1) + " C", 170, 149,
                 isfinite(temperatureC) ? TFT_WHITE : TFT_ORANGE);
  tft.drawRoundRect(8, 198, 304, 21, 4, flameDetected ? TFT_RED : TFT_DARKGREY);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(flameDetected ? TFT_RED : TFT_GREEN, TFT_BLACK);
  tft.drawString(flameDetected ? "FLAME INPUT ACTIVE - CHECK AREA" : "FLAME INPUT: NORMAL", 160, 209, 2);
  drawFooter();
}

void handlePageButton() {
  bool reading = digitalRead(PIN_PAGE_BUTTON);
  if (reading != lastButtonReading) {
    lastButtonChange = millis();
    lastButtonReading = reading;
  }
  if ((millis() - lastButtonChange) > BUTTON_DEBOUNCE_MS && reading != stableButtonState) {
    stableButtonState = reading;
    if (stableButtonState == LOW) {
      pageTwo = !pageTwo;
      lastDisplayUpdate = 0;
    }
  }
}

void handleBuzzer() {
  if (!ENABLE_BUZZER || !flameDetected) return;
  if (millis() - lastBuzzerUpdate >= 1200) {
    tone(PIN_BUZZER, 2200, 180);
    lastBuzzerUpdate = millis();
  }
}

void setup() {
  pinMode(PIN_STATUS_LED, OUTPUT);
  setStatusLed(false);
  pinMode(PIN_PAGE_BUTTON, INPUT_PULLUP);
  if (ENABLE_FLAME_INPUT) pinMode(PIN_FLAME_SENSOR, INPUT);
  if (ENABLE_BUZZER) pinMode(PIN_BUZZER, OUTPUT);

  if (ENABLE_NEOPIXEL) {
    pixels.begin();
    pixels.setBrightness(NEOPIXEL_BRIGHTNESS);
    pixels.clear();
    pixels.show();
  }

  Serial.begin(115200);
  Serial2.begin(GPS_BAUD, SERIAL_8N1, PIN_GPS_RX, PIN_GPS_TX);
  tft.init();
  tft.setRotation(TFT_ROTATION);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("ESP 32 BASED MFD", 160, 72, 4);
  tft.drawString("FOR FLIGHT INFO", 160, 102, 2);
  tft.drawString("KAUSTUBH ACHARYA", 160, 132, 2);
  tft.drawString("SOUVIK MAHATA  |  SRM-IST", 160, 155, 2);
  tft.drawString("INITIALISING SENSORS...", 160, 190, 2);

  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
  imuReady = mpu.begin(0x68, &Wire);
  if (imuReady) {
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  }

  // Adafruit_BMP280 begin(address, chipId, wire); BMP280 ID = 0x58.
  baroReady = bmp.begin(0x76, 0x58, &Wire);
  if (!baroReady) baroReady = bmp.begin(0x77, 0x58, &Wire);
  if (baroReady) {
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                    Adafruit_BMP280::SAMPLING_X2,
                    Adafruit_BMP280::SAMPLING_X16,
                    Adafruit_BMP280::FILTER_X16,
                    Adafruit_BMP280::STANDBY_MS_125);
  }

  Serial.println();
  Serial.println(F("ESP 32 BASED MFD for Flight Info startup"));
  Serial.print(F("MPU6050: ")); Serial.println(imuReady ? F("OK") : F("NOT FOUND"));
  Serial.print(F("BMP280:  ")); Serial.println(baroReady ? F("OK") : F("NOT FOUND"));
  Serial.println(F("GPS UART2 started at 9600 baud"));
  Serial.println(F("Yaw is relative gyro integration, not compass heading."));

  delay(900);
  lastButtonReading = digitalRead(PIN_PAGE_BUTTON);
  stableButtonState = lastButtonReading;
  lastSensorUpdate = millis();
  lastDisplayUpdate = 0;
}

void loop() {
  updateGps();
  handlePageButton();
  handleBuzzer();
  if (millis() - lastSensorUpdate >= SENSOR_REFRESH_MS) {
    lastSensorUpdate = millis();
    updateSensors();
  }
  if (millis() - lastDisplayUpdate >= DISPLAY_REFRESH_MS) {
    lastDisplayUpdate = millis();
    if (pageTwo) drawNavigationPage();
    else drawPfdPage();
  }
}
