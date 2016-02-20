# Finds wpilib
if(WPILIB_HOME)
  message("WPILIB_HOME: ${WPILIB_HOME}")
else()
  set(WPILIB_HOME $ENV{WPILIB})
  if(WPILIB_HOME)
      message("WPILIB_HOME: ${WPILIB_HOME}")
  else()
	message("ERROR: Environment variable $WPILIB was not set")
  endif()
endif()
# CMake Find Module for WPILib by Matt Coffin



# Common attributes
set(WPILib_SUFFIXES wpilib WPILib include lib)
set(WPILib_NAMES wpilib libwpilib wpi libwpi)

# Find includes
find_path(WPILib_INCLUDE_DIR
	NAMES WPILib.h
	PATHS ${WPILIB_HOME}/include ${WPILIB_HOME} ENV WPILIB
	PATH_SUFFIXES ${WPILib_SUFFIXES}
)

# Library
find_library(WPILib_LIBRARY
	NAMES ${WPILib_NAMES}
	PATHS ${WPILIB_HOME}/lib ${WPILIB_HOME} ENV WPILIB
	PATH_SUFFIXES ${WPILib_SUFFIXES}
)

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