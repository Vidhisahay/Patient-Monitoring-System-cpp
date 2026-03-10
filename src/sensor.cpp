#include <random>
#include "sensor.h"

int generateHeartRate() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(60, 120);
    return dist(gen);
}

int generateOxygenLevel() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(90, 100);
    return dist(gen);
}

float generateTemperature() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(36.0, 38.5);
    return dist(gen);
}