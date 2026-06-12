#ifndef LOGGER_H
#define LOGGER_H

#include <string>

bool configureLogFilePath(const std::string& preferredPath = "");
bool logEvent(const std::string& message);
void printLine(const std::string& message);
std::string getCurrentTime();

#endif