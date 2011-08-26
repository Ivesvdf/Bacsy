cmake_minimum_required(VERSION 2.6)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
set(CMAKE_COLOR_MAKEFILE	ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
add_definitions(-Wall)

if(CMAKE_BUILD_TYPE MATCHES Profile)
	message("Building with profiling information")
endif()

set(CMAKE_CXX_FLAGS_PROFILE "-pg -g")
set(CMAKE_C_FLAGS_PROFILE "-pg -g")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")

project(bacsy)

include_directories(lib/)
include_directories(src/main)

add_subdirectory(lib/gtest)
add_subdirectory(lib/json)
add_subdirectory(src)
