cmake_minimum_required(VERSION 3.7.2)
project(USTH_OSCourse_S02)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(cmdTest tst.c readcmd.c)
add_executable(cmdInterpreter readcmd.c shell.cpp)
add_executable(sync Thread.cpp Thread.h sync.cpp)
add_executable(EX1 Thread.cpp Thread.h EX1.cpp)