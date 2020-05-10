//
// Created by Antonio Vilches on 09/05/2020.
//

#include <cstdlib>
#include <iostream>
#include <chrono>

#include "StratixMonitor.h"

int main (int argc, char *argv[]) {

    StratixMonitor * StratixMonitor = StratixMonitor::getInstance(100);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    auto counter1 = StratixMonitor->get_fpga_counters();
    std::cout << "Counter1: " << counter1.getCountersStates()[0] << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    auto counter2 = StratixMonitor->get_fpga_counters();
    std::cout << "Counter2: " << counter2.getCountersStates()[0] << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    auto counter3 = StratixMonitor->get_fpga_counters();
    std::cout << "Counter3: " << counter3.getCountersStates()[0] << std::endl;

    return 0;
}