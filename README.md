# Patient Monitoring System Simulator (C++)

A modular real-time patient monitoring simulator in C++17 with concurrent sensor workers, stateful alerting, synchronized logging, and a CMake/CTest quality pipeline.

## Overview

The system simulates heart rate, oxygen saturation (SpO₂), and temperature streams using dedicated sensor threads.  
The main monitoring loop consumes synchronized snapshots, evaluates sustained abnormal conditions, and logs events with timestamps.

## Runtime Architecture

- **MonitorContext ownership model**: shared patient state, mutex, and run-state flag are owned explicitly by the runtime context.
- **Graceful shutdown**: `SIGINT`/`SIGTERM` stop the loop, signal worker threads to exit, then join all threads.
- **Thread-safe state access**: all shared state reads/writes use RAII locks.
- **Thread-safe output/logging**: synchronized console and log writing avoid interleaved output.

## Alerting Model

- **Configurable thresholds** loaded from `data/thresholds.cfg`
- **Transient filtering** via configurable consecutive abnormal readings
- **Cooldown suppression** to avoid alert spam during sustained abnormalities

Default configuration:

```ini
min_heart_rate=60
max_heart_rate=100
min_oxygen=92
max_temperature=38.0
consecutive_readings=2
cooldown_seconds=5
```

## Build & Run

```bash
cmake -S /home/runner/work/Patient-Monitoring-System-cpp/Patient-Monitoring-System-cpp/Vidhisahay/Patient-Monitoring-System-cpp -B /home/runner/work/Patient-Monitoring-System-cpp/Patient-Monitoring-System-cpp/Vidhisahay/Patient-Monitoring-System-cpp/build -DCMAKE_BUILD_TYPE=Release
cmake --build /home/runner/work/Patient-Monitoring-System-cpp/Patient-Monitoring-System-cpp/Vidhisahay/Patient-Monitoring-System-cpp/build --config Release
/home/runner/work/Patient-Monitoring-System-cpp/Patient-Monitoring-System-cpp/Vidhisahay/Patient-Monitoring-System-cpp/build/monitor
```

Optional bounded run for automation:

```bash
/home/runner/work/Patient-Monitoring-System-cpp/Patient-Monitoring-System-cpp/Vidhisahay/Patient-Monitoring-System-cpp/build/monitor 5
```

## Testing

```bash
ctest --test-dir /home/runner/work/Patient-Monitoring-System-cpp/Patient-Monitoring-System-cpp/Vidhisahay/Patient-Monitoring-System-cpp/build --output-on-failure
```

Included tests:

- `alert_engine_behavior`: sustained-alert and cooldown behavior
- `threaded_sensor_updates`: concurrent sensor update flow and value-range checks

## CI Quality Gates

GitHub Actions workflow (`.github/workflows/ci.yml`) runs:

1. Build + tests
2. Sanitizer build (`-DENABLE_SANITIZERS=ON`)
3. Static analysis build with clang-tidy (`-DENABLE_CLANG_TIDY=ON`)

## Future Scalability Path

- Queue-based producer/consumer monitoring pipeline
- Structured JSON/CSV event export
- Sensor plugin framework for additional vital signs
- Dashboard integration for real-time visualization
