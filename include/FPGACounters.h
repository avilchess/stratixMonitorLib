//
// Created by Antonio Vilches on 10/05/2020.
//


#ifndef STRATIXMONITORLIB_FPGACOUNTERS_H
#define STRATIXMONITORLIB_FPGACOUNTERS_H

#include <vector>
#include <chrono>

namespace FPGAEnergyCounter {
    constexpr int32_t total_board_energy = 0;
    constexpr int32_t pci_e_energy = 1;
    constexpr int32_t ext_1_energy = 2;
    constexpr int32_t ext_2_energy = 3;
    constexpr int32_t v3_3_energy = 4;
    constexpr int32_t core_energy = 5;
    constexpr int32_t eram_energy = 6;
    constexpr int32_t vccr_energy = 7;
    constexpr int32_t v1_8_energy = 8;
    constexpr int32_t v1_8a_energy = 9;
    constexpr int32_t v2_5_energy = 10;
    constexpr int32_t v1_2_energy = 11;
    constexpr int32_t uib_energy = 12;
}

class FPGAEnergyCounterState {
private:
    float total_board;
    float pci_e;
    float ext_1;
    float ext_2;
    float v3_3;
    float core;
    float eram;
    float vccr;
    float v1_8;
    float v1_8a;
    float v2_5;
    float v1_2;
    float uib;

public:
    FPGAEnergyCounterState();

    FPGAEnergyCounterState(float total_board, float pci_e, float ext_1,
                           float ext_2, float v3_3, float core, float eram, float vccr,
                           float v1_8, float v1_8a, float v2_5, float v1_2, float uib);

    FPGAEnergyCounterState operator+(FPGAEnergyCounterState const &obj);

    FPGAEnergyCounterState operator-(FPGAEnergyCounterState const &obj);

    std::vector<float> getCountersState();
};

class FPGAPowerCounterState {
private:
    float total_board;
    float pci_e;
    float ext_1;
    float ext_2;
    float v3_3;
    float core;
    float eram;
    float vccr;
    float v1_8;
    float v1_8a;
    float v2_5;
    float v1_2;
    float uib;

    std::chrono::time_point<std::chrono::high_resolution_clock> timestamp;
public:
    FPGAPowerCounterState();

    FPGAPowerCounterState(float total_board, float pci_e, float ext_1,
                          float ext_2, float v3_3, float core, float eram, float vccr,
                          float v1_8, float v1_8a, float v2_5, float v1_2, float uib,
                          std::chrono::time_point<std::chrono::high_resolution_clock> timestamp);

    FPGAEnergyCounterState operator*(float time);

    std::chrono::time_point<std::chrono::high_resolution_clock> get_timestamp();

    FPGAEnergyCounterState
    integrate_energy_from_timestamp(std::chrono::time_point<std::chrono::high_resolution_clock> timestamp_last_measure);

};

#endif //STRATIXMONITORLIB_FPGACOUNTERS_H
