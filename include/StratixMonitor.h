//
// Created by Antonio Vilches on 07/05/2020.
//

#ifndef STRATIXMONITORLIB_STRATIX_MONITOR_H
#define STRATIXMONITORLIB_STRATIX_MONITOR_H

#include "FPGACounters.h"
#include "SensorIds.h"
#include <thread>
#include <mutex>
#include <atomic>
#include <map>
#include <vector>
#include <chrono>

using Measure = std::pair<std::chrono::time_point<std::chrono::high_resolution_clock>, float>;

class StratixMonitor {
private:
    // Fields
    static StratixMonitor *instance;
    int32_t time_period;                                                           // in milliseconds
    FPGAPowerCounterState power_state;
    FPGAEnergyCounterState energy_state;
    static std::mutex my_mutex;
    std::thread monitor_thread;
    std::atomic<int32_t> control_thread;
    std::vector<std::atomic<int32_t>> sensors_registration;
    std::map<SensorID, std::vector<Measure>> historical_data;

    //Methods
    StratixMonitor(int32_t period);                             // forbidden to call directly because it is a singleton

    static void initialize_sublibraries();

    [[noreturn]] void read_fpga_counters();

    static float get_power_from_voltage_and_current_values(float voltage, float current);

    void update_power_and_energy_with_last_measure(FPGAPowerCounterState instant_power);

    static FPGAPowerCounterState get_current_power_values(const std::vector<float> &values,
                                                   std::chrono::time_point<std::chrono::high_resolution_clock> timestamp);

    void update_historical_data(const std::vector<float> &values,
                                  std::chrono::time_point<std::chrono::high_resolution_clock> timestamp);

    void initialize_sensors_registration();

    void initialize_historical_data();

    static std::vector<float> get_all_sensor_values();

public:
    static StratixMonitor *getInstance(int32_t period = 10);   // the only way to get access

    ~StratixMonitor();

    FPGAEnergyCounterState get_energy_counters();

    static float get_counter_state_from_sensor(SensorID sensor_id);

    void register_values_for_sensor(SensorID sensor);

    void unregister_values_for_sensor(SensorID sensor);

    std::vector<Measure> get_historical_data(SensorID sensor,
            std::chrono::time_point<std::chrono::high_resolution_clock> start,
            std::chrono::time_point<std::chrono::high_resolution_clock> end);

};

#endif //STRATIXMONITORLIB_STRATIX_MONITOR_H
