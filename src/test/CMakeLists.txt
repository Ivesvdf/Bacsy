add_executable(test main.cpp configurationTests.cpp stringUtilsTests.cpp
	timerStringParserTests.cpp)
target_link_libraries(test gtest ${POCO_LIBRARIES} ${RSYNC_LIBRARIES} common)
