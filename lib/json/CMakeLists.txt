if(${BACSY_BUILD_TYPE} STREQUAL "SHARED")
	# This is required to compile the static json library
	# into the shared bacsy library.
	add_definitions(-fPIC)
endif()

add_library(json STATIC json_reader json_value json_writer)
install(FILES       autolink.h
                    config.h
                    features.h
                    forwards.h
                    json_batchallocator.h
                    json.h
                    reader.h
                    value.h
                    writer.h
        DESTINATION include/bacsy/json)
