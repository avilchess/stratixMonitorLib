cmake_minimum_required(VERSION 3.9.6)
project (StratixMonitor VERSION 0.1.0 LANGUAGES C CXX)

# Using C++ 11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Required
set(CMAKE_CXX_EXTENSIONS ON) # with compiler extensions like gnu++11

set(PRJ_SRC "${PROJECT_SOURCE_DIR}/src")
set(PRJ_INC "${PROJECT_SOURCE_DIR}/include")

if(BW_BMC_USB_LIB)
    set(BW_BMC_USB_LIB "/usr/share/bittware/520nmx/cots/bw_bmc_usb_lib/")
endif()

if(BW_MCTP_PLDM_LIB)
    set(BW_MCTP_PLDM_LIB "/usr/share/bittware/520nmx/cots/bw_mctp_pldm_lib/")
endif()

set(STRATIX_MONITOR_CORE_SOURCES ${PROJECT_SOURCE_DIR}/src/FPGACounters.cpp
        ${PROJECT_SOURCE_DIR}/src/StratixMonitor.cpp)
set(STRATIX_MONITOR_CORE_HEADERS ${PROJECT_SOURCE_DIR}/include/FPGACounters.h
        ${PRJ_INC}/StratixMonitor.h
        ${PRJ_INC}/SensorIds.h
        ${BW_BMC_USB_LIB}/bw_bmc_usb_lib.h
        ${BW_MCTP_PLDM_LIB}/bw_mctp_pldm_lib.h)

add_library(stratixMonitor "")
target_sources(stratixMonitor PRIVATE ${STRATIX_MONITOR_CORE_SOURCES} PUBLIC ${STRATIX_MONITOR_CORE_HEADERS})
target_include_directories(stratixMonitor PUBLIC $<BUILD_INTERFACE:${PRJ_INC}> ${BW_BMC_USB_LIB} ${BW_MCTP_PLDM_LIB})
target_link_libraries(stratixMonitor PUBLIC ${BW_BMC_USB_LIB}/libbw_bmc_usb.so ${BW_MCTP_PLDM_LIB}/libbw_mctp_pldm.so)

add_executable(main src/main.cpp)
target_link_libraries(main PUBLIC stratixMonitor)
target_include_directories(main PUBLIC ${PRJ_INC})
