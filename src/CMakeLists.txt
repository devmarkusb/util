cmake_minimum_required(VERSION 3.23)

include(${UL_CMAKE_UTIL_DIR}/cmake-util.cmake OPTIONAL)

ul_include(defaults.cmake)
ul_include(gsl.cmake)

add_subdirectory(string/sdks)

file(GLOB_RECURSE target_INCS "${CMAKE_CURRENT_SOURCE_DIR}/../include/*.h*")
file(GLOB_RECURSE target_SRC_INCS "${CMAKE_CURRENT_SOURCE_DIR}/*.h*")

# Exclude gtest-utils.h if not building tests
if(NOT UL_BUILD_UNITTESTS)
    list(FILTER target_INCS EXCLUDE REGEX "gtest_utils\\.h$")
endif()

file(GLOB_RECURSE target_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(FILTER target_SOURCES EXCLUDE REGEX "\\.test\\.cpp$")
list(FILTER target_SOURCES EXCLUDE REGEX "compile-check\\.cpp$")

add_library(mb.util STATIC ${target_SOURCES})

target_sources(
    mb.util
    PUBLIC
        FILE_SET headers
            TYPE HEADERS
            BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../include"
            FILES ${target_INCS}
)
target_sources(
    mb.util
    PRIVATE
        FILE_SET src_headers
            TYPE HEADERS
            BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
            FILES ${target_SRC_INCS}
)

target_include_directories(
    mb.util
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include
)
target_include_directories(mb.util PUBLIC ${GSL_SOURCE_DIR}/include)
target_include_directories(mb.util SYSTEM INTERFACE ${utfcpp_INCLUDE_DIRS})

target_link_libraries(mb.util PUBLIC Microsoft.GSL::GSL)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_libraries(mb.util PUBLIC stdc++exp)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND NOT ANDROID)
    target_link_libraries(mb.util PUBLIC stdc++exp)
    target_compile_options(mb.util INTERFACE -Wno-unsafe-buffer-usage)
endif()

if(UL_BUILD_UNITTESTS)
    target_link_libraries(mb.util INTERFACE $<BUILD_INTERFACE:GTest::gtest>)
endif()

ul_set_target_defaults(mb.util)

# compile-check
add_library(mb.util-compile-check STATIC compile-check.cpp)
target_link_libraries(mb.util-compile-check PRIVATE mb.util)
ul_set_target_defaults(mb.util-compile-check)

if(NOT UL_BUILD_UNITTESTS)
    return()
endif()

set(targetTest mb.util-test)

file(GLOB_RECURSE targetTest_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.test.cpp")

add_executable(${targetTest} ${targetTest_SOURCES})

ul_add_test(${targetTest})

target_link_libraries(
    ${targetTest}
    PRIVATE GTest::gtest GTest::gtest_main mb.util
)

if(MB_DEVENV_SANITIZER)
    target_compile_definitions(${targetTest} PRIVATE MB_DEVENV_SANITIZER=1)
endif()

ul_set_target_filesystem(${targetTest})
ul_set_target_essentials(${targetTest})
