cmake_minimum_required(VERSION 3.23)

include(${ulBuildEnv_CMAKE_DIRS}/cmake_util.cmake)

ul_include(defaults.cmake)

add_subdirectory(sdks)

file(GLOB_RECURSE target_INCS "include/*.h*")

add_library(ulString INTERFACE)
target_sources(
    ulString
    INTERFACE
        FILE_SET headers
            TYPE HEADERS
            BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
            FILES ${target_INCS}
)

set(ulString_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${ulBasicCodeSupport_INCLUDE_DIRS}
    CACHE INTERNAL
    "ulString: include directories"
    FORCE
)

target_include_directories(ulString INTERFACE ${ulString_INCLUDE_DIRS})
target_include_directories(ulString SYSTEM INTERFACE ${utfcpp_INCLUDE_DIRS})

target_link_libraries(ulString INTERFACE ulBasicCodeSupport) # utf8cpp

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(ulString INTERFACE -Wno-unsafe-buffer-usage)
endif()

ul_set_target_defaults(ulString)

################################################################################################

if(NOT UL_BUILD_UNITTESTS)
    return()
endif()

set(targetTest ulString-test)

file(GLOB_RECURSE targetTest_SOURCES "src/*.test.cpp")

add_executable(${targetTest} ${targetTest_SOURCES})

ul_add_test(${targetTest})

target_link_libraries(
    ${targetTest}
    PRIVATE GTest::gtest GTest::gtest_main ulString
)

ul_set_target_essentials(${targetTest})
