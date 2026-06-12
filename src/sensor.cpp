#include <chrono>
#include <mutex>
#include <random>
#include <thread>

#include "sensor.h"

namespace {
std::mt19937 makeGenerator() {
    thread_local std::random_device randomDevice;
    return std::mt19937(randomDevice());
}

int generateHeartRate() {
    thread_local std::mt19937 generator = makeGenerator();
    static thread_local std::uniform_int_distribution<int> distribution(60, 120);
    return distribution(generator);
}

int generateOxygenLevel() {
    thread_local std::mt19937 generator = makeGenerator();
    static thread_local std::uniform_int_distribution<int> distribution(90, 100);
    return distribution(generator);
}

float generateTemperature() {
    thread_local std::mt19937 generator = makeGenerator();
    static thread_local std::uniform_real_distribution<float> distribution(36.0F, 38.5F);
    return distribution(generator);
}
} // namespace

void heartRateSensor(MonitorContext& context, std::chrono::milliseconds interval) {
    while (context.running.load(std::memory_order_relaxed)) {
        const int value = generateHeartRate();
        {
            std::lock_guard<std::mutex> lock(context.patientMutex);
            context.patientData.heartRate = value;
        }
        std::this_thread::sleep_for(interval);
    }
}

void oxygenSensor(MonitorContext& context, std::chrono::milliseconds interval) {
    while (context.running.load(std::memory_order_relaxed)) {
        const int value = generateOxygenLevel();
        {
            std::lock_guard<std::mutex> lock(context.patientMutex);
            context.patientData.oxygen = value;
        }
        std::this_thread::sleep_for(interval);
    }
}

void temperatureSensor(MonitorContext& context, std::chrono::milliseconds interval) {
    while (context.running.load(std::memory_order_relaxed)) {
        const float value = generateTemperature();
        {
            std::lock_guard<std::mutex> lock(context.patientMutex);
            context.patientData.temperature = value;
        }
        std::this_thread::sleep_for(interval);
    }
}