//
// Created by Antonio Vilches on 10/05/2020.
//

#include <cstdlib>
#include <iostream>

#include "StratixMonitor.h"
#include "FPGACounters.h"
#include "SensorIds.h"
#include "/usr/share/bittware/520nmx/cots/bw_bmc_usb_lib/bw_bmc_usb_lib.h"
#include "/usr/share/bittware/520nmx/cots/bw_mctp_pldm_lib/bw_mctp_pldm_lib.h"

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

StratixMonitor::StratixMonitor(int32_t period) {
    time_period = period;
    initialize_sublibraries();

    power_state = get_current_power_counters();
    energy_state = FPGAEnergyCounters();
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

void StratixMonitor::update_power_and_energy_with_last_measure(FPGAPowerCounters instant_power) {
    auto timestamp_last_measure = power_state.get_timestamp();
    auto new_energy = instant_power.integrate_energy_from_timestamp(timestamp_last_measure);

    power_state = instant_power;
    energy_state = energy_state + new_energy;
}


[[noreturn]] void StratixMonitor::read_fpga_counters() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(time_period));
        auto instant_power = get_current_power_counters();
        update_power_and_energy_with_last_measure(instant_power);
    }
}

FPGAEnergyCounters StratixMonitor::get_fpga_counters() {
    return energy_state;
}

float StratixMonitor::get_counter_state_from_sensor(int32_t sensor_id) {
    float measure;
    BwMctpPldm_getNumericSensorReadingById(mctpPldmHandle, &measure, sensor_id);
    return measure;
}

float StratixMonitor::get_power_from_voltage_and_current_sensors(int32_t sensor_voltage, int32_t sensor_current) {
    float voltage = get_counter_state_from_sensor(sensor_voltage);
    float current = get_counter_state_from_sensor(sensor_current);
    BwMctpPldm_getNumericSensorReadingById(mctpPldmHandle, &current, sensor_current);

    return voltage * current;
}

FPGAPowerCounters StratixMonitor::get_current_power_counters() {
    auto timestamp = std::chrono::high_resolution_clock::now();

    return FPGAPowerCounters(
            get_counter_state_from_sensor(SensorId::total_power),
            get_power_from_voltage_and_current_sensors(SensorId::pci_e_voltage, SensorId::pci_e_current),
            get_power_from_voltage_and_current_sensors(SensorId::ext_1_voltage, SensorId::ext_1_current),
            get_power_from_voltage_and_current_sensors(SensorId::ext_2_voltage, SensorId::ext_2_current),
            get_power_from_voltage_and_current_sensors(SensorId::v3_3_voltage, SensorId::v3_3_current),
            get_power_from_voltage_and_current_sensors(SensorId::core_voltage, SensorId::core_current),
            get_power_from_voltage_and_current_sensors(SensorId::eram_voltage, SensorId::eram_current),
            get_power_from_voltage_and_current_sensors(SensorId::vccr_voltage, SensorId::vccr_current),
            get_power_from_voltage_and_current_sensors(SensorId::v1_8_voltage, SensorId::v1_8_current),
            get_power_from_voltage_and_current_sensors(SensorId::v1_8a_voltage, SensorId::v1_8a_current),
            get_power_from_voltage_and_current_sensors(SensorId::v2_5_voltage, SensorId::v2_5_current),
            get_power_from_voltage_and_current_sensors(SensorId::v1_2_voltage, SensorId::v1_2_current),
            get_power_from_voltage_and_current_sensors(SensorId::uib_voltage, SensorId::uib_current),
            timestamp);
}
