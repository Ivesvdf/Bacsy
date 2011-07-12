# Written by Ives van der Flaas <ives.vdf@gmail.com>, 2011
#
# License: Public domain.
#
# Defines
#
#  RSYNC_INCLUDE_DIRS
#     which contains the include directory for librsync.h
#
#  RSYNC_LIBRARIES
#     which contains the library directory for librsync.a

set( SEARCHPATH /usr /usr/local /opt /opt/local $ENV{HOME}/opt
	${CMAKE_SOURCE_DIR}/external/)
#			
find_path( RSYNC_INCLUDE_DIRS librsync.h
				PATHS ${SEARCHPATH} 	
				PATH_SUFFIXES include 
)		



find_library( RSYNC_LIBRARIES rsync
				PATHS ${SEARCHPATH} 	
				PATH_SUFFIXES lib 
)

unset( SEARCHPATH )

include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS(rsync DEFAULT_MSG RSYNC_LIBRARIES RSYNC_INCLUDE_DIRS)
