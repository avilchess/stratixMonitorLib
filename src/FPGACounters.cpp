//
// Created by Antonio Vilches on 10/05/2020.
//

#include "FPGACounters.h"

FPGACounters::FPGACounters() {
    total_energy = 0.0;
};

FPGACounters::FPGACounters(float t_energy) {
    total_energy = t_energy;
};

FPGACounters FPGACounters::operator+(FPGACounters const &obj) {
    FPGACounters res(total_energy + obj.total_energy);
    return res;
}

FPGACounters FPGACounters::operator-(FPGACounters const &obj) {
    FPGACounters res(total_energy - obj.total_energy);
    return res;
}

std::vector<float> FPGACounters::getCountersState() {
    std::vector<float> res;
    res.push_back(total_energy);
    return res;
}
