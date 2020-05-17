//
// Created by Antonio Vilches on 09/05/2020.
//

#include <cstdlib>
#include <iostream>
#include <chrono>

#include "FPGACounters.h"
#include "StratixMonitor.h"
#include "SensorIds.h"

int main(int argc, char *argv[]) {

    //Initialise stratixMonitor library
    StratixMonitor *stratixMonitor = StratixMonitor::getInstance();

    auto counter0 = stratixMonitor->get_energy_counters();
    std::cout << "C0: " << counter0.getCountersState()[FPGAEnergyCounter::total_board_energy] << " joules" << std::endl;

    stratixMonitor->register_values_for_sensor(SensorId::total_power);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    auto timestamp1 = std::chrono::high_resolution_clock::now();

    auto counter1 = stratixMonitor->get_energy_counters();
    std::cout << "C1: " << counter1.getCountersState()[FPGAEnergyCounter::total_board_energy] << " joules" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    auto counter2 = stratixMonitor->get_energy_counters();
    std::cout << "C2: " << counter2.getCountersState()[FPGAEnergyCounter::total_board_energy] << " joules" << std::endl;

    auto timestamp2 = std::chrono::high_resolution_clock::now();
    stratixMonitor->unregister_values_for_sensor(SensorId::total_power);

    auto historic = stratixMonitor->get_historical_data(SensorId::total_power, timestamp1, timestamp2);
    std::cout << "Historical data [Total_power]: " << std::endl;
    for (auto it : historic){
        std::cout << it.first.time_since_epoch().count() << ": " << it.second << std::endl;
    }
    std::cout << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    auto counter3 = stratixMonitor->get_energy_counters();
    std::cout << "C3: " << counter3.getCountersState()[FPGAEnergyCounter::total_board_energy] << " joules" << std::endl;

    std::cout << std::endl;
    std::cout << "Example accessing a specific sensor [Core Temperature]: ";
    std::cout << stratixMonitor->get_counter_state_from_sensor(SensorId::core_temperature) << std::endl << std::endl;

    auto difference31 = counter3 - counter1;
    std::cout << "Difference count3-count1: " << std::endl;

    auto measure = difference31.getCountersState();

    std::cout << "Vccr: " << measure[FPGAEnergyCounter::vccr_energy] << std::endl;
    std::cout << "V1_2: " << measure[FPGAEnergyCounter::v1_2_energy] << std::endl;
    std::cout << "V1_8: " << measure[FPGAEnergyCounter::v1_8_energy] << std::endl;
    std::cout << "V1_8a: " << measure[FPGAEnergyCounter::v1_8a_energy] << std::endl;
    std::cout << "V2_5: " << measure[FPGAEnergyCounter::v2_5_energy] << std::endl;
    std::cout << "V3_3: " << measure[FPGAEnergyCounter::v3_3_energy] << std::endl;
    std::cout << "Core: " << measure[FPGAEnergyCounter::core_energy] << std::endl;
    std::cout << "ERAM: " << measure[FPGAEnergyCounter::eram_energy] << std::endl;
    std::cout << "UIB: " << measure[FPGAEnergyCounter::uib_energy] << std::endl;
    std::cout << "EXT1: " << measure[FPGAEnergyCounter::ext_1_energy] << std::endl;
    std::cout << "EXT2: " << measure[FPGAEnergyCounter::ext_2_energy] << std::endl;
    std::cout << "PCI-E: " << measure[FPGAEnergyCounter::pci_e_energy] << std::endl;
    std::cout << "Total Board: " << measure[FPGAEnergyCounter::total_board_energy] << std::endl;

    return 0;
}