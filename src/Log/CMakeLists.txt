# Log sub-directory cmake config

get_sources(local_src local_headers local_includes)
set (SOURCES ${SOURCES} ${local_src} PARENT_SCOPE)
set (HEADERS ${HEADERS} ${local_headers} PARENT_SCOPE)
set (INCLUDES ${INCLUDES} ${local_includes} PARENT_SCOPE)

find_package(Boost REQUIRED log)

if(Boost_FOUND)
  include_directories (${Boost_INCLUDE_DIRS})
  set (local_libs ${LIBRARIES})
  list (APPEND local_libs ${Boost_LIBRARIES})
  set (LIBRARIES ${local_libs} PARENT_SCOPE)
endif(Boost_FOUND)