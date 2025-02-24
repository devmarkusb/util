# Include this file and just add e.g.
#   ul_find_boost("1" "63" "0" "") # or
#   ul_find_boost("1" "63" "0" "system filesystem") # or similar
# Then include ${Boost_INCLUDE_DIRS} and link ${Boost_LIBRARIES} if necessary.
# *Important*
#	If you use uiwrap you should define
#		set(UL_USE_BOOST_ver1 "1" CACHE STRING "Use boost version ver1.ver2.ver3")
#		set(UL_USE_BOOST_ver2 "64" CACHE STRING "Cf. UL_USE_BOOST_ver1")
#		set(UL_USE_BOOST_ver3 "0" CACHE STRING "Cf. UL_USE_BOOST_ver1")
#	and write
#		ul_find_boost(${UL_USE_BOOST_ver1} ${UL_USE_BOOST_ver2} ${UL_USE_BOOST_ver3} "")
#	or similar, in order to make the library use the same boost version as your project.

set(UL_SDK_PATH_HELPSTR "string has to match the Qt install location with version subdirs")
set(UL_SDK_PATH "" CACHE STRING "${UL_SDK_PATH_HELPSTR}")
set(UL_USE_BOOST_ver1 "1" CACHE STRING "Use boost version ver1.ver2.ver3")
set(UL_USE_BOOST_ver2 "67" CACHE STRING "Cf. UL_USE_BOOST_ver1")
set(UL_USE_BOOST_ver3 "0" CACHE STRING "Cf. UL_USE_BOOST_ver1")

if (UL_ANDROID)
    include(${CMAKE_CURRENT_LIST_DIR}/detail/find_host_package.cmake)
endif ()

# Impl. notes:
#	macro instead of function, since this makes it trivial to bring
#	find_package's results to parent scope
macro(ul_find_boost ver1_ ver2_ ver3_ libs)
    string(REPLACE " " ";" ver1 ${ver1_})
    string(REPLACE " " ";" ver2 ${ver2_})
    string(REPLACE " " ";" ver3 ${ver3_})
    find_package(Boost ${ver1_}.${ver2_}.${ver3_} COMPONENTS container QUIET)
    if (Boost_FOUND)
        add_library(libboost INTERFACE)
        target_link_libraries(libboost INTERFACE ${Boost_LIBRARIES})
        return()
    endif()
    message(STATUS "boost not found, try alternative")
    if (NOT DEFINED ENV{dev_sdk_path} AND UL_SDK_PATH STREQUAL "")
        message(FATAL_ERROR "Configure cache variable UL_SDK_PATH or env variable dev_sdk_path to point to where boost \
subdirs like boost_1.63.0 are located.")
    endif()
    if (UL_SDK_PATH STREQUAL "" AND DEFINED ENV{dev_sdk_path})
        set(UL_SDK_PATH $ENV{dev_sdk_path} CACHE STRING "${UL_SDK_PATH_HELPSTR}" FORCE)
    endif()
    file(TO_CMAKE_PATH "${UL_SDK_PATH}" ul_sdk_path)
    set(BOOST_ROOT ${ul_sdk_path}/boost_${ver1}_${ver2}_${ver3})
    set(Boost_USE_MULTITHREADED     ON)
    set(Boost_USE_STATIC_LIBS       OFF)
    set(Boost_USE_STATIC_RUNTIME    OFF)
    # BOOST_FILESYSTEM_NO_DEPRECATED added because 'generic' was used as identifier,
    # 	which clashes with C++/CX
    add_definitions(-DBOOST_ALL_NO_LIB -DBOOST_FILESYSTEM_NO_DEPRECATED)
    string(COMPARE EQUAL "${libs}" "" libs_empty)
    if (libs_empty)
        if (UL_ANDROID)
            find_host_package(Boost ${ver1}.${ver2}.${ver3})
        else()
            find_package(Boost ${ver1}.${ver2}.${ver3})
        endif ()
    else ()
    	string(REPLACE " " ";" libs_list "${libs}")
    	find_package(Boost ${ver1}.${ver2}.${ver3} COMPONENTS ${libs_list})
    endif ()
endmacro()
