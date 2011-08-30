find_path(POCO_INCLUDE_DIRS    Poco/Ascii.h
              PATH_SUFFIXES    include
)

find_library(POCO_FOUNDATION_LIB     PocoFoundation
                 PATH_SUFFIXES       lib
)

foreach(COMPONENT ${Poco_FIND_COMPONENTS})
	find_library(POCO_${COMPONENT}_LIB     "Poco${COMPONENT}"
	                 PATH_SUFFIXES         lib
	)

	list(APPEND POCO_LIBRARIES ${POCO_${COMPONENT}_LIB})
endforeach()

list(APPEND POCO_LIBRARIES ${POCO_FOUNDATION_LIB})
list(REMOVE_DUPLICATES POCO_LIBRARIES)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Poco DEFAULT_MSG
                                       POCO_LIBRARIES
                                       POCO_INCLUDE_DIRS)
