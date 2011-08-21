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

#			
find_path(RSYNC_INCLUDE_DIRS    librsync.h
              PATH_SUFFIXES     include
)		



find_library(RSYNC_LIBRARIES      rsync
                 PATH_SUFFIXES    lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(rsync DEFAULT_MSG
                                        RSYNC_LIBRARIES
                                        RSYNC_INCLUDE_DIRS)
