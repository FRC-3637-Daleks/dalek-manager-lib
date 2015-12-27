# Copyright (C) EdWard <ezeward4@gmail.com>
# Not really

function (create_test_target SOURCE_FILE)
	string (REGEX MATCH "[^/]+\\.(cpp|cc|cxx)" 
		STRIPPED_FILE ${SOURCE_FILE})
	string (REPLACE ".cpp" "" STRIPPED_FILE ${STRIPPED_FILE})
	set (TARGET_NAME "${STRIPPED_FILE}")
	
	add_executable(${TARGET_NAME} EXCLUDE_FROM_ALL ${SOURCE_FILE})
	target_link_libraries(${TARGET_NAME} dalek-manager)
endfunction ()