#ifndef PATIENT_STATE_H
#define PATIENT_STATE_H

#include <atomic>
#include <mutex>

struct PatientState {
    int heartRate{75};
    int oxygen{98};
    float temperature{36.8F};
};

struct MonitorContext {
    PatientState patientData{};
    std::mutex patientMutex{};
    std::atomic<bool> running{true};
};

#endif