find_package( Poco REQUIRED )
find_package( rsync REQUIRED )

include_directories( ${POCO_INCLUDE_DIRS} )
include_directories( ${RSYNC_INCLUDE_DIRS} )

add_subdirectory(test)
add_subdirectory(main)

