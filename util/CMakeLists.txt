cmake_minimum_required(VERSION 3.23)

include(${ulBuildEnv_CMAKE_DIRS}/cmake_util.cmake)

ul_include(defaults.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../devenv/cmake/install-library-config.cmake)

file(GLOB_RECURSE target_INCS "include/*.h")

add_library(mb.util INTERFACE)
add_library(mb::util ALIAS mb.util)
add_library(mb-util ALIAS mb.util)

target_sources(
    mb.util
    INTERFACE
        FILE_SET HEADERS
            BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
            FILES ${target_INCS}
)

set(MB_UTIL_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    CACHE INTERNAL
    "mb.util: include directories"
    FORCE
)

target_include_directories(
    mb.util
    INTERFACE
        ${MB_UTIL_INCLUDE_DIRS}
        ${ulBasicCodeSupport_INCLUDE_DIRS}
        ${ulBuildEnv_INCLUDE_DIRS}
        ${ulLinklib_INCLUDE_DIRS}
        ${ulMem_INCLUDE_DIRS}
        ${ulString_INCLUDE_DIRS}
)

target_link_libraries(
    mb.util
    INTERFACE ulBuildEnv ulBasicCodeSupport ulString
)

ul_set_target_defaults(mb.util)

set(compile_check_target mb.util-compile-check)
add_executable(${compile_check_target} compile-check.cpp)
target_link_libraries(${compile_check_target} PRIVATE mb.util)
ul_set_target_defaults(${compile_check_target})

#mb_install_library(mb.util)
