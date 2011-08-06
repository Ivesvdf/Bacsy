find_package( Poco REQUIRED )
find_package( rsync REQUIRED )

if(WIN32)
set(PLATFORMDEPENDENTLIBS wsock32 iphlpapi)
elseif(UNIX)
set(PLATFORMDEPENDENTLIBS pthread)
else(WIN32)
set(PLATFORMDEPENDENTLIBS "")
endif(WIN32)

include_directories( ${POCO_INCLUDE_DIRS} )
include_directories( ${RSYNC_INCLUDE_DIRS} )

add_subdirectory(test)
add_subdirectory(main)

