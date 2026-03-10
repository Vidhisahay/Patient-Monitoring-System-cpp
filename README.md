# Patient Monitoring System Simulator

A modular **real-time patient monitoring simulator built in C++** that generates vital signals, evaluates abnormal conditions, and logs system alerts with timestamps. The project models the **core software architecture used in ICU monitoring systems**, where multiple physiological sensors produce data that is continuously evaluated for abnormal patterns and recorded for diagnostics.

## Overview

This project simulates a **real-time hospital patient monitoring system** at the software level. It models how physiological sensors generate continuous vital signals like heart rate, oxygen saturation (SpO₂), and body temperature which are processed by a monitoring engine that detects abnormal conditions and logs critical events.

The system is built to demonstrate **core systems programming concepts in modern C++**, including modular architecture, concurrent sensor simulation, real-time monitoring pipelines, rule-based alert detection, and timestamped event logging.

## Features

- **Concurrent Vital Signal Simulation** - Simulates multiple concurrent physiological sensor streams generating real-time vital signals including heart rate, oxygen saturation (SpO₂), and body temperature.
- **Real-Time Monitoring Engine** - Continuously evaluates incoming sensor data and processes physiological signals at fixed monitoring intervals.
- **Automatic Alert Detection** - Detects abnormal physiological conditions when signals exceed clinically safe thresholds.
- **Timestamped Event Logging** - Records critical alerts with timestamps to support traceability and diagnostic analysis.
- **Modular System Architecture** - Organized into independent components (`sensor`, `alert`, `logger`) to maintain clean separation of responsibilities.


## Tech Stack

| Component | Technology |
|--------|--------|
| Language | C++17 |
| Compiler | g++ (MinGW / GCC) |
| Standard Libraries | `<iostream>`, `<random>`, `<chrono>`, `<thread>`, `<fstream>`, `<ctime>` |
| Build Tool | g++ CLI |
| Version Control | Git + GitHub |


## Architecture

The system follows a **modular architecture where each component has a clear responsibility**.
<br>

![architecture diagram](https://github.com/Vidhisahay/Patient-Monitoring-System-cpp/blob/main/visual/architectureDiagram.png)

The architecture models concurrent sensor threads updating a shared patient state protected by a mutex.  
A monitoring engine evaluates the data in real time, triggers alerts when abnormal conditions occur, and logs critical events for diagnostics.


## How It Works

1. The **sensor module** generates simulated physiological signals.
2. The **main controller** continuously collects sensor data.
3. The **alert module** evaluates signals against medical thresholds.
4. When abnormal values are detected, alerts are triggered.
5. The **logger module** records alerts with timestamps in `logs.txt`.

This pipeline mimics the **data flow in real patient monitoring systems**.


## Systems Concepts Demonstrated

- **Concurrency & Thread Management** - Simulated independent sensor streams using multithreading to model real-time medical sensor behavior.
- **Thread-Safe Shared State** - Used mutex synchronization to protect shared monitoring data under concurrent access.
- **Real-Time Data Processing Pipeline** - Implemented a monitoring loop that continuously processes sensor signals and triggers alert evaluation.
- **Systems-Level C++ Design** - Built using modern C++ (C++17) with structured modules, standard libraries, and reproducible build practices.

## Build & Run

```bash
# Clone repository
git clone https://github.com/Vidhisahay/Patient-Monitoring-System-cpp.git
cd Patient-Monitoring-System-cpp

# Compile
g++ -std=c++17 src/main.cpp src/sensor.cpp src/alert.cpp src/logger.cpp src/patient_state.cpp -Iinclude -o monitor

# Run
monitor

```


## Future Improvements

Potential extensions for the monitoring system include:

- **Producer–Consumer Processing Pipeline** - Introduce a queue-based architecture where sensor threads produce vital data and a monitoring engine consumes and evaluates it asynchronously.
- **Configurable Alert Thresholds** - Externalize physiological thresholds into configuration files or runtime parameters.
- **Real-Time Visualization Dashboard** - Stream sensor data to a UI dashboard for live monitoring of patient vitals.
- **Structured Log Export** - Extend logging to export structured formats such as JSON or CSV for easier analysis.
- **Scalable Sensor Framework** - Generalize the sensor module to dynamically support additional physiological sensors.

# Acknowledgements
Thanks to the open-source C++ community and documentation resources that helped guide the implementation of system-level features such as random number generation, file handling, and time utilities ❤️