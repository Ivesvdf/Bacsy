#find_file(BUILD_NUMBER_FILE_FOUND build_number.txt PATHS .) 

#if(${BUILD_NUMBER_FILE_FOUND} MATCHES "NOTFOUND") 
#	set(CURRENT_BUILD_NUM 1) 
#	file(WRITE build_number.txt "${CURRENT_BUILD_NUM}") 
#else() 
#	file(READ build_number.txt OLD_BUILD_NUM) 
#	math(EXPR CURRENT_BUILD_NUM "${OLD_BUILD_NUM} + 1") 
#	file(WRITE build_number.txt "${CURRENT_BUILD_NUM}") 
#endif() 

configure_file(common/info.h.in common/info.h @ONLY) 
include_directories("${PROJECT_BINARY_DIR}/src/common")

set(BACSY_COMMON_SOURCE_FILES
                 client/BackupEngine
                 client/CascadingSourceConfiguration
                 client/Source
                 client/TimeSchedule
                 client/TimerStringParser
                 common/ArgParsingUtils
                 common/CascadingFileConfiguration
                 common/ConcurrentMap
                 common/ConfigurationFile
                 common/DatagramHelper
                 common/Environment
                 common/JsonHelper
                 common/StringUtils
                 rules/ExclusionRule
                 rules/ExclusionStringParser
                 rules/ExclusionSubRule
                 rules/FileGlobExclusionSubRule
                 rules/PathExclusionSubRule
                 rules/PathGlobExclusionSubRule
                 rules/SizeExclusionSubRule
                 rules/StringExclusionRuleBuilder
                 server/BacsyConnection
                 server/BacsyServer
                 server/CascadingStoreConfiguration
                 server/JsonStoreIndex
                 server/MulticastResponder
                 server/Store
                 server/StoreManager
                 streams/LoadSignatureStream
                 streams/NullStream
                 streams/PerformDeltaStream
                 streams/RsyncStream
                 streams/SimpleBufferStream
                 streams/StreamUtils
                 streams/ToSignatureStream
)

foreach(source_file ${BACSY_COMMON_SOURCE_FILES})
	list(APPEND BACSY_COMMON_HEADER_FILES "${source_file}.h")
endforeach()

add_library(bacsy ${BACSY_BUILD_TYPE} ${BACSY_COMMON_SOURCE_FILES})

# add_executable(bacsyd bacsyd/Main.cpp)
add_executable(bacsy-server server/BacsyServerMain.cpp)
add_executable(bacsy-client client/BacsyClientMain.cpp)
if(${BACSY_BUILD_TYPE} STREQUAL "SHARED")
	target_link_libraries(bacsy json)
endif()

# target_link_libraries(bacsyd ${bacsy_libraries})
target_link_libraries(bacsy-server ${bacsy_libraries})
target_link_libraries(bacsy-client ${bacsy_libraries})

install(TARGETS bacsy
	# bacsyd
                bacsy-server
                bacsy-client
        ARCHIVE DESTINATION lib/static
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)
install(FILES ${BACSY_COMMON_HEADER_FILES}
        DESTINATION include/bacsy)
