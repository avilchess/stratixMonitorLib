//
// Created by Antonio Vilches on 12/05/2020.
//

#ifndef STRATIXMONITORLIB_SENSORIDS_H
#define STRATIXMONITORLIB_SENSORIDS_H

using SensorID = int32_t;

namespace SensorId {
    constexpr int32_t sensor_number = 43;      // Sensor number + 1, to use the sensorIds directly.

    constexpr SensorID total_power = 1;
    constexpr SensorID pci_e_voltage = 2;
    constexpr SensorID pci_e_current = 3;
    constexpr SensorID ext_1_voltage = 4;
    constexpr SensorID ext_1_current = 5;
    constexpr SensorID ext_2_voltage = 6;
    constexpr SensorID ext_2_current = 7;
    constexpr SensorID v3_3_pci_voltage = 8;
    constexpr SensorID v3_3_voltage = 9;
    constexpr SensorID v3_3_current = 10;
    constexpr SensorID v3_3_temperature = 11;
    constexpr SensorID core_voltage = 12;
    constexpr SensorID core_current = 13;
    constexpr SensorID core_temperature = 14;
    constexpr SensorID eram_voltage = 15;
    constexpr SensorID eram_current = 16;
    constexpr SensorID eram_temperature = 17;
    constexpr SensorID vccr_voltage = 18;
    constexpr SensorID vccr_current = 19;
    constexpr SensorID vccr_temperature = 20;
    constexpr SensorID vcct_voltage = 21;
    constexpr SensorID v1_8_voltage = 22;
    constexpr SensorID v1_8_current = 23;
    constexpr SensorID v1_8_temperature = 24;
    constexpr SensorID v1_8a_voltage = 25;
    constexpr SensorID v1_8a_current = 26;
    constexpr SensorID v1_8a_temperature = 27;
    constexpr SensorID v2_5_voltage = 28;
    constexpr SensorID v2_5_current = 29;
    constexpr SensorID v2_5_temperature = 30;
    constexpr SensorID v1_2_voltage = 31;
    constexpr SensorID v1_2_current = 32;
    constexpr SensorID v1_2_temperature = 33;
    constexpr SensorID uib_voltage = 34;
    constexpr SensorID uib_current = 35;
    constexpr SensorID uib_temperature = 36;
    constexpr SensorID dimm0_temperature = 37;
    constexpr SensorID dimm1_temperature = 38;
    constexpr SensorID fpga_temperature = 39;
    constexpr SensorID front_temperature = 40;
    constexpr SensorID middle_temperature = 41;
    constexpr SensorID rear_temperature = 42;
}

#endif //STRATIXMONITORLIB_SENSORIDS_H
