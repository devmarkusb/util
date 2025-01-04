cmake_minimum_required(VERSION 3.5)

include(${ulBuildEnv_CMAKE_DIRS}/cmake_util.cmake)

project(mb-util)

ul_include(defaults.cmake)

set(target ${PROJECT_NAME})

file(GLOB_RECURSE target_INCS "include/*.h")

set(${target}_INCLUDE_DIRS
        ${PROJECT_SOURCE_DIR}/include
        CACHE INTERNAL "${target}: include directories" FORCE)

add_library(${target} INTERFACE ${target_INCS})

target_include_directories(${target} INTERFACE
        ${mb-util_INCLUDE_DIRS}
        ${ulBasicCodeSupport_INCLUDE_DIRS}
        ${ulBuildEnv_INCLUDE_DIRS}
        ${ulLinklib_INCLUDE_DIRS}
        ${ulMem_INCLUDE_DIRS}
        ${ulString_INCLUDE_DIRS}
)

target_link_libraries(${target} INTERFACE ulBuildEnv ulBasicCodeSupport ulString)

ul_set_target_defaults(${target})
