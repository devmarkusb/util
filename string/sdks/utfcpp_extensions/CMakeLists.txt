cmake_minimum_required(VERSION 3.23)

if(NOT UL_BUILD_UNITTESTS)
    return()
endif()

file(GLOB_RECURSE utfcppTest_SRC_INCS "test/*.h")
file(GLOB_RECURSE utfcppTest_SOURCES "test/*.test.cpp")

add_executable(utfcppTest ${utfcppTest_SOURCES})
target_sources(
    utfcppTest
    PRIVATE
        FILE_SET headers
            TYPE HEADERS
            BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/test"
            FILES ${utfcppTest_SRC_INCS}
)

target_link_libraries(utfcppTest PRIVATE GTest::gtest GTest::gtest_main ulString)

ul_set_target_defaults(utfcppTest)
