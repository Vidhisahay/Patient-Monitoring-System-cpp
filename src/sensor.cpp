#include <random>
#include <thread>
#include <chrono>
#include <iostream>

#include "sensor.h"
#include "patient_state.h"

using namespace std;


// -------- Sensor Value Generators --------

int generateHeartRate() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(60,120);
    return dist(gen);
}

int generateOxygenLevel() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(90,100);
    return dist(gen);
}

float generateTemperature() {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(36.0,38.5);
    return dist(gen);
}


// -------- Sensor Threads --------

void heartRateSensor() {

    while(true) {

        int value = generateHeartRate();

        patientMutex.lock();
        patientData.heartRate = value;
        patientMutex.unlock();

        std::cout << "[Heart Sensor Thread ID: "
                  << std::this_thread::get_id()
                  << "]\n";


        this_thread::sleep_for(chrono::seconds(1));
    }
}

void oxygenSensor() {

    while(true) {

        int value = generateOxygenLevel();

        patientMutex.lock();
        patientData.oxygen = value;
        patientMutex.unlock();

        std::cout << "[Oxygen Sensor Thread ID: "
          << std::this_thread::get_id()
          << "]\n";

        this_thread::sleep_for(chrono::seconds(1));
    }
}

void temperatureSensor() {

    while(true) {

        float value = generateTemperature();

        patientMutex.lock();
        patientData.temperature = value;
        patientMutex.unlock();

        std::cout << "[Temp Sensor Thread ID: "
          << std::this_thread::get_id()
          << "]\n";

        this_thread::sleep_for(chrono::seconds(1));
    }
}