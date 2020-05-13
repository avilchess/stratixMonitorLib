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
    int32_t time_period;                                                                        // in milliseconds
    FPGAPowerCounters power_state;
    FPGAEnergyCounters energy_state;
    std::mutex my_mutex;
    std::thread monitor_thread;

    //Methods
    StratixMonitor(int32_t period);                             // forbidden to call directly because it is a singleton
    void initialize_sublibraries();

    [[noreturn]] void read_fpga_counters();

    float get_power_from_voltage_and_current_sensors(int32_t sensor_voltage, int32_t sensor_current);

    void update_power_and_energy_with_last_measure(FPGAPowerCounters instant_power);

    FPGAPowerCounters get_current_power_counters();

public:
    static StratixMonitor *getInstance(int32_t period = 10);   // the only way to get access

    FPGAEnergyCounters get_energy_counters();

    float get_counter_state_from_sensor(int32_t sensor_id);
};

#endif //STRATIXMONITORLIB_STRATIX_MONITOR_H
