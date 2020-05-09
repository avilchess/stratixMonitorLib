//
// Created by Antonio Vilches on 07/05/2020.
//

#ifndef STRATIXMONITORLIB_STRATIX_MONITOR_H
#define STRATIXMONITORLIB_STRATIX_MONITOR_H

#include "/usr/share/bittware/520nmx/cots/bw_bmc_usb_lib/bw_bmc_usb_lib.h"
#include "/usr/share/bittware/520nmx/cots/bw_mctp_pldm_lib/bw_mctp_pldm_lib.h"
#include <mutex>
#include <thread>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <vector>

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

class HardwareCounters {
private:
    float total_energy;                         // in joules

public:
    HardwareCounters(){
        total_energy = 0.0;
    };

    HardwareCounters(float t_energy){
        total_energy = t_energy;
    };

    HardwareCounters operator + (HardwareCounters const &obj) {
        HardwareCounters res(total_energy + obj.total_energy);
        return res;
    }

    HardwareCounters operator - (HardwareCounters const &obj) {
        HardwareCounters res(total_energy - obj.total_energy);
        return res;
    }

    std::vector<float> getCounters(){
        std::vector<float> res;
        res.push_back(total_energy);
    };
};

class SML{
private:
    // Fields
    usb_dev_handle* usbHandle = NULL;
    BW_MCTP_PLDM_HANDLE mctpPldmHandle;
    static SML * instance;
    std::mutex my_mutex;
    std::thread monitor_thread;
    int32_t time_period;                          // in milliseconds
    HardwareCounters current_state;

    //Methods
    SML(int32_t period);                     // forbidden to call directly because it is a singleton
    void initialize_sublibraries();
    void read_hardware_counters();

public:
    static SML * getInstance(int32_t period = 100);                   // the only way to get access
    HardwareCounters get_hardware_counters();
};

SML * SML::instance = nullptr;


SML::SML(int32_t period){
    time_period = period;
    initialize_sublibraries();
    current_state = HardwareCounters(0.0);
    //monitor_thread = std::thread(read_hardware_counters);

    monitor_thread = std::thread(&SML::read_hardware_counters, this);
    monitor_thread.detach();
}

SML * SML::getInstance(int32_t period) {
    if (instance) return instance;              // no lock here

    //my_mutex.lock();
    if (!instance) instance = new SML(period);
    //my_mutex.unlock();

    return instance;
}

void SML::initialize_sublibraries() {

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

void SML::read_hardware_counters(){

    while (true) {
        float power;
        uint16_t sensorId = 1;
        BwMctpPldm_getNumericSensorReadingById(mctpPldmHandle, &power, sensorId);

        // Integrate power in time to get energy in joules
        float time = (time_period / 1000.0f);
        float energy = power * time;

        auto last_value_counters = HardwareCounters(energy);
        current_state = current_state + last_value_counters;
        std::this_thread::sleep_for(std::chrono::milliseconds(time_period));
    }
}

#endif //STRATIXMONITORLIB_STRATIX_MONITOR_H
