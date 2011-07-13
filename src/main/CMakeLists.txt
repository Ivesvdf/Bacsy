add_library(common STATIC configurationFile.cpp stringUtils.cpp
	bacsyConnection bacsyServer)
add_executable(bacsy main.cpp)
target_link_libraries(bacsy gtest ${POCO_LIBRARIES} ${RSYNC_LIBRARIES} common)
