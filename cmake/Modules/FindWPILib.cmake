# Finds wpilib
if(WPILIB_HOME)
  message("WPILIB_HOME: ${WPILIB_HOME}")
else()
  set(WPILIB_HOME $ENV{WPILIB})
  if(WPILIB_HOME)
	  set(WPILIB_HOME ${WPILIB_HOME}/cpp/current)
      message("WPILIB_HOME: ${WPILIB_HOME}")
  else()
	message("ERROR: Environment variable $WPILIB was not set")
  endif()
endif()


# Common attributes
set(WPILib_SUFFIXES wpilib WPILib include lib)
set(WPILib_NAMES wpilib libwpilib wpi libwpi)

#Find Includes
set(WPILib_INCLUDE_DIR ${WPILIB_HOME}/include)
#find_path(WPILib_INCLUDE_DIR "WPILib.h" "${WPILIB_HOME}/include/")

# Library
set(WPILib_LIBRARY ${WPILIB_HOME}/lib/libwpi.so)
#find_library(WPILib_LIBRARY libwpi.so ${WPILIB_HOME}/lib)

# Handle Args
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(WPILib DEFAULT_MSG WPILib_LIBRARY WPILib_INCLUDE_DIR)

if (WPILib_FOUND)
	set(WPILib_LIBRARIES ${WPILib_LIBRARY})
else (WPILib_FOUND)
	set( WPILib_LIBRARIES )
endif (WPILib_FOUND)

mark_as_advanced(
	WPILib_LIBRARY
	WPILib_INCLUDE_DIR
)
