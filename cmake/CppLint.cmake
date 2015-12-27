# Copyright (C) 2013 Daniel Scharrer
#
# This software is provided 'as-is', without any express or implied
# warranty.  In no event will the author(s) be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.


# Copyright (C) 2014 Greg Horn
# In order to comply with the above copyright, I am noting that I took
# Daniel's script and hacked it a bit, mostly changing paths and filters

find_package(PythonInterp)

set(STYLE_FILTER)

# disable unwanted filters
 set(STYLE_FILTER ${STYLE_FILTER}-whitespace/blank_line,)
 set(STYLE_FILTER ${STYLE_FILTER}-whitespace/tab,)
 set(STYLE_FILTER ${STYLE_FILTER}-whitespace/indent,)
 set(STYLE_FILTER ${STYLE_FILTER}-whitespace/braces,)
 set(STYLE_FILTER ${STYLE_FILTER}-whitespace/operators,)

 set(STYLE_FILTER ${STYLE_FILTER}-build/namespaces,)

 set(STYLE_FILTER ${STYLE_FILTER}-readability/streams,)
 
 set(STYLE_FILTER ${STYLE_FILTER}-build/c++11,)
 
 set(STYLE_FILTER ${STYLE_FILTER}-build/include_what_you_use)
 

# Add a target that runs cpplint.py
#
# Parameters:
# - TARGET_NAME the name of the target to add
# - SOURCES_LIST a complete list of source and include files to check
function(add_style_check_target TARGET_NAME SOURCES_LIST0) # PROJECT)

  if(NOT PYTHONINTERP_FOUND)
    return()
  endif()

  list(REMOVE_DUPLICATES SOURCES_LIST0)
  list(SORT SOURCES_LIST0)

  # filtering out unwanted files
#  set(SOURCES_LIST)
#  foreach(item ${SOURCES_LIST0})
#    #string(REGEX MATCH ".*\\.hpp$" dummy ${item})
#    string(REGEX MATCH "runtime_embedded\\.hpp" dummy ${item})
#    if(NOT dummy)
#      string(REGEX MATCH ".*meta\\.cpp" dummy ${item})
#      if(NOT dummy)
#        list(APPEND SOURCES_LIST ${item})
#      endif()
#    endif()
#  endforeach()
  set(SOURCES_LIST ${SOURCES_LIST0}) # just use all files

  string (REPLACE " " ";" SOURCES_LIST "${SOURCES_LIST}")

  add_custom_target(lint_${TARGET_NAME} COMMENT "Linting ${TARGET_NAME}")
  
  foreach(item ${SOURCES_LIST})
    if(item)
      string(REGEX MATCH "[^/]+/[^./]+\\.(cpp|hpp|h)" item_target_name ${item})
      string(REPLACE "/" "_" item_target_name "${item_target_name}")
      
      # Executes command into build cache
      add_custom_command(OUTPUT "${PROJECT_BINARY_DIR}/lints/${item_target_name}.lint"
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/lints
        COMMAND "${CMAKE_COMMAND}" -E chdir
                "${CMAKE_CURRENT_SOURCE_DIR}"
                "${PYTHON_EXECUTABLE}"
                "${CMAKE_SOURCE_DIR}/cmake/cpplint.py"
                "--filter=${STYLE_FILTER}"
                "--counting=detailed"
                "--extensions=cpp,hpp,h"
                "--linelength=80"
                ${item}
                > ${PROJECT_BINARY_DIR}/lints/${item_target_name}.lint
	   DEPENDS "${item}"
	   COMMENT "Linting ${item}"
	   VERBATIM)
	   
		add_custom_target(lint_${item_target_name}
	                     DEPENDS ${PROJECT_BINARY_DIR}/lints/${item_target_name}.lint)
	                     
		add_dependencies(lint_${TARGET_NAME} lint_${item_target_name})
	endif(item)
  endforeach()
    
  add_dependencies (${TARGET_NAME} lint_${TARGET_NAME})

endfunction()
