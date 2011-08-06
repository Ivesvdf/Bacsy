if(MINGW AND WIN32)
	add_definitions( -DGTEST_OS_WINDOWS_MINGW )
endif(MINGW AND WIN32)

add_library( gtest STATIC gtest-all.cc )


