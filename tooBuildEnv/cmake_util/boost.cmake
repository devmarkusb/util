# Include this file and just add e.g.
#   too_find_boost(1 63 0 "") # or
#   too_find_boost(1 63 0 "system filesystem") # or similar
# Then include ${Boost_INCLUDE_DIRS} and link ${Boost_LIBRARIES} if necessary.

# Impl. notes:
#	macro instead of function, since this makes it trivial to bring
#	find_package's results to parent scope
macro(too_find_boost ver1 ver2 ver3 libs)
	set(BOOST_ROOT $ENV{dev_sdk_path}/boost_${ver1}_${ver2}_${ver3})
	set(Boost_USE_MULTITHREADED     ON)
	set(Boost_USE_STATIC_LIBS       OFF)
	set(Boost_USE_STATIC_RUNTIME    OFF)
	# BOOST_FILESYSTEM_NO_DEPRECATED added because 'generic' was used as identifier,
	# 	which clashes with C++/CX
	add_definitions(-DBOOST_ALL_NO_LIB -DBOOST_FILESYSTEM_NO_DEPRECATED)
	string(COMPARE EQUAL "${libs}" "" libs_empty)
	if (libs_empty)
		find_package(Boost ${ver1}.${ver2}.${ver3})
	else ()
		string(REPLACE " " ";" libs_list "${libs}")
		find_package(Boost ${ver1}.${ver2}.${ver3} COMPONENTS ${libs_list})
	endif ()
endmacro()
