//
// Created by Antonio Vilches on 10/05/2020.
//

#include <cstdlib>
#include <iostream>
#include <mutex>
#include <atomic>
#include "StratixMonitor.h"
#include "FPGACounters.h"
#include "SensorIds.h"
#include "bw_bmc_usb_lib.h"
#include "bw_mctp_pldm_lib.h"

BW_MCTP_PLDM_HANDLE mctpPldmHandle;
usb_dev_handle *usbHandle = NULL;

int8_t bmcUsbWriteFunction(uint8_t *buffer, uint16_t *length) {
    int writeResult = bw_bmc_usb_write(usbHandle, buffer, *length);
    if (writeResult == *length)
        return 0;
    return -1;
}

int8_t bmcUsbReadFunction(uint8_t *buffer, uint16_t *length) {
    int readResult;
    readResult = bw_bmc_usb_read(usbHandle, buffer, 1000);
    *length = readResult;
    return 0;
}

StratixMonitor *StratixMonitor::instance = nullptr;

std::mutex StratixMonitor::my_mutex;

void StratixMonitor::initialize_sensors_registration() {
    if (sensors_registration.empty()) {
        sensors_registration = std::vector<std::atomic<int32_t>>(SensorId::sensor_number);
        for (int i = 0; i < SensorId::sensor_number; i++) {
            sensors_registration.at(i) = 0;
        }
    }
}

void StratixMonitor::initialize_historical_data() {
    if (historical_data.empty()) {
        for (int32_t i = 0; i < SensorId::sensor_number; i++) {
            std::vector<Measure> measures;
            historical_data.insert({i, measures});
        }
    }
}

StratixMonitor::StratixMonitor(int32_t period) {
    time_period = period;
    initialize_sublibraries();
    initialize_sensors_registration();
    initialize_historical_data();
    control_thread = 1;

    auto timestamp = std::chrono::high_resolution_clock::now();
    auto values = get_all_sensor_values();
    update_historical_data(values, timestamp);

    power_state = get_current_power_values(values, timestamp);
    energy_state = FPGAEnergyCounterState();
    monitor_thread = std::thread(&StratixMonitor::read_fpga_counters, this);
}

StratixMonitor *StratixMonitor::getInstance(int32_t period) {
    if (instance) return instance;                              // no lock here

    my_mutex.lock();
    if (!instance) instance = new StratixMonitor(period);
    my_mutex.unlock();

    return instance;
}

void StratixMonitor::initialize_sublibraries() {

    // Get a USB connection to the BMC
    usbHandle = bw_bmc_usb_open();
    if (!usbHandle) {
        std::cout << "Failed to open USB interface to BMC!" << std::endl;
        exit(-1);
    }
    //initialize BwMctpPldm library
    mctpPldmHandle = BwMctpPldm_initialise(bmcUsbWriteFunction, bmcUsbReadFunction);
    if (!mctpPldmHandle) {
        std::cout << "Failed to initialise MCTP/PLDM Library!" << std::endl;
        exit(-1);
    }
}

void StratixMonitor::update_power_and_energy_with_last_measure(FPGAPowerCounterState instant_power) {
    auto timestamp_last_measure = power_state.get_timestamp();
    auto new_energy = instant_power.integrate_energy_from_timestamp(timestamp_last_measure);

    power_state = instant_power;
    energy_state = energy_state + new_energy;
}


std::vector<float> StratixMonitor::get_all_sensor_values() {
    std::vector<float> data(SensorId::sensor_number, 0.0);

    for (int32_t i = 1; i < SensorId::sensor_number; i++) {
        data[i] = get_counter_state_from_sensor(i);
    }

    return data;
}

void StratixMonitor::update_historical_data(const std::vector<float> &sensor_values,
                                            std::chrono::time_point<std::chrono::high_resolution_clock> timestamp) {

    for (int32_t i = 1; i < sensors_registration.size(); i++) {

        if (sensors_registration[i].fetch_add(0)) {
            historical_data[i].emplace_back(timestamp, sensor_values[i]);
        }
    }
}

void StratixMonitor::read_fpga_counters() {
    while (control_thread.fetch_add(0)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(time_period));

        auto timestamp = std::chrono::high_resolution_clock::now();
        auto values = get_all_sensor_values();
        update_historical_data(values, timestamp);

        auto instant_power = get_current_power_values(values, timestamp);
        update_power_and_energy_with_last_measure(instant_power);
    }
}

FPGAEnergyCounterState StratixMonitor::get_energy_counters() {
    return energy_state;
}

float StratixMonitor::get_counter_state_from_sensor(int32_t sensor_id) {
    float measure;
    BwMctpPldm_getNumericSensorReadingById(mctpPldmHandle, &measure, sensor_id);
    return measure;
}

float StratixMonitor::get_power_from_voltage_and_current_values(float voltage, float current) {
    return voltage * current;
}

FPGAPowerCounterState StratixMonitor::get_current_power_values(const std::vector<float> &values,
                                                               std::chrono::time_point<std::chrono::high_resolution_clock> timestamp) {

    return {
            values[SensorId::total_power],
            get_power_from_voltage_and_current_values(values[SensorId::pci_e_voltage], values[SensorId::pci_e_current]),
            get_power_from_voltage_and_current_values(values[SensorId::ext_1_voltage], values[SensorId::ext_1_current]),
            get_power_from_voltage_and_current_values(values[SensorId::ext_2_voltage], values[SensorId::ext_2_current]),
            get_power_from_voltage_and_current_values(values[SensorId::v3_3_voltage], values[SensorId::v3_3_current]),
            get_power_from_voltage_and_current_values(values[SensorId::core_voltage], values[SensorId::core_current]),
            get_power_from_voltage_and_current_values(values[SensorId::eram_voltage], values[SensorId::eram_current]),
            get_power_from_voltage_and_current_values(values[SensorId::vccr_voltage], values[SensorId::vccr_current]),
            get_power_from_voltage_and_current_values(values[SensorId::v1_8_voltage], values[SensorId::v1_8_current]),
            get_power_from_voltage_and_current_values(values[SensorId::v1_8a_voltage], values[SensorId::v1_8a_current]),
            get_power_from_voltage_and_current_values(values[SensorId::v2_5_voltage], values[SensorId::v2_5_current]),
            get_power_from_voltage_and_current_values(values[SensorId::v1_2_voltage], values[SensorId::v1_2_current]),
            get_power_from_voltage_and_current_values(values[SensorId::uib_voltage], values[SensorId::uib_current]),
            timestamp};
}


void StratixMonitor::register_values_for_sensor(SensorID sensor) {
    sensors_registration[sensor]++;
}

void StratixMonitor::unregister_values_for_sensor(SensorID sensor) {
    sensors_registration[sensor]--;
}

std::vector<Measure>
StratixMonitor::get_historical_data(SensorID sensor, std::chrono::time_point<std::chrono::high_resolution_clock> start,
                                    std::chrono::time_point<std::chrono::high_resolution_clock> end) {

    std::vector<Measure> res;
    auto data = historical_data[sensor];

    for (auto it : data) {
        if ((it.first > start) && (it.first < end)) {
            res.push_back(it);
        }
    }
    return res;
}

StratixMonitor::~StratixMonitor() {
    // This will kill the monitor_thread
    control_thread = 0;
}
