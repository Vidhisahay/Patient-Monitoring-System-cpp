#ifndef ALERT_H
#define ALERT_H

#include <ctime>
#include <string>
#include <vector>

#include "patient_state.h"

struct AlertThresholds {
    int minHeartRate{60};
    int maxHeartRate{100};
    int minOxygen{92};
    float maxTemperature{38.0F};
    int consecutiveReadings{2};
    int cooldownSeconds{5};
};

AlertThresholds loadAlertThresholds(const std::string& configPath);

class AlertEngine {
public:
    explicit AlertEngine(AlertThresholds thresholds);
    std::vector<std::string> evaluate(const PatientState& patientState, std::time_t nowEpochSeconds);

private:
    AlertThresholds thresholds_;
    int heartRateCounter_{0};
    int oxygenCounter_{0};
    int temperatureCounter_{0};
    std::time_t lastHeartRateAlert_{0};
    std::time_t lastOxygenAlert_{0};
    std::time_t lastTemperatureAlert_{0};
};

#endif