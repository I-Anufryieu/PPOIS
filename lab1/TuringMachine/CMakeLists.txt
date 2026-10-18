cmake_minimum_required(VERSION 3.10)
project(TuringMachineProject)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(TuringMachineApp 
    app/main.cpp 
    src/TuringMachine.cpp 
    src/Tape.cpp 
    src/Program.cpp 
    src/Parser.cpp
)

add_executable(UnitTests 
    tests/test.cpp 
    src/TuringMachine.cpp 
    src/Tape.cpp 
    src/Program.cpp 
    src/Parser.cpp 
    tests/catch.hpp
)

target_include_directories(UnitTests PRIVATE include .)
target_compile_options(UnitTests PRIVATE --coverage)
target_link_options(UnitTests PRIVATE --coverage)