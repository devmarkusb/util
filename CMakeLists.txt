cmake_minimum_required(VERSION 3.23)

project(mb.util VERSION 0.1.0)

include(buildenv-bootstrap.cmake)

set(UL_BUILD_UNITTESTS ${PROJECT_IS_TOP_LEVEL} CACHE BOOL "build unit tests")
option(
    UL_DISABLE_NAMESPACE_ALIAS
    "Set to ON if you want the base namespace to be mb::ul instead of ul"
    OFF
)

configure_file(
    ${PROJECT_SOURCE_DIR}/include/ul/buildenv/config-gen.h.in
    ${PROJECT_SOURCE_DIR}/include/ul/buildenv/config-gen.h
    @ONLY
)

ul_include(defaults.cmake)

if(UL_BUILD_UNITTESTS)
    ul_include(googletest.cmake)
endif()

add_subdirectory(src)

set(UL_USE_DEFAULT_CLANG_TIDY_CONFIG ON)
set(UL_CLANG_TIDY_DIRS "src;mb.alloc-statistics")
ul_include(clang-tidy.cmake)

set(dirs_to_doc README.md src mb.alloc-statistics)
ul_include(doxygen.cmake)

set(MB_UTIL_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL
    "util include directories"
    FORCE
)

set(ul_CMAKE_DIRS
    ${PROJECT_SOURCE_DIR}/cmake
    CACHE INTERNAL
    "util cmake util directories"
    FORCE
)
