cmake_minimum_required (VERSION 3.0)
project (dalek_manager_lib)

# include(${CMAKE_TOOLCHAIN_FILE})
set (DEFAULT_TOOLCHAIN ${CMAKE_TOOLCHAIN_FILE})

# Custom Module Path
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

# Version Number
set (DMAN_VERSION_MAJOR "0")
set (DMAN_VERSION_MINOR "1")

# Compiler Settings
# set (CMAKE_CXX_COMPILER_ID GNU)
set (CMAKE_CXX_FLAGS "-std=c++1y")
set (CMAKE_CXX_VERSION 4.9)

# Directory Structure
set (PROJECT_SOURCE_DIR "src")
set (PROJECT_BINARY_DIR "build")
set (OUTPUT_INCLUDE_DIR "${PROJECT_BINARY_DIR}/include")
set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# Configure header file to pass in cmake settings
configure_file ("${PROJECT_SOURCE_DIR}/ProjectConfig.h.in" "${PROJECT_SOURCE_DIR}/ProjectConfig.h" )

# Add include directories
# include_directories("${PROJECT_SOURCE_DIR}")


# Options which default to ON. Don't require external libraries
option (USE_LOG "Compile logging features under Log" ON)
option (USE_CONFIG "Compile config features under Config" ON)

# Options which default to OFF. Require external libraries
option (USE_MQTT "Build mosquitto library and compile its dependents under MQTT/" OFF)
option (USE_WPILIB "Link wpilib and use frc toolchain, as well as compile its dependents under WPILib/" OFF)
option (USE_LUA "Link lua and compile its autonomous mode dependnets under Auton/" OFF)

# Values set in the subdirectories
set (SOURCES)
set (LIBRARIES)
set (HEADERS)
set (INCLUDES)
set (INC_DIRS ${PROJECT_SOURCE_DIR})

# Defines function for adding CPP Files in a directory to the SOURCES variable
function(get_sources SOURCE_ARG SOURCE_HEADER_ARG INCLUDE_ARG)
  file(GLOB function_sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
  set(${SOURCE_ARG} ${function_sources}  PARENT_SCOPE)
  file(GLOB function_source_headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  set(${SOURCE_HEADER_ARG} ${function_source_headers} PARENT_SCOPE)
  file(GLOB_RECURSE function_includes "${CMAKE_CURRENT_SOURCE_DIR}/*.(h|hpp)")
  set(${INCLUDE_ARG} ${function_includes} PARENT_SCOPE)
endfunction(get_sources)

# Subdirectories

if (USE_WPILIB)
  message ("Using WPILIB")
  # Modify toolchain and libraries
  execute_process(COMMAND frc-cmake-toolchain OUTPUT_VARIABLE CMAKE_TOOLCHAIN_FILE)
  add_subdirectory("${PROJECT_SOURCE_DIR}/WPILib")
else (USE_WPILIB)
  set (CMAKE_TOOLCHAIN_FILE ${DEFAULT_TOOLCHAIN})
endif (USE_WPILIB)

# Checks to see which libs to look up
if (CMAKE_TOOLCHAIN_FILE)
  list (APPEND CMAKE_FIND_ROOT_PATH "/opt")
else (CMAKE_TOOLCHAIN_FILE)
  unset (CMAKE_FIND_ROOT_PATH)
endif (CMAKE_TOOLCHAIN_FILE)

string (STRIP "${CMAKE_TOOLCHAIN_FILE}" CMAKE_TOOLCHAIN_FILE)
include ("${CMAKE_TOOLCHAIN_FILE}")

if (USE_LOG)
  add_subdirectory("${PROJECT_SOURCE_DIR}/Log")
endif (USE_LOG)

if (USE_CONFIG)
  add_subdirectory("${PROJECT_SOURCE_DIR}/Config")
endif (USE_CONFIG)

if (USE_MQTT)
  add_subdirectory("${PROJECT_SOURCE_DIR}/MQTT")
endif (USE_MQTT)

if (USE_LUA)
  add_subdirectory("${PROJECT_SOURCE_DIR}/Auton")
endif (USE_LUA)

message("${INC_DIRS}")
include_directories ("${INC_DIRS}")

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()

# Library target
string (REPLACE " " ";" SOURCES_LIST "${SOURCES}")
add_library(dalek-manager SHARED ${SOURCES_LIST})

# Link Libraries
target_link_libraries(dalek-manager ${LIBRARIES})

# Linter
include(cmake/CppLint.cmake)
add_style_check_target(dalek-manager "${SOURCES} ${HEADERS}")

