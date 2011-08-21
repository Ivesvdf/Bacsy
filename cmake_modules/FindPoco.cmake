set(SEARCHPATH ${CMAKE_SOURCE_DIR}/external/ /usr /usr/local /opt /opt/local $ENV{HOME}/opt)
#			
find_path(POCO_INCLUDE_DIRS    Poco/
              PATHS            ${SEARCHPATH}
              PATH_SUFFIXES    include
)

set(POCO_COMPONENTS
        PocoNet
        PocoUtil
        PocoFoundation
)

set(POCO_LIBRARIES "")

foreach(COMPONENT ${POCO_COMPONENTS})
	find_library(${COMPONENT}_LIB     ${COMPONENT}
	                 PATHS            ${SEARCHPATH}
	                 PATH_SUFFIXES    lib
	)

	list(APPEND POCO_LIBRARIES ${${COMPONENT}_LIB})
	list(REMOVE_DUPLICATES POCO_LIBRARIES)
endforeach()

unset(SEARCHPATH)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Poco DEFAULT_MSG
                                       POCO_LIBRARIES
                                       POCO_INCLUDE_DIRS)
