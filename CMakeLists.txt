cmake_minimum_required(VERSION 2.6)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
set(CMAKE_COLOR_MAKEFILE	ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
add_definitions(-Wall)

if(CMAKE_BUILD_TYPE MATCHES Profile)
	message("Building with profiling information")
endif()

set(BACSY_BUILD_TYPE STATIC
                     CACHE STRING "The build type for bacsy libraries. Default STATIC.
                                   To build shared libraries set this value to SHARED.")
set(CMAKE_CXX_FLAGS_PROFILE "-pg -g")
set(CMAKE_C_FLAGS_PROFILE "-pg -g")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")

project(bacsy)
set(BACSY_VERSION_MAJOR 0)
set(BACSY_VERSION_MINOR 1)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A powerful backup system capable of operating on a distributed P2P basis.")
set(CPACK_PACKAGE_VENDOR "Bacsy")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR ${BACSY_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${BACSY_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH 0)

include(CMakePackageRPM.cmake)
include(CMakePackageDEB.cmake)
include(CPack)


include_directories(lib/)
include_directories(src/main)

add_subdirectory(lib/gtest)
add_subdirectory(lib/json)

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
include_directories(src/)

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

add_subdirectory(src)
add_subdirectory(test)
