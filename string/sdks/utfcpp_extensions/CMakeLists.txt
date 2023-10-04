cmake_minimum_required(VERSION 3.5)

if (NOT UL_BUILD_UNITTESTS)
    return ()
endif ()

project(utfcppTest)
set(target ${PROJECT_NAME})

file(GLOB_RECURSE ${target}_SRC_INCS "test/*.h")
file(GLOB_RECURSE ${target}_SOURCES "test/*.test.cpp")

add_executable(${target}
    ${${target}_SOURCES}
    ${${target}_SRC_INCS}
)

target_include_directories(${target} PRIVATE ${ulBasicCodeSupport_INCLUDE_DIRS})
target_include_directories(${target} PRIVATE ${ulString_INCLUDE_DIRS})
target_include_directories(${target} PRIVATE ${utfcpp_INCLUDE_DIRS})

target_link_libraries(${target} PRIVATE gtest_main)

ul_set_target_defaults(${target})

ul_run_target_postbuild(${target})
