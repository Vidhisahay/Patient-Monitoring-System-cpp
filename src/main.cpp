#include <chrono>
#include <csignal>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>

#include "alert.h"
#include "logger.h"
#include "patient_state.h"
#include "sensor.h"

namespace {
std::atomic<bool>* globalRunningFlag = nullptr;

void handleSignal(int) {
    if (globalRunningFlag != nullptr) {
        globalRunningFlag->store(false, std::memory_order_relaxed);
    }
}
} // namespace

int main(int argc, char* argv[]) {
    int maxIterations = -1;
    if (argc == 2) {
        maxIterations = std::stoi(argv[1]);
    }

    constexpr auto sampleInterval = std::chrono::seconds(1);
    MonitorContext context{};
    globalRunningFlag = &context.running;
    std::signal(SIGINT, handleSignal);
    std::signal(SIGTERM, handleSignal);

    const auto dataDirectory = std::filesystem::current_path() / "data";
    const auto thresholdsPath = dataDirectory / "thresholds.cfg";
    const auto logsPath = dataDirectory / "logs.txt";

    if (!configureLogFilePath(logsPath.string())) {
        printLine("Warning: logging path setup failed. Runtime errors will be printed to stderr.");
    }

    const AlertThresholds thresholds = loadAlertThresholds(thresholdsPath.string());
    AlertEngine alertEngine(thresholds);

    printLine("Patient Monitoring System Running (Ctrl+C to stop)");

    std::vector<std::thread> sensorThreads;
    sensorThreads.emplace_back(heartRateSensor, std::ref(context), sampleInterval);
    sensorThreads.emplace_back(oxygenSensor, std::ref(context), sampleInterval);
    sensorThreads.emplace_back(temperatureSensor, std::ref(context), sampleInterval);

    int iteration = 0;
    try {
        while (context.running.load(std::memory_order_relaxed)) {
            PatientState snapshot;
            {
                std::lock_guard<std::mutex> lock(context.patientMutex);
                snapshot = context.patientData;
            }

            printLine("Heart Rate: " + std::to_string(snapshot.heartRate) + " bpm");
            printLine("Oxygen Level: " + std::to_string(snapshot.oxygen) + " %");
            printLine("Temperature: " + std::to_string(snapshot.temperature) + " C");

            const auto alerts = alertEngine.evaluate(snapshot, std::time(nullptr));
            for (const std::string& alert : alerts) {
                printLine(alert);
                logEvent(alert);
            }
            printLine("");

            std::this_thread::sleep_for(sampleInterval);
            ++iteration;
            if (maxIterations > 0 && iteration >= maxIterations) {
                context.running.store(false, std::memory_order_relaxed);
            }
        }
    } catch (const std::exception& exception) {
        context.running.store(false, std::memory_order_relaxed);
        printLine(std::string("Fatal runtime error: ") + exception.what());
    }

    context.running.store(false, std::memory_order_relaxed);
    for (std::thread& thread : sensorThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    printLine("Patient Monitoring System Stopped");
    return 0;
}