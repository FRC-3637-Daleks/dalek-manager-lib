add_executable (LogAttributesTest EXCLUDE_FROM_ALL
				test/LogAttributes/test.cpp)
target_link_libraries (LogAttributesTest dalek-manager)