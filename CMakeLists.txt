cmake_minimum_required (VERSION 2.8)
project (dalek_manager_lib)

# Version Number
set (DMAN_VERSION_MAJOR 0)
set (DMAN_VERSION_MINOR 1)

# Compiler Settings
set (CMAKE_CXX_COMPILER_ID GNU)
set (CMAKE_CXX_FLAGS "-std=c++1y")
set (CMAKE_CXX_VERSION 4.9)

# Directory Structure
set (PROJECT_SOURCE_DIR "src")
set (PROJECT_BINARY_DIR "build")
set (INCLUDE_DIR "include")
set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# Configure header file to pass in cmake settings
configure_file ("${INCLUDE_DIR}/ProjectConfig.h.in" "${PROJECT_BINARY_DIR}/ProjectConfig.h" )

# Add include directories
include_directories("${PROJECT_BINARY_DIR} ${INCLUDE_DIR}")


# Options which default to ON. Don't require external libraries
option (USE_LOG "Compile logging features under Log" ON)
option (USE_CONFIG "Compile config features under Config" ON)

# Options which default to OFF. Require external libraries
option (USE_MQTT "Build mosquitto library and compile its dependents under MQTT/" OFF)
option (USE_WPILIB "Link wpilib and use frc toolchain, as well as compile its dependents under WPILib/" OFF)
option (USE_LUA "Link lua and compile its autonomous mode dependnets under Auton/" OFF)

# Values set in the subdirectories
set (SOURCES "")
set (LIBRARIES "")


# Defines function for adding CPP Files in a directory to the SOURCES variable
function(get_sources arg1)
  file(GLOB function_sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
  set(${arg1} ${function_sources}  PARENT_SCOPE)
endfunction(get_sources)

# Subdirectories
if (USE_LOG)
  add_subdirectory("${PROJECT_SOURCE_DIR}/Log")
endif (USE_LOG)

if (USE_CONFIG)
  add_subdirectory("${PROJECT_SOURCE_DIR}/Config")
endif (USE_CONFIG)

if (USE_MQTT)
  add_subdirectory("${PROJECT_SOURCE_DIR}/MQTT")
endif (USE_MQTT)

if (USE_WPILIB)
  add_subdirectory("${PROJECT_SOURCE_DIR}WPILib")
endif (USE_WPILIB)

if (USE_LUA)
  add_subdirectory("${PROJECT_SOURCE_DIR}/Auton")
endif (USE_LUA)

message("Sources: ${SOURCES}")

# Library target
add_library(dalek-manager STATIC ${SOURCES})

# Link Libraries
target_link_libraries(dalek-manager "${LIBRARIES}")

