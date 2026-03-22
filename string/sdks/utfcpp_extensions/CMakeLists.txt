cmake_minimum_required(VERSION 3.23)

if(NOT UL_BUILD_UNITTESTS)
    return()
endif()

project(utfcppTest)
set(target ${PROJECT_NAME})

file(GLOB_RECURSE ${target}_SRC_INCS "test/*.h")
file(GLOB_RECURSE ${target}_SOURCES "test/*.test.cpp")

add_executable(${target} ${${target}_SOURCES})
target_sources(
    ${target}
    PRIVATE
        FILE_SET headers
            TYPE HEADERS
            BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/test"
            FILES ${${target}_SRC_INCS}
)

target_link_libraries(${target} PRIVATE GTest::gtest GTest::gtest_main ulString)

ul_set_target_defaults(${target})
