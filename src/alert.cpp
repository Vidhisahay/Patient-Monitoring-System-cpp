#include "alert.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

namespace {
std::string trim(const std::string& value) {
    const auto first = std::find_if_not(value.begin(), value.end(), [](unsigned char c) {
        return std::isspace(c) != 0;
    });
    const auto last = std::find_if_not(value.rbegin(), value.rend(), [](unsigned char c) {
        return std::isspace(c) != 0;
    }).base();
    if (first >= last) {
        return {};
    }
    return std::string(first, last);
}

bool shouldEmit(int counter, int requiredReadings, std::time_t now, std::time_t last, int cooldown) {
    return counter >= requiredReadings && (last == 0 || now - last >= cooldown);
}
} // namespace

AlertThresholds loadAlertThresholds(const std::string& configPath) {
    AlertThresholds thresholds;
    std::ifstream configFile(configPath);

    if (!configFile.is_open()) {
        std::cerr << "Warning: could not open threshold config at '" << configPath
                  << "'. Using defaults.\n";
        return thresholds;
    }

    std::unordered_map<std::string, std::string> values;
    std::string line;
    while (std::getline(configFile, line)) {
        const std::string cleaned = trim(line);
        if (cleaned.empty() || cleaned[0] == '#') {
            continue;
        }

        const std::size_t separator = cleaned.find('=');
        if (separator == std::string::npos) {
            continue;
        }

        const std::string key = trim(cleaned.substr(0, separator));
        const std::string value = trim(cleaned.substr(separator + 1));
        if (!key.empty() && !value.empty()) {
            values[key] = value;
        }
    }

    try {
        if (values.count("min_heart_rate")) {
            thresholds.minHeartRate = std::stoi(values["min_heart_rate"]);
        }
        if (values.count("max_heart_rate")) {
            thresholds.maxHeartRate = std::stoi(values["max_heart_rate"]);
        }
        if (values.count("min_oxygen")) {
            thresholds.minOxygen = std::stoi(values["min_oxygen"]);
        }
        if (values.count("max_temperature")) {
            thresholds.maxTemperature = std::stof(values["max_temperature"]);
        }
        if (values.count("consecutive_readings")) {
            thresholds.consecutiveReadings = std::max(1, std::stoi(values["consecutive_readings"]));
        }
        if (values.count("cooldown_seconds")) {
            thresholds.cooldownSeconds = std::max(0, std::stoi(values["cooldown_seconds"]));
        }
    } catch (const std::exception&) {
        std::cerr << "Warning: invalid values in '" << configPath << "'. Using defaults.\n";
        return AlertThresholds{};
    }

    return thresholds;
}

AlertEngine::AlertEngine(AlertThresholds thresholds) : thresholds_(thresholds) {}

std::vector<std::string> AlertEngine::evaluate(const PatientState& patientState, std::time_t nowEpochSeconds) {
    std::vector<std::string> alerts;

    const bool heartRateAbnormal =
        patientState.heartRate < thresholds_.minHeartRate || patientState.heartRate > thresholds_.maxHeartRate;
    const bool oxygenAbnormal = patientState.oxygen < thresholds_.minOxygen;
    const bool temperatureAbnormal = patientState.temperature > thresholds_.maxTemperature;

    heartRateCounter_ = heartRateAbnormal ? heartRateCounter_ + 1 : 0;
    oxygenCounter_ = oxygenAbnormal ? oxygenCounter_ + 1 : 0;
    temperatureCounter_ = temperatureAbnormal ? temperatureCounter_ + 1 : 0;

    if (shouldEmit(heartRateCounter_, thresholds_.consecutiveReadings, nowEpochSeconds, lastHeartRateAlert_,
                   thresholds_.cooldownSeconds)) {
        alerts.emplace_back("ALERT: Abnormal Heart Rate");
        lastHeartRateAlert_ = nowEpochSeconds;
    }

    if (shouldEmit(oxygenCounter_, thresholds_.consecutiveReadings, nowEpochSeconds, lastOxygenAlert_,
                   thresholds_.cooldownSeconds)) {
        alerts.emplace_back("ALERT: Low Oxygen Level");
        lastOxygenAlert_ = nowEpochSeconds;
    }

    if (shouldEmit(temperatureCounter_, thresholds_.consecutiveReadings, nowEpochSeconds, lastTemperatureAlert_,
                   thresholds_.cooldownSeconds)) {
        alerts.emplace_back("ALERT: High Temperature");
        lastTemperatureAlert_ = nowEpochSeconds;
    }

    return alerts;
}