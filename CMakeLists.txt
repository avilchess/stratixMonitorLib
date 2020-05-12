cmake_minimum_required(VERSION 3.9.6)
project (StratixMonitor VERSION 0.1.0 LANGUAGES C CXX)

# Using C++ 11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Required
set(CMAKE_CXX_EXTENSIONS ON) # with compiler extensions like gnu++11

set(PRJ_SRC "${PROJECT_SOURCE_DIR}/src")
set(PRJ_INC "${PROJECT_SOURCE_DIR}/include")

set(STRATIX_MONITOR_CORE_SOURCES ${PROJECT_SOURCE_DIR}/src/FPGACounters.cpp
        ${PROJECT_SOURCE_DIR}/src/StratixMonitor.cpp)
set(STRATIX_MONITOR_CORE_HEADERS ${PROJECT_SOURCE_DIR}/include/FPGACounters.h
        ${PROJECT_SOURCE_DIR}/include/StratixMonitor.h
        ${PROJECT_SOURCE_DIR}/include/SensorIds.h)

add_library(stratixMonitor "")
target_sources(stratixMonitor PRIVATE ${STRATIX_MONITOR_CORE_SOURCES} PUBLIC ${STRATIX_MONITOR_CORE_HEADERS})
target_include_directories(stratixMonitor PUBLIC $<BUILD_INTERFACE:${PRJ_INC}>)

add_executable(main src/main.cpp)
target_link_libraries(main PUBLIC stratixMonitor)
target_include_directories(main PUBLIC ${PRJ_INC})
