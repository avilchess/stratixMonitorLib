//
// Created by Antonio Vilches on 07/05/2020.
//

#ifndef STRATIXMONITORLIB_STRATIX_MONITOR_H
#define STRATIXMONITORLIB_STRATIX_MONITOR_H

#include "FPGACounters.h"
#include <thread>
#include <mutex>

class StratixMonitor {
private:
    // Fields
    static StratixMonitor *instance;
    int32_t time_period;                                         // in milliseconds
    std::chrono::time_point<std::chrono::high_resolution_clock> ts;
    FPGACounters current_state;
    float idle_power;
    uint16_t sensorId;
    std::mutex my_mutex;
    std::thread monitor_thread;

    //Methods
    StratixMonitor(int32_t period);                             // forbidden to call directly because it is a singleton

    void initialize_sublibraries();

    void read_fpga_counters();

public:
    static StratixMonitor *getInstance(int32_t period = 100);   // the only way to get access

    FPGACounters get_fpga_counters();
};

#endif //STRATIXMONITORLIB_STRATIX_MONITOR_H
