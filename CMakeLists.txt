cmake_minimum_required (VERSION 3.0)

set (CMAKE_CXX_VERSION 4.9)

if (USE_WPILIB)
  message ("Using WPILIB")
  # Modify toolchain and libraries
  if (WIN32)
    message ("Winblows")
	set(CMAKE_TOOLCHAIN_FILE "cmake/WindowsFRC.cmake")
  else (WIN32)
    message("Good")
    execute_process(COMMAND frc-cmake-toolchain OUTPUT_VARIABLE CMAKE_TOOLCHAIN_FILE)
  endif (WIN32)
else (USE_WPILIB)
  set (CMAKE_TOOLCHAIN_FILE ${DEFAULT_TOOLCHAIN})
endif (USE_WPILIB)


project (dalek_manager_lib)

# include(${CMAKE_TOOLCHAIN_FILE})
set (DEFAULT_TOOLCHAIN ${CMAKE_TOOLCHAIN_FILE})

# Custom Module Path
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

# Version Number
set (DMAN_VERSION_MAJOR "0")
set (DMAN_VERSION_MINOR "1")
execute_process(COMMAND git rev-parse HEAD OUTPUT_VARIABLE DMAN_GIT_REV)
string (STRIP ${DMAN_GIT_REV} DMAN_GIT_REV)
set (DMAN_GIT_REV "\"${DMAN_GIT_REV}\"")

# Compiler Settings
# set (CMAKE_CXX_COMPILER_ID GNU)
set (CMAKE_CXX_FLAGS "-std=c++1y")

# Directory Structure
set (PROJECT_SOURCE_DIR "src")
set (PROJECT_BINARY_DIR "build")
set (OUTPUT_INCLUDE_DIR "${PROJECT_BINARY_DIR}/include")
set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set (LIBRARIES)

# Configure header file to pass in cmake settings
configure_file ("${PROJECT_SOURCE_DIR}/ProjectConfig.h.in" "${PROJECT_SOURCE_DIR}/ProjectConfig.h" )

# Configure Doxyfile for documentation generation
configure_file ("Doxyfile.in" "Doxyfile")
add_custom_target (doc
			doxygen Doxyfile
			COMMENT "Generating API documentation with Doxygen" VERBATIM
)

# Add include directories
include_directories("${PROJECT_SOURCE_DIR}")


# Required core of the library
# option (USE_LOG "Compile logging features under Log" ON)
# option (USE_CONFIG "Compile config features under Config" ON)
set (USE_LOG ON)
set (USE_CONFIG ON)
set (USE_UTILITY ON)

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

function(get_sources subdir)
  file(GLOB function_sources "${CMAKE_CURRENT_SOURCE_DIR}/src/${subdir}/*.cpp")
  set(local_src ${SOURCES})
  list(APPEND local_src ${function_sources})
  set(SOURCES ${local_src} PARENT_SCOPE)
  file(GLOB function_source_headers "${CMAKE_CURRENT_SOURCE_DIR}/src/${subdir}/*.h")
  set(local_headers ${HEADERS})
  list(APPEND local_headers ${function_source_headers})
  set(HEADERS ${local_headers} PARENT_SCOPE)
  file(GLOB_RECURSE function_includes "${CMAKE_CURRENT_SOURCE_DIR}/src/${subdir}/*.(h|hpp)")
  set(local_includes ${INCLUDES})
  list(APPEND local_includes ${function_includes})
  set(INCLUDES ${local_includes} PARENT_SCOPE)
endfunction(get_sources)

# Subdirectories

if (USE_WPILIB)
  message ("Using WPILIB")
  get_sources("WPILib")
  find_package(WPILib REQUIRED)
  if(WPILIB_FOUND)
    include_directories(${WPILIB_INCLUDES})
	list (APPEND LIBRARIES ${WPILIB_LIBRARIES})
  endif()
endif (USE_WPILIB)

# Checks to see which libs to look up
if (CMAKE_TOOLCHAIN_FILE)
  message ("Using frc boost")
  # list (APPEND CMAKE_FIND_ROOT_PATH "${CMAKE_SOURCE_DIR}/" "${CMAKE_SOURCE_DIR}/arm-frc-linux-gnueabi/")
  set (Boost_INCLUDE_DIR ${CMAKE_SOURCE_DIR})
  set (Boost_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/arm-frc-linux-gnueabi/)
  message("Boost_LIBRARY_DIR: ${Boost_LIBRARY_DIR}")
else (CMAKE_TOOLCHAIN_FILE)
  list (APPEND CMAKE_FIND_ROOT_PATH "${CMAKE_SOURCE_DIR}/lib/")
  message ("ye")
  unset (CMAKE_FIND_ROOT_PATH)
endif (CMAKE_TOOLCHAIN_FILE)

string (STRIP "${CMAKE_TOOLCHAIN_FILE}" CMAKE_TOOLCHAIN_FILE)
include ("${CMAKE_TOOLCHAIN_FILE}")

if (USE_LOG)
  get_sources("Log")

  set (Boost_THREADAPI "pthread")
  set (Boost_USE_STATIC_LIBS "ON")

  message("CMAKE_FIND_ROOT_PATH: ${CMAKE_FIND_ROOT_PATH}")
  find_package(Boost REQUIRED log_setup log filesystem system thread)

  if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    list (APPEND LIBRARIES ${Boost_LIBRARIES} pthread)
   
    # Set for dynamic linking
    # set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_ALL_DYN_LINK")
  endif(Boost_FOUND)
  
endif (USE_LOG)

if (USE_CONFIG)
  get_sources("Config")

  include_directories("${PROJECT_SOURCE_DIR}/Config/json/src")

  find_package(Boost REQUIRED)

  if(Boost_FOUND)
    include_directories("${Boost_INCLUDE_DIRS}")
  endif(Boost_FOUND)
  
endif (USE_CONFIG)

if (USE_UTILITY)
  get_sources("Utility")

  find_package(Boost REQUIRED)

  if(Boost_FOUND)
    include_directories("${Boost_INCLUDE_DIRS}")
  endif(Boost_FOUND)
  
endif (USE_UTILITY)

if (USE_MQTT)
  get_sources("MQTT")

  # Adds mosquitto as dependency

  add_subdirectory (${PROJECT_SOURCE_DIR}/MQTT/mosquitto)
  include_directories("${PROJECT_SOURCE_DIR}/MQTT/org.eclipse.mosquitto/lib/cpp")
  include_directories("{PROJECT_SOURCE_DIR}/MQTT/org.eclipse.mosquitto/lib/")
    
  list (APPEND LIBRARIES libmosquitto mosquittopp)

endif (USE_MQTT)

if (USE_LUA)
  get_sources("Auton")
endif (USE_LUA)

# message("${INC_DIRS}")
include_directories ("${INC_DIRS}")

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()

# Library target
string (REPLACE " " ";" SOURCES_LIST "${SOURCES}")
add_library(dalek-manager STATIC ${SOURCES_LIST})

# Link Libraries
message ("LIBRARIES: ${LIBRARIES}")
target_link_libraries(dalek-manager ${LIBRARIES})

# Linter
include(cmake/CppLint.cmake)
add_style_check_target(dalek-manager "${SOURCES} ${HEADERS}")

# Tester
include (cmake/CppTest.cmake)
file (GLOB TEST_SOURCE_FILES "${CMAKE_SOURCE_DIR}/tests/*.cpp")
foreach (SOURCE_FILE ${TEST_SOURCE_FILES})
  create_test_target(${SOURCE_FILE})
endforeach()
