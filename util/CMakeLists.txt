cmake_minimum_required(VERSION 3.23)

include(${ulBuildEnv_CMAKE_DIRS}/cmake_util.cmake)

ul_include(defaults.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../devenv/cmake/install-library-config.cmake)

set(target ${PROJECT_NAME})

file(GLOB_RECURSE target_INCS "include/*.h")

add_library(${target} INTERFACE)
add_library(mb::util ALIAS ${target})
add_library(mb-util ALIAS ${target})

target_sources(
    ${target}
    INTERFACE
        FILE_SET HEADERS
            BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
            FILES ${target_INCS}
)

set(${target}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL
    "${target}: include directories"
    FORCE
)

target_include_directories(
    ${target}
    INTERFACE
        ${mb-util_INCLUDE_DIRS}
        ${ulBasicCodeSupport_INCLUDE_DIRS}
        ${ulBuildEnv_INCLUDE_DIRS}
        ${ulLinklib_INCLUDE_DIRS}
        ${ulMem_INCLUDE_DIRS}
        ${ulString_INCLUDE_DIRS}
)

target_link_libraries(
    ${target}
    INTERFACE ulBuildEnv ulBasicCodeSupport ulString
)

ul_set_target_defaults(${target})

set(compile_check_target ${target}-compile-check)
add_executable(${compile_check_target} compile-check.cpp)
target_link_libraries(${compile_check_target} PRIVATE ${target})
ul_set_target_defaults(${compile_check_target})

#mb_install_library(mb.util)
