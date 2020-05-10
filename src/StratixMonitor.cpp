//
// Created by Antonio Vilches on 10/05/2020.
//

#include <mutex>
#include <thread>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <vector>

#include "StratixMonitor.h"
#include "FPGACounters.h"
#include "/usr/share/bittware/520nmx/cots/bw_bmc_usb_lib/bw_bmc_usb_lib.h"
#include "/usr/share/bittware/520nmx/cots/bw_mctp_pldm_lib/bw_mctp_pldm_lib.h"

BW_MCTP_PLDM_HANDLE mctpPldmHandle;
usb_dev_handle* usbHandle = NULL;

int8_t bmcUsbWriteFunction(uint8_t* buffer, uint16_t* length) {
    int writeResult = bw_bmc_usb_write(usbHandle, buffer, *length);
    if (writeResult == *length)
        return 0;
    return -1;
}

int8_t bmcUsbReadFunction(uint8_t* buffer, uint16_t* length) {
    int readResult;
    readResult = bw_bmc_usb_read(usbHandle, buffer, 1000);
    *length = readResult;
    return 0;
}


StratixMonitor * StratixMonitor::instance = nullptr;

StratixMonitor::StratixMonitor(int32_t period){
    time_period = period;
    initialize_sublibraries();
    current_state = FPGACounters(0.0);
    monitor_thread = std::thread(&StratixMonitor::read_hardware_counters, this);
    monitor_thread.detach();
}

StratixMonitor * StratixMonitor::getInstance(int32_t period) {
    if (instance) return instance;              // no lock here

    //my_mutex.lock();
    if (!instance) instance = new StratixMonitor(period);
    //my_mutex.unlock();

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

void StratixMonitor::read_fpga_counters(){
    while (true) {
        float power;
        uint16_t sensorId = 1;
        BwMctpPldm_getNumericSensorReadingById(mctpPldmHandle, &power, sensorId);

        // Integrate power in time to get energy in joules
        float time = (time_period / 1000.0f);
        float energy = power * time;

        auto last_value_counters = FPGACounters(energy);
        current_state = current_state + last_value_counters;
        std::this_thread::sleep_for(std::chrono::milliseconds(time_period));
    }
}

FPGACounters StratixMonitor::get_fpga_counters(){
    return current_state;
}