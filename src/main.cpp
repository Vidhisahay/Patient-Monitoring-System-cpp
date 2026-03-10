#include <iostream>
#include <thread>
#include <chrono>

#include "sensor.h"
#include "alert.h"

using namespace std;

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