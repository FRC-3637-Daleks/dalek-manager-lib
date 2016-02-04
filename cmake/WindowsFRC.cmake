cmake_minimum_required(VERSION 2.8)
set(ARM_PREFIX arm-frc-linux-gnueabi)

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSROOT $ENV{FRC})


set(CMAKE_C_COMPILER ${CMAKE_SYSROOT}/bin/${ARM_PREFIX}-gcc.exe)
set(CMAKE_CXX_COMPILER ${CMAKE_SYSROOT}/bin/${ARM_PREFIX}-g++.exe)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)