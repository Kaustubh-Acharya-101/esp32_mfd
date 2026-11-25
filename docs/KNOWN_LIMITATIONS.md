# Known Limitations

**Project owner: Kaustubh Acharya**

This repository is a learning prototype. The following points are important when interpreting its output.

1. **Yaw drift:** the MPU6050 has no magnetometer. Gyroscope integration accumulates bias and error, so yaw is a relative estimate, not a compass heading.
2. **Basic attitude:** pitch and roll currently use accelerometer-only equations. Acceleration and vibration can distort them.
3. **No validated sensor fusion:** the firmware does not currently implement a validated complementary, Kalman, or Madgwick filter.
4. **No full calibration routine:** accelerometer offsets and gyro bias have not been calibrated as part of an automated startup routine.
5. **Approximate barometric altitude:** altitude depends on a reference pressure. Weather and pressure changes affect the result.
6. **GPS availability:** a fix may take time or be unavailable indoors. The implementation should be enhanced with stale-fix and quality checks.
7. **Flame input:** comparator polarity and threshold are module-specific; this is not a certified fire-protection system.
8. **Power integrity:** regulator sizing, battery protection, transient response, and load-sharing must be validated for the actual assembly.
9. **No flight qualification:** vibration, EMI, temperature, power transients, and display readability have not been certified.

## Recommended development roadmap

- Record the final part numbers and measured wiring.
- Add and document IMU calibration and a tested sensor-fusion algorithm.
- Add a magnetometer if absolute heading is required.
- Add GPS fix age, quality, and stale-data warnings.
- Add sensor timeouts, watchdog handling, and brownout diagnostics.
- Test power integrity, EMI, and vibration on the bench.
- Keep a written test log for each firmware revision.

This project is not a flight-certified instrument, flight controller, or safety-critical system.
