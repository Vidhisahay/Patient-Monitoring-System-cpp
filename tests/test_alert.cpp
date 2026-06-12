#include <cassert>
#include <ctime>

#include "alert.h"

int main() {
    AlertThresholds thresholds;
    thresholds.minHeartRate = 60;
    thresholds.maxHeartRate = 100;
    thresholds.minOxygen = 92;
    thresholds.maxTemperature = 38.0F;
    thresholds.consecutiveReadings = 2;
    thresholds.cooldownSeconds = 10;

    AlertEngine engine(thresholds);
    const std::time_t start = 100;

    PatientState state{};
    state.heartRate = 80;
    state.oxygen = 98;
    state.temperature = 36.7F;
    assert(engine.evaluate(state, start).empty());

    state.heartRate = 120;
    assert(engine.evaluate(state, start + 1).empty());

    const auto firstAlert = engine.evaluate(state, start + 2);
    assert(firstAlert.size() == 1);
    assert(firstAlert[0] == "ALERT: Abnormal Heart Rate");

    assert(engine.evaluate(state, start + 3).empty());

    const auto secondAlert = engine.evaluate(state, start + 13);
    assert(secondAlert.size() == 1);
    assert(secondAlert[0] == "ALERT: Abnormal Heart Rate");

    return 0;
}
