# Known limitations

1. **Yaw drift:** MPU6050 has no magnetometer. Gyroscope integration accumulates error and is not absolute heading.
2. **Basic attitude estimate:** Pitch and roll are accelerometer-derived. Aircraft acceleration and vibration distort the values.
3. **No validated sensor fusion:** No Kalman, complementary, or Madgwick filter is implemented.
4. **Approximate barometric altitude:** Depends on reference pressure and weather.
5. **GPS display only:** The second page shows position, altitude, and satellite count; it does not provide route guidance.
6. **Flame input:** Comparator polarity and threshold are hardware-specific.
7. **No flight qualification:** Vibration, EMI, temperature, power transients, and display readability have not been certified.

## Recommended next steps

- Record actual final pinout and module part numbers.
- Add sensor calibration and a validated attitude-estimation filter.
- Add a magnetometer if absolute heading is required.
- Add GPS fix quality and stale-data warnings.
- Add watchdog and brownout diagnostics.
- Validate power, EMI, and vibration on the bench before any airborne demonstration.

This prototype is not a flight-certified instrument or safety-critical system.
