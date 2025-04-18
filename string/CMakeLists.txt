cmake_minimum_required(VERSION 3.5)

include(${ulBuildEnv_CMAKE_DIRS}/cmake_util.cmake)

project(ulString VERSION 0.1.0.0)

ul_include(defaults.cmake)

set(target ${PROJECT_NAME})

add_subdirectory(sdks)

file(GLOB_RECURSE target_INCS "include/*.h*")

add_library(${target} INTERFACE ${target_INCS})

set(${target}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${ulBasicCodeSupport_INCLUDE_DIRS}
    CACHE INTERNAL "${target}: include directories" FORCE)

target_include_directories(${target} INTERFACE ${${target}_INCLUDE_DIRS})
target_include_directories(${target} SYSTEM INTERFACE ${utfcpp_INCLUDE_DIRS})

target_link_libraries(${target} INTERFACE ulBasicCodeSupport) # utf8cpp

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(${target} INTERFACE -Wno-unsafe-buffer-usage)
endif ()

ul_set_target_defaults(${target})


################################################################################################

if (NOT UL_BUILD_UNITTESTS)
    return ()
endif ()

set(targetTest ${PROJECT_NAME}-test)

file(GLOB_RECURSE targetTest_SOURCES "src/*.test.cpp")

add_executable(${targetTest}
    ${targetTest_SOURCES}
    ${target_INCS}
)

ul_add_test(${targetTest})

target_include_directories(${targetTest} PRIVATE ${ulString_INCLUDE_DIRS})

target_link_libraries(${targetTest} PRIVATE gtest_main ${target})

ul_set_target_defaults(${targetTest})

ul_run_target_postbuild(${targetTest})
