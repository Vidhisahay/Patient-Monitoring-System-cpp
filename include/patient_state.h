#ifndef PATIENT_STATE_H
#define PATIENT_STATE_H

#include <mutex>

struct PatientState {
    int heartRate;
    int oxygen;
    float temperature;
};

extern PatientState patientData;
extern std::mutex patientMutex;

#endif