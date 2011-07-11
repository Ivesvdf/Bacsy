add_executable(test main.cpp configurationTests.cpp stringUtilsTests.cpp )
target_link_libraries(test gtest common)
