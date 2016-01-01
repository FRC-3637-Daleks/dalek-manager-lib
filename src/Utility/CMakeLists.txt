# Log sub-directory cmake config

get_sources(local_src local_headers local_includes)
set (SOURCES ${SOURCES} ${local_src} PARENT_SCOPE)
set (HEADERS ${HEADERS} ${local_headers} PARENT_SCOPE)
set (INCLUDES ${INCLUDES} ${local_includes} PARENT_SCOPE)

find_package(Boost REQUIRED)

if(Boost_FOUND)
  set (local_inc ${INC_DIRS})
  list (APPEND local_inc ${Boost_INCLUDE_DIRS})
  set (INC_DIRS ${local_inc} PARENT_SCOPE)
   set (local_libs ${LIBRARIES})
   list (APPEND local_libs ${Boost_LIBRARIES})
   set (LIBRARIES ${local_libs} PARENT_SCOPE)
   
   # Set for dynamic linking
   set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_ALL_DYN_LINK" PARENT_SCOPE)
endif(Boost_FOUND)