#ifndef SENSOR_H
#define SENSOR_H

#include <chrono>

#include "patient_state.h"

void heartRateSensor(MonitorContext& context, std::chrono::milliseconds interval);
void oxygenSensor(MonitorContext& context, std::chrono::milliseconds interval);
void temperatureSensor(MonitorContext& context, std::chrono::milliseconds interval);

#endif