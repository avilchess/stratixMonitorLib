//
// Created by Antonio Vilches on 10/05/2020.
//


#ifndef STRATIXMONITORLIB_FPGACOUNTERS_H
#define STRATIXMONITORLIB_FPGACOUNTERS_H

#include <vector>
#include <chrono>

struct counter_t {
  float counter;
  std::chrono::time_point<std::chrono::high_resolution_clock> t_stamp;
};

class FPGACounters {
private:
    float total_energy;                                     // in joules

public:
    FPGACounters();

    FPGACounters(float t_energy);

    FPGACounters operator+(FPGACounters const &obj);

    FPGACounters operator-(FPGACounters const &obj);

    std::vector<float> getCountersState();
};

#endif //STRATIXMONITORLIB_FPGACOUNTERS_H
