cmake_minimum_required(VERSION 3.30)
project(arduino_test)

set(CMAKE_CXX_STANDARD 20)

add_executable(arduino_test emulator.cpp)
