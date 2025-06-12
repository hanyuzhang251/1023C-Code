cmake_minimum_required(VERSION 3.15)
project(ChiselFirmware C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.[cS]" "src/*.cpp")

add_executable(dummy_target ${SOURCES})

