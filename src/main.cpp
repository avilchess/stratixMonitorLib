//
// Created by Antonio Vilches on 09/05/2020.
//

#include <cstdlib>
#include <iostream>
#include <chrono>

#include "FPGACounters.h"
#include "StratixMonitor.h"

int main(int argc, char *argv[]) {

    //Initialise StratixMonitor library
    StratixMonitor *StratixMonitor = StratixMonitor::getInstance();

    auto counter0 = StratixMonitor->get_fpga_counters();
    std::cout << "C0: " << counter0.getCountersState()[FPGAEnergyCounter::total_board_energy] << " joules" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    auto counter1 = StratixMonitor->get_fpga_counters();
    std::cout << "C1: " << counter1.getCountersState()[FPGAEnergyCounter::total_board_energy] << " joules" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    auto counter2 = StratixMonitor->get_fpga_counters();
    std::cout << "C2: " << counter2.getCountersState()[FPGAEnergyCounter::total_board_energy] << " joules" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    auto counter3 = StratixMonitor->get_fpga_counters();
    std::cout << "C3: " << counter3.getCountersState()[FPGAEnergyCounter::total_board_energy] << " joules" << std::endl;

    auto difference31 = counter3 - counter1;
    std::cout << "Difference count2-count1" << std::endl;
    std::cout << "Core: " << difference31.getCountersState()[FPGAEnergyCounter::core_energy] << std::endl;
    std::cout << "ERAM: " << difference31.getCountersState()[FPGAEnergyCounter::eram_energy] << std::endl;
    std::cout << "UIB: " << difference31.getCountersState()[FPGAEnergyCounter::uib_energy] << std::endl;
    std::cout << "EXT1: " << difference31.getCountersState()[FPGAEnergyCounter::ext_1_energy] << std::endl;
    std::cout << "EXT2: " << difference31.getCountersState()[FPGAEnergyCounter::ext_2_energy] << std::endl;
    std::cout << "PCI-E: " << difference31.getCountersState()[FPGAEnergyCounter::pci_e_energy] << std::endl;
    std::cout << "Total Board: " << difference31.getCountersState()[FPGAEnergyCounter::total_board_energy] << std::endl;

    return 0;
}