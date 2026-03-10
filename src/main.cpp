#include <iostream>
#include <random>

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

int main() {

    cout << "Patient Monitoring System Running\n\n";

    int heartRate = generateHeartRate();
    int oxygen = generateOxygenLevel();
    float temperature = generateTemperature();

    cout << "Heart Rate: " << heartRate << " bpm\n";
    cout << "Oxygen Level: " << oxygen << " %\n";
    cout << "Temperature: " << temperature << " C\n";

    return 0;
}