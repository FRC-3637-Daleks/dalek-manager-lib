# Config sub-directory cmake config

get_sources(local_src)
set (SOURCES ${SOURCES} ${local_src} PARENT_SCOPE)

# Adds mosquitto as dependency

add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/org.eclipse.mosquitto)
set (LIBRARIES ${LIBRARIES} libmosquitto libmosquittopp)

# Using the above version since it will rebuild the library when cross-compiling
# find_package(Mosquittopp REQUIRED)
# if(MOSQUITTOPP_FOUND)
#	set(LIBRARIES ${LIBRARIES} ${MOSQUITTOPP_LIBRARIES})
#	include_directories(${MOSQUITTOPP_INCLUDE_DIR})
# endif(MOSQUITTOPP_FOUND)

