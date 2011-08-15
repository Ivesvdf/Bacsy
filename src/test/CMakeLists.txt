add_executable(test main configurationTests stringUtilsTests
	timerStringParserTests storeIndexTest concurrentMapTest
	rsyncStreamTests)
target_link_libraries(test common ${POCO_LIBRARIES} gtest 
	${RSYNC_LIBRARIES} json ${PLATFORMDEPENDENTLIBS})
