#include <cassert>
#include <chrono>
#include <thread>

#include "sensor.h"

int main() {
    MonitorContext context{};
    const auto interval = std::chrono::milliseconds(10);

    std::thread heartThread(heartRateSensor, std::ref(context), interval);
    std::thread oxygenThread(oxygenSensor, std::ref(context), interval);
    std::thread tempThread(temperatureSensor, std::ref(context), interval);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    context.running.store(false, std::memory_order_relaxed);

    heartThread.join();
    oxygenThread.join();
    tempThread.join();

    PatientState snapshot{};
    {
        std::lock_guard<std::mutex> lock(context.patientMutex);
        snapshot = context.patientData;
    }

    assert(snapshot.heartRate >= 60 && snapshot.heartRate <= 120);
    assert(snapshot.oxygen >= 90 && snapshot.oxygen <= 100);
    assert(snapshot.temperature >= 36.0F && snapshot.temperature <= 38.5F);
    return 0;
}
