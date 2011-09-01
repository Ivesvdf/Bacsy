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
                 common/ConfigurationFile
                 common/StringUtils
                 server/BacsyConnection
                 server/BacsyServer
                 client/Source
                 client/TimerStringParser
                 client/TimeSchedule
                 client/BackupEngine
                 common/CascadingFileConfiguration
				 client/CascadingSourceConfiguration
				 server/CascadingStoreConfiguration
                 server/JsonStoreIndex
                 common/DatagramHelper
                 server/MulticastResponder
                 server/StoreManager
                 server/Store
                 common/JsonHelper
                 streams/StreamUtils
                 common/ConcurrentMap
                 streams/ToSignatureStream
                 streams/SimpleBufferStream
                 streams/RsyncStream
                 streams/NullStream
                 streams/LoadSignatureStream
                 streams/PerformDeltaStream
                 rules/ExclusionStringParser
                 rules/ExclusionRule
                 rules/ExclusionSubRule
                 rules/PathExclusionSubRule
                 rules/FileGlobExclusionSubRule
                 rules/PathGlobExclusionSubRule
                 rules/SizeExclusionSubRule
                 rules/StringExclusionRuleBuilder
				 common/ArgParsingUtils
				 common/Environment
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
