add_executable(test
                   main
                   configurationTests
                   stringUtilsTests
                   timerStringParserTests
                   storeIndexTest
                   concurrentMapTest
                   rsyncStreamTests
                   exclusionStringParserTests
                   stringExclusionRuleBuilderTests
)

target_link_libraries(test gtest ${bacsy_libraries})
