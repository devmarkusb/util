cmake_minimum_required(VERSION 3.23)

include(${ulBuildEnv_CMAKE_DIRS}/cmake_util.cmake)

project(ulBasicCodeSupport VERSION 0.1.0.0)

ul_include(defaults.cmake)

set(target ${PROJECT_NAME})

file(GLOB_RECURSE target_INCS "include/*.h")

add_library(${target} INTERFACE)
target_sources(${target} INTERFACE
    FILE_SET headers TYPE HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
    FILES ${target_INCS}
)

set(${target}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${ulBuildEnv_INCLUDE_DIRS}
    CACHE INTERNAL
    "${target}: include directories"
    FORCE
)

target_include_directories(
    ${target}
    INTERFACE ${ulBuildEnv_INCLUDE_DIRS} ${ulBasicCodeSupport_INCLUDE_DIRS}
)

target_link_libraries(${target} INTERFACE ulBuildEnv)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_libraries(${target} INTERFACE stdc++exp)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND NOT ANDROID)
    # workaround
    # needed for stacktrace
    target_link_libraries(${target} INTERFACE stdc++exp)
endif()

ul_set_target_defaults(${target})

################################################################################################

if(NOT UL_BUILD_UNITTESTS)
    return()
endif()

set(targetTest ${PROJECT_NAME}-test)

file(GLOB_RECURSE targetTest_SOURCES "src/*.test.cpp")

add_executable(${targetTest} ${targetTest_SOURCES})

ul_add_test(${targetTest})

target_link_libraries(${targetTest} PRIVATE GTest::gtest GTest::gtest_main ${target})

ul_set_target_filesystem(${targetTest})

ul_set_target_essentials(${targetTest})

ul_run_target_postbuild(${targetTest})

#ul_target_coverage(${PROJECT_NAME})
