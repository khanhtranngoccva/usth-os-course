cmake_minimum_required(VERSION 3.7.2)
project(USTH_OSCourse_S02)

set(CMAKE_CXX_STANDARD 11)

add_executable(cmdTest tst.c readcmd.c)
add_executable(cmdInterpreter readcmd.c shell.cpp)