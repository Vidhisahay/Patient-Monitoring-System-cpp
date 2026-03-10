#include <fstream>
#include <ctime>
#include "logger.h"

std::string getCurrentTime() {

    time_t now = time(0);
    tm *ltm = localtime(&now);

    char buffer[9];

    sprintf(buffer, "%02d:%02d:%02d",
        ltm->tm_hour,
        ltm->tm_min,
        ltm->tm_sec);

    return std::string(buffer);
}

void logEvent(std::string message) {

    std::ofstream logFile("data/logs.txt", std::ios::app);

    if(logFile.is_open()) {
        logFile << getCurrentTime() << " " << message << std::endl;
    }
}