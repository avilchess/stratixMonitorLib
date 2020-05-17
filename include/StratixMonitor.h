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
    static int32_t time_period;                                                           // in milliseconds
    static FPGAPowerCounterState power_state;
    static FPGAEnergyCounterState energy_state;
    static std::mutex my_mutex;
    static std::thread monitor_thread;
    static std::vector<std::atomic<int32_t>> sensors_registration;
    static std::map<SensorID, std::vector<Measure>> historical_data;

    //Methods
    StratixMonitor(int32_t period);                             // forbidden to call directly because it is a singleton

    void initialize_sublibraries();

    [[noreturn]] void read_fpga_counters();

    float get_power_from_voltage_and_current_values(float voltage, float current);

    void update_power_and_energy_with_last_measure(FPGAPowerCounterState instant_power);

    FPGAPowerCounterState get_current_power_values(std::vector<float> values,
                                                   std::chrono::time_point<std::chrono::high_resolution_clock> timestamp);

    void update_historical_data(std::vector<float> values,
                                  std::chrono::time_point<std::chrono::high_resolution_clock> timestamp);

    void initialize_sensors_registration();

    std::vector<float> getAllSensorValues();

public:
    static StratixMonitor *getInstance(int32_t period = 10);   // the only way to get access

    FPGAEnergyCounterState get_energy_counters();

    float get_counter_state_from_sensor(SensorID sensor_id);

    void registerValuesForSensor(SensorID sensor);

    void unregisterValuesForSensor(SensorID sensor);

    std::vector<Measure> getHistoricalData(SensorID sensor, std::chrono::time_point<std::chrono::high_resolution_clock> start,
                                           std::chrono::time_point<std::chrono::high_resolution_clock> end);

};

#endif //STRATIXMONITORLIB_STRATIX_MONITOR_H
