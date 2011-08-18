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


add_library(common STATIC configurationFile stringUtils
	bacsyConnection bacsyServer target timerStringParser 
	timeSchedule backupEngine cascadingFileConfiguration
	jsonStoreIndex datagramHelper multicastResponder
	storeManager store jsonHelper streamUtilities
	concurrentMap toSignatureStream simpleBufferStream
	rsyncStream nullStream loadSignatureStream 
	performDeltaStream excludeStringParser exclusionRule
	exclusionSubRule pathExclusionSubRule 
	fileGlobExclusionSubRule pathGlobExclusionSubRule)

add_executable(bacsy main.cpp)
target_link_libraries(bacsy 
	${RSYNC_LIBRARIES} common ${POCO_LIBRARIES} json ${PLATFORMDEPENDENTLIBS})
