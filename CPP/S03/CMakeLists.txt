cmake_minimum_required(VERSION 3.7.2)
project(USTH_OSCourse_S02)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(EX1 EX1.cpp)
add_executable(mem mem.cpp)