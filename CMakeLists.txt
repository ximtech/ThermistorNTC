cmake_minimum_required(VERSION 3.20)
project(ThermistorNTC C)

set(CMAKE_C_STANDARD 99)

include(cmake/CPM.cmake)

CPMAddPackage(
        NAME STM32Core
        GITHUB_REPOSITORY ximtech/STM32Core
        GIT_TAG origin/main)

set(THERMISTOR_NTC_DIRECTORY
        ${ADC_IT_DIRECTORY}
        ${ThermistorNTC_SOURCE_DIR}
        ${ThermistorNTC_SOURCE_DIR}/include
        CACHE STRING "Thermistor NTC directories include to the main project" FORCE)

set(THERMISTOR_NTC_SOURCES
        ${ADC_IT_SOURCES}
        ${ThermistorNTC_SOURCE_DIR}/ThermistorNTC.c
        ${ThermistorNTC_SOURCE_DIR}/include/ThermistorNTC.h
        CACHE STRING "Thermistor NTC source files include to the main project" FORCE)
