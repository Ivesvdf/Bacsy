find_package(Poco REQUIRED COMPONENTS Net Util)
find_package(rsync REQUIRED)

if(WIN32)
	set(PLATFORM_DEPENDENT_LIBS wsock32 iphlpapi)
elseif(UNIX)
	set(PLATFORM_DEPENDENT_LIBS pthread)
else()
	set(PLATFORM_DEPENDENT_LIBS "")
endif()

include_directories(${POCO_INCLUDE_DIRS})
include_directories(${RSYNC_INCLUDE_DIRS})

set(bacsy_libraries bacsy
                    ${RSYNC_LIBRARIES}
                    ${POCO_LIBRARIES})

if("${BACSY_BUILD_TYPE}" STREQUAL "STATIC")
	list(APPEND bacsy_libraries
                    json
                    ${PLATFORM_DEPENDENT_LIBS})
else()
	list(APPEND bacsy_libraries ${PLATFORM_DEPENDENT_LIBS})
endif()

add_subdirectory(test)
add_subdirectory(main)
