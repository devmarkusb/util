# Easier access of various cmake utility files of common use. You just need to pass the file name of the utility
# desired from cmake_util subdir to the macro ul_include instead of include.
# We could also have added the cmake_util dir to CMAKE_MODULE_PATH but for now decided not to (file name clashes for
# lib users?).

set(UL_CMAKE_UTIL_DIR ${CMAKE_CURRENT_LIST_DIR}/cmake_util
    CACHE INTERNAL "UL_CMAKE_UTIL_DIR: path to various cmake utility include files" FORCE)

macro(ul_include cmake_util_filename)
    include(${UL_CMAKE_UTIL_DIR}/${cmake_util_filename})
endmacro()
