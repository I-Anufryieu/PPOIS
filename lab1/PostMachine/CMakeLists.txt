cmake_minimum_required(VERSION 3.10)
project(PostMachineProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(PostMachineApp main.cpp PostMachine.cpp PostMachine.h)

add_executable(UnitTests tests/test.cpp PostMachine.cpp PostMachine.h tests/catch.hpp)

target_include_directories(UnitTests PRIVATE .)

target_compile_options(UnitTests PRIVATE --coverage)
target_link_options(UnitTests PRIVATE --coverage)