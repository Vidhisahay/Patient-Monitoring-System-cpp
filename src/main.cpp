#include <iostream>
#include <random>
#include <thread>
#include <chrono>
#include <fstream>
#include <ctime>

using namespace std;

// Heart rate generator
int generateHeartRate() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(60, 120);
    return dist(gen);
}

// Oxygen level generator
int generateOxygenLevel() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(90, 100);
    return dist(gen);
}

// Temperature generator
float generateTemperature() {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(36.0, 38.5);
    return dist(gen);
}

string getCurrentTime() {

    time_t now = time(0);
    tm *ltm = localtime(&now);

    char buffer[9];

    sprintf(buffer, "%02d:%02d:%02d",
        ltm->tm_hour,
        ltm->tm_min,
        ltm->tm_sec);

    return string(buffer);
}

// Logging function
void logEvent(string message) {

    ofstream logFile("data/logs.txt", ios::app);

    if(logFile.is_open()) {
        logFile << getCurrentTime() << " " << message << endl;
    }
}

// Alert detection system
void checkAlerts(int heartRate, int oxygen, float temperature) {

    if(heartRate < 60 || heartRate > 100) {
        cout << "ALERT: Abnormal Heart Rate\n";
        logEvent("ALERT: Abnormal Heart Rate");
    }

    if(oxygen < 92) {
        cout << "ALERT: Low Oxygen Level\n";
        logEvent("ALERT: Low Oxygen Level");
    }

    if(temperature > 38.0) {
        cout << "ALERT: High Temperature\n";
        logEvent("ALERT: High Temperature");
    }
}

int main() {

    cout << "Patient Monitoring System Running\n\n";

    while(true) {

        int heartRate = generateHeartRate();
        int oxygen = generateOxygenLevel();
        float temperature = generateTemperature();

        cout << "Heart Rate: " << heartRate << " bpm\n";
        cout << "Oxygen Level: " << oxygen << " %\n";
        cout << "Temperature: " << temperature << " C\n";

        checkAlerts(heartRate, oxygen, temperature);

        cout << "\n";

        this_thread::sleep_for(chrono::seconds(1));
    }

    return 0;
}