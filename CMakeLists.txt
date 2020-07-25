cmake_minimum_required(VERSION 2.8.9)
project(xgtvm)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(xgtvm programs/xgtvm.cpp libraries/machine.cpp libraries/machine.hpp)
target_include_directories(xgtvm PRIVATE libraries)
add_executable(xgtvm_tests programs/xgtvm_tests.cpp libraries/machine.cpp libraries/machine.hpp)
target_include_directories(xgtvm_tests PRIVATE libraries)
