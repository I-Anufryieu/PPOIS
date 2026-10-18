cmake_minimum_required(VERSION 3.10)
project(PostMachineProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(PostMachineApp 
    app/main.cpp 
    src/Tape.cpp 
    include/Tape.h 
    src/Command.cpp 
    include/Command.h 
    src/PostMachine.cpp 
    include/PostMachine.h
)

target_include_directories(PostMachineApp PRIVATE include)

# Юнит-тесты
add_executable(UnitTests 
    tests/test.cpp 
    src/Tape.cpp 
    include/Tape.h 
    src/Command.cpp 
    include/Command.h 
    src/PostMachine.cpp 
    include/PostMachine.h 
    tests/catch.hpp
)

target_include_directories(UnitTests PRIVATE include)

target_compile_options(UnitTests PRIVATE --coverage)
target_link_options(UnitTests PRIVATE --coverage)