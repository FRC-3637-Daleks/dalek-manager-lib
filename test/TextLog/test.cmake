add_executable (TextLogTest EXCLUDE_FROM_ALL
				test/TextLog/test.cpp)
target_link_libraries (TextLogTest dalek-manager)