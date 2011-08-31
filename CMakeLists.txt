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
add_subdirectory(src)

install(FILES lib/woodcutter/woodcutter.h
        DESTINATION include/bacsy/woodcutter)
