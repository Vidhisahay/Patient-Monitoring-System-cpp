#include "logger.h"

#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>

namespace {
std::mutex ioMutex;
std::filesystem::path logFilePath = std::filesystem::current_path() / "data" / "logs.txt";

std::filesystem::path resolveLogPath(const std::string& preferredPath) {
    if (!preferredPath.empty()) {
        return std::filesystem::path(preferredPath);
    }
    const char* envPath = std::getenv("MONITOR_LOG_PATH");
    if (envPath != nullptr && std::string(envPath).size() > 0) {
        return std::filesystem::path(envPath);
    }
    return std::filesystem::current_path() / "data" / "logs.txt";
}
} // namespace

bool configureLogFilePath(const std::string& preferredPath) {
    std::lock_guard<std::mutex> lock(ioMutex);
    try {
        logFilePath = resolveLogPath(preferredPath);
        if (logFilePath.has_parent_path()) {
            std::filesystem::create_directories(logFilePath.parent_path());
        }
        return true;
    } catch (const std::exception& exception) {
        std::cerr << "Logger configuration error: " << exception.what() << '\n';
        return false;
    }
}

std::string getCurrentTime() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
    std::tm timeInfo{};

#if defined(_WIN32)
    localtime_s(&timeInfo, &nowTime);
#else
    localtime_r(&nowTime, &timeInfo);
#endif

    std::ostringstream stream;
    stream << std::put_time(&timeInfo, "%H:%M:%S");
    return stream.str();
}

bool logEvent(const std::string& message) {
    std::lock_guard<std::mutex> lock(ioMutex);

    std::ofstream logFile(logFilePath, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file: " << logFilePath << '\n';
        return false;
    }

    logFile << getCurrentTime() << " " << message << '\n';
    if (!logFile.good()) {
        std::cerr << "Failed to write to log file: " << logFilePath << '\n';
        return false;
    }
    return true;
}

void printLine(const std::string& message) {
    std::lock_guard<std::mutex> lock(ioMutex);
    std::cout << message << '\n';
}