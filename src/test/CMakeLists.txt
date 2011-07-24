add_executable(test main configurationTests stringUtilsTests
	timerStringParserTests storeIndexTest)
target_link_libraries(test gtest ${POCO_LIBRARIES}
	${RSYNC_LIBRARIES} common json)
