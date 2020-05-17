//
// Created by Antonio Vilches on 10/05/2020.
//

#include "FPGACounters.h"
#include <vector>

FPGAEnergyCounterState::FPGAEnergyCounterState() {
    total_board = 0.0;
    pci_e = 0.0;
    ext_1 = 0.0;
    ext_2 = 0.0;
    v3_3 = 0.0;
    core = 0.0;
    eram = 0.0;
    vccr = 0.0;
    v1_8 = 0.0;
    v1_8a = 0.0;
    v2_5 = 0.0;
    v1_2 = 0.0;
    uib = 0.0;
};

FPGAEnergyCounterState::FPGAEnergyCounterState(float total_board, float pci_e, float ext_1,
                                               float ext_2, float v3_3, float core, float eram, float vccr,
                                               float v1_8, float v1_8a, float v2_5, float v1_2, float uib) :
        total_board(total_board),
        pci_e(pci_e),
        ext_1(ext_1),
        ext_2(ext_2),
        v3_3(v3_3),
        core(core),
        eram(eram),
        vccr(vccr),
        v1_8(v1_8),
        v1_8a(v1_8a),
        v2_5(v2_5),
        v1_2(v1_2),
        uib(uib) {}

FPGAEnergyCounterState FPGAEnergyCounterState::operator+(FPGAEnergyCounterState const &obj) {

    FPGAEnergyCounterState res(total_board + obj.total_board,
                           pci_e + obj.pci_e,
                           ext_1 + obj.ext_1,
                           ext_2 + obj.ext_2,
                           v3_3 + obj.v3_3,
                           core + obj.core,
                           eram + obj.eram,
                           vccr + obj.vccr,
                           v1_8 + obj.v1_8,
                           v1_8a + obj.v1_8a,
                           v2_5 + obj.v2_5,
                           v1_2 + obj.v1_2,
                           uib + obj.uib);

    return res;
}

FPGAEnergyCounterState FPGAEnergyCounterState::operator-(FPGAEnergyCounterState const &obj) {

    FPGAEnergyCounterState res(total_board - obj.total_board,
                           pci_e - obj.pci_e,
                           ext_1 - obj.ext_1,
                           ext_2 - obj.ext_2,
                           v3_3 - obj.v3_3,
                           core - obj.core,
                           eram - obj.eram,
                           vccr - obj.vccr,
                           v1_8 - obj.v1_8,
                           v1_8a - obj.v1_8a,
                           v2_5 - obj.v2_5,
                           v1_2 - obj.v1_2,
                           uib - obj.uib);
    return res;
}

/**
 * This method returns the counter states for a given measurement of FPGA counters.
 * @return
 */
std::vector<float> FPGAEnergyCounterState::getCountersState() {
    std::vector<float> res;
    res.push_back(total_board);
    res.push_back(pci_e);
    res.push_back(ext_1);
    res.push_back(ext_2);
    res.push_back(v3_3);
    res.push_back(core);
    res.push_back(eram);
    res.push_back(vccr);
    res.push_back(v1_8);
    res.push_back(v1_8a);
    res.push_back(v2_5);
    res.push_back(v1_2);
    res.push_back(uib);
    return res;
}

FPGAPowerCounterState::FPGAPowerCounterState() {
    total_board = 0.0;
    pci_e = 0.0;
    ext_1 = 0.0;
    ext_2 = 0.0;
    v3_3 = 0.0;
    core = 0.0;
    eram = 0.0;
    vccr = 0.0;
    v1_8 = 0.0;
    v1_8a = 0.0;
    v2_5 = 0.0;
    v1_2 = 0.0;
    uib = 0.0;
    timestamp = std::chrono::high_resolution_clock::now();
};

FPGAPowerCounterState::FPGAPowerCounterState(float total_board, float pci_e, float ext_1,
                                             float ext_2, float v3_3, float core, float eram, float vccr,
                                             float v1_8, float v1_8a, float v2_5, float v1_2, float uib,
                                             std::chrono::time_point<std::chrono::high_resolution_clock> timestamp) :
        total_board(total_board),
        pci_e(pci_e),
        ext_1(ext_1),
        ext_2(ext_2),
        v3_3(v3_3),
        core(core),
        eram(eram),
        vccr(vccr),
        v1_8(v1_8),
        v1_8a(v1_8a),
        v2_5(v2_5),
        v1_2(v1_2),
        uib(uib),
        timestamp(timestamp) {}

std::chrono::time_point<std::chrono::high_resolution_clock> FPGAPowerCounterState::get_timestamp() {
    return timestamp;
}

FPGAEnergyCounterState FPGAPowerCounterState::integrate_energy_from_timestamp(
        std::chrono::time_point<std::chrono::high_resolution_clock> timestamp_last_measure) {
    float time = std::chrono::duration<float>(timestamp - timestamp_last_measure).count();
    return *this * time;
}

FPGAEnergyCounterState FPGAPowerCounterState::operator*(float time) {
    return FPGAEnergyCounterState(this->total_board * time,
                              this->pci_e * time,
                              this->ext_1 * time,
                              this->ext_2 * time,
                              this->v3_3 * time,
                              this->core * time,
                              this->eram * time,
                              this->vccr * time,
                              this->v1_8 * time,
                              this->v1_8a * time,
                              this->v2_5 * time,
                              this->v1_2 * time,
                              this->uib * time
    );
}
