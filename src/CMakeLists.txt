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

set(bacsy_libraries common
                    ${RSYNC_LIBRARIES}
                    ${POCO_LIBRARIES}
                    json
                    ${PLATFORM_DEPENDENT_LIBS})

add_subdirectory(test)
add_subdirectory(main)
