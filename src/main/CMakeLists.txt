#find_file(BUILD_NUMBER_FILE_FOUND build_number.txt PATHS .) 

set (VERSION_MAJOR 1)
set (VERSION_MINOR 1)

#if(${BUILD_NUMBER_FILE_FOUND} MATCHES "NOTFOUND") 
#	set(CURRENT_BUILD_NUM 1) 
#	file(WRITE build_number.txt "${CURRENT_BUILD_NUM}") 
#else() 
#	file(READ build_number.txt OLD_BUILD_NUM) 
#	math(EXPR CURRENT_BUILD_NUM "${OLD_BUILD_NUM} + 1") 
#	file(WRITE build_number.txt "${CURRENT_BUILD_NUM}") 
#endif() 

configure_file(info.h.in info.h @ONLY) 
include_directories("${PROJECT_BINARY_DIR}/src/main")


add_library(common STATIC
                 ConfigurationFile
                 StringUtils
                 BacsyConnection
                 BacsyServer
                 Source
                 TimerStringParser
                 TimeSchedule
                 BackupEngine
                 CascadingFileConfiguration
                 JsonStoreIndex
                 DatagramHelper
                 MulticastResponder
                 StoreManager
                 Store
                 JsonHelper
                 StreamUtilities
                 ConcurrentMap
                 ToSignatureStream
                 SimpleBufferStream
                 RsyncStream
                 NullStream
                 LoadSignatureStream
                 PerformDeltaStream
                 ExclusionStringParser
                 ExclusionRule
                 ExclusionSubRule
                 PathExclusionSubRule
                 FileGlobExclusionSubRule
                 PathGlobExclusionSubRule
                 SizeExclusionSubRule
                 StringExclusionRuleBuilder
)

add_executable(bacsy Main.cpp)
add_executable(bacsy-server BacsyServerMain.cpp)
add_executable(bacsy-client BacsyClientMain.cpp)
target_link_libraries(bacsy ${bacsy_libraries})
target_link_libraries(bacsy-server ${bacsy_libraries})
target_link_libraries(bacsy-client ${bacsy_libraries})
