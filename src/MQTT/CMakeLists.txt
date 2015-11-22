# Config sub-directory cmake config

get_sources(local_src)
set (SOURCES ${SOURCES} ${local_src} PARENT_SCOPE)

# Adds mosquitto as dependency
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/org.eclipse.mosquitto)
set(LIBRARIES ${LIBRARIES} libmosquitto libmosquittopp)
