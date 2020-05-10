//
// Created by Antonio Vilches on 09/05/2020.
//

#include <cstdlib>
#include <iostream>
#include <chrono>

#include "StratixMonitor.h"
#include "FPGACounters.h"

int main(int argc, char *argv[]) {

    StratixMonitor *StratixMonitor = StratixMonitor::getInstance(100);
    auto counter0 = StratixMonitor->get_fpga_counters();
    std::cout << "Counter0: " << counter0.getCountersState()[0] << "joules." << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    auto counter1 = StratixMonitor->get_fpga_counters();
    std::cout << "Counter1: " << counter1.getCountersState()[0] << "joules." << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    auto counter2 = StratixMonitor->get_fpga_counters();
    std::cout << "Counter2: " << counter2.getCountersState()[0] << "joules." << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    auto counter3 = StratixMonitor->get_fpga_counters();
    std::cout << "Counter3: " << counter3.getCountersState()[0] << "joules." << std::endl;

    auto difference21 = counter2 - counter1;
    std::cout << "Difference count2-count1: " << difference21.getCountersState()[0] << "joules." << std::endl;

    auto difference30 = counter3 - counter0;
    std::cout << "Difference count3-count0: " << difference30.getCountersState()[0] << "joules." << std::endl;

    return 0;
}