#find_file(BUILD_NUMBER_FILE_FOUND build_number.txt PATHS .) 

#if(${BUILD_NUMBER_FILE_FOUND} MATCHES "NOTFOUND") 
#	set(CURRENT_BUILD_NUM 1) 
#	file(WRITE build_number.txt "${CURRENT_BUILD_NUM}") 
#else() 
#	file(READ build_number.txt OLD_BUILD_NUM) 
#	math(EXPR CURRENT_BUILD_NUM "${OLD_BUILD_NUM} + 1") 
#	file(WRITE build_number.txt "${CURRENT_BUILD_NUM}") 
#endif() 

configure_file(Common/Info.h.in Common/Info.h @ONLY) 
include_directories("${PROJECT_BINARY_DIR}")

set(BACSY_COMMON_SOURCE_FILES
                 Client/BackupEngine
                 Client/CascadingSourceConfiguration
                 Client/ISourceConfiguration
                 Client/Source
				 Client/StoreMessage
                 Client/TimeSchedule
                 Client/TimerStringParser
				 Common/AMessage
                 Common/ArgParsingUtils
                 Common/CascadingFileConfiguration
                 Common/ConcurrentMap
                 Common/ConfigurationFile
                 Common/DatagramHelper
				 Common/DummyFile
                 Common/Environment
				 Common/IFile
                 Common/JsonHelper
				 Common/PocoFile
                 Common/StringUtils
                 Rules/ExclusionRule
                 Rules/ExclusionStringParser
                 Rules/ExclusionSubRule
                 Rules/FileGlobExclusionSubRule
                 Rules/PathExclusionSubRule
                 Rules/PathGlobExclusionSubRule
                 Rules/SizeExclusionSubRule
                 Rules/StringExclusionRuleBuilder
                 Server/BacsyConnection
                 Server/BacsyServer
                 Server/CascadingStoreConfiguration
                 Server/JsonStoreIndex
                 Server/MulticastResponder
                 Server/Store
                 Server/StoreManager
                 Streams/LoadSignatureStream
                 Streams/NullStream
                 Streams/PerformDeltaStream
                 Streams/RsyncStream
                 Streams/SimpleBufferStream
                 Streams/StreamUtils
                 Streams/ToSignatureStream
)

add_library(bacsy ${BACSY_BUILD_TYPE} ${BACSY_COMMON_SOURCE_FILES})

# add_executable(bacsyd bacsyd/Main.cpp)
add_executable(bacsy-server Server/BacsyServerMain.cpp)
add_executable(bacsy-client Client/BacsyClientMain.cpp)
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
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DESTINATION include/bacsy
        FILES_MATCHING PATTERN "*.h")
install(FILES       ${CMAKE_CURRENT_BINARY_DIR}/Common/Info.h
        DESTINATION include/bacsy/Bacsy/Common)
