add_executable(test
                   Main
                   ConfigurationTests
                   StringUtilsTests
                   TimerStringParserTests
                   StoreIndexTest
                   ConcurrentMapTest
                   RsyncStreamTests
                   ExclusionStringParserTests
                   StringExclusionRuleBuilderTests
)

target_link_libraries(test gtest ${bacsy_libraries})
