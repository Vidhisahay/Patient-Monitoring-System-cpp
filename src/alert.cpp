#include <iostream>
#include "alert.h"
#include "logger.h"

using namespace std;

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