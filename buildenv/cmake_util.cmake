# Easier access of various cmake utility files of common use. You just need to pass the file name of the utility
# desired from cmake_util subdir to the macro too_include.

set(TOO_CMAKE_UTIL_DIR ${CMAKE_CURRENT_LIST_DIR}/cmake_util
    CACHE INTERNAL "TOO_CMAKE_UTIL_DIR: path to various cmake utility include files" FORCE)

macro(too_include cmake_util_filename)
    include(${TOO_CMAKE_UTIL_DIR}/${cmake_util_filename})
endmacro()
