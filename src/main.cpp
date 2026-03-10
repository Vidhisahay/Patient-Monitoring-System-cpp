#include <iostream>
#include <thread>
#include <chrono>

#include "sensor.h"
#include "alert.h"
#include "patient_state.h"

using namespace std;

int main() {

    cout << "Patient Monitoring System Running\n\n";

    thread t1(heartRateSensor);
    thread t2(oxygenSensor);
    thread t3(temperatureSensor);

    while(true) {

        patientMutex.lock();

        int hr = patientData.heartRate;
        int ox = patientData.oxygen;
        float temp = patientData.temperature;

        patientMutex.unlock();

        cout << "Heart Rate: " << hr << " bpm\n";
        cout << "Oxygen Level: " << ox << " %\n";
        cout << "Temperature: " << temp << " C\n";

        checkAlerts(hr, ox, temp);

        cout << "\n";

        this_thread::sleep_for(chrono::seconds(1));
    }

    t1.join();
    t2.join();
    t3.join();

    return 0;
}