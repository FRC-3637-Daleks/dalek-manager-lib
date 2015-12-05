# Log sub-directory cmake config

get_sources(local_src local_headers local_includes)
set (SOURCES ${SOURCES} ${local_src} PARENT_SCOPE)
set (HEADERS ${HEADERS} ${local_headers} PARENT_SCOPE)
set (INCLUDES ${INCLUDES} ${local_includes} PARENT_SCOPE)

unset (Boost_INCLUDE_DIR)
unset (Boost_LIBRARY_DIR)
unset (Boost_CXX_LIBRARY_DEBUG)
unset (Boost_CXX_LIBRARY_RELEASE)
unset (Boost_C_LIBRARY_DEBUG)
unset (Boost_C_LIBRARY_RELEASE)
unset (Boost_FOUND)
unset (Boost_INCLUDE_DIRS)
unset (Boost_LIBRARY_DIRS)
unset (Boost_LIBRARIES)
unset (BOOST_ROOT)
unset (BOOST_INCLUDE_DIR)
unset (BOOST_LIBRARYDIR)
unset (Boost_DIR)

#message("CMAKE_FIND_ROOT_PATH: ${CMAKE_FIND_ROOT_PATH}")

find_package(Boost REQUIRED log)

if(Boost_FOUND)
  set (local_inc ${INC_DIRS})
  list (APPEND local_inc ${Boost_INCLUDE_DIRS})
  set (INC_DIRS ${local_inc} PARENT_SCOPE)
   set (local_libs ${LIBRARIES})
   list (APPEND local_libs ${Boost_LIBRARIES})
   set (LIBRARIES ${local_libs} PARENT_SCOPE)
endif(Boost_FOUND)