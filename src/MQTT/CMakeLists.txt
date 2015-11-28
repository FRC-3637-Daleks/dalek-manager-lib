# Config sub-directory cmake config

get_sources(local_src local_headers local_includes)
set (SOURCES ${SOURCES} ${local_src} PARENT_SCOPE)
set (HEADERS ${HEADERS} ${local_headers} PARENT_SCOPE)
set (INCLUDES ${INCLUDES} ${local_includes} PARENT_SCOPE)

# Adds mosquitto as dependency

add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/org.eclipse.mosquitto)
set (local_libs ${LIBRARIES})
list (APPEND local_libs libmosquitto libmosquittopp)
set (LIBRARIES ${local_libs} PARENT_SCOPE)

# Using the above version since it will rebuild the library when cross-compiling
# find_package(Mosquittopp REQUIRED)
# if(MOSQUITTOPP_FOUND)
#	set(LIBRARIES ${LIBRARIES} ${MOSQUITTOPP_LIBRARIES})
#	include_directories(${MOSQUITTOPP_INCLUDE_DIR})
# endif(MOSQUITTOPP_FOUND)

