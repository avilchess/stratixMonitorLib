//
// Created by Antonio Vilches on 09/05/2020.
//

#include <stratix_monitor.h>

int main (int argc, char *argv[]) {

    SLM * slm = SLM::getInstance();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    auto counter1 = slm->get_hardware_counters();
    std::cout << "Counter1: " << counter1.getCounters()[0] << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    auto counter2 = slm->get_hardware_counters();
    std::cout << "Counter2: " << counter2.getCounters()[0] << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    auto counter3 = slm->get_hardware_counters();
    std::cout << "Counter3: " << counter3.getCounters()[0] << std::endl;

    return 0;
}