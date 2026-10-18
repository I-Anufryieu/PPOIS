cmake_minimum_required(VERSION 3.10)
project(PostMachineProject)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(PostMachineApp 
    app/main.cpp 
    src/Tape.cpp 
    src/Command.cpp 
    src/PostMachine.cpp
)


add_executable(UnitTests 
    tests/test.cpp 
    src/Tape.cpp 
    src/Command.cpp 
    src/PostMachine.cpp 
    tests/catch.hpp
)

target_include_directories(UnitTests PRIVATE include .)
target_compile_options(UnitTests PRIVATE --coverage)
target_link_options(UnitTests PRIVATE --coverage)