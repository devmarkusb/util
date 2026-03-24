cmake_minimum_required(VERSION 3.23)

include(${ulBuildEnv_CMAKE_DIRS}/cmake_util.cmake)

ul_include(defaults.cmake)

file(GLOB_RECURSE target_INCS "include/*.h")

add_library(ulLinklib INTERFACE)
target_sources(
    ulLinklib
    INTERFACE
        FILE_SET headers
            TYPE HEADERS
            BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
            FILES ${target_INCS}
)
target_include_directories(ulLinklib INTERFACE ${ulBuildEnv_INCLUDE_DIRS})

set(ulLinklib_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${ulBuildEnv_INCLUDE_DIRS}
    CACHE INTERNAL
    "ulLinklib: include directories"
    FORCE
)

################################################################################################

if(NOT UL_BUILD_UNITTESTS)
    return()
endif()

set(targetTest ulLinklib-test)

file(GLOB_RECURSE targetTest_SOURCES "src/*.test.cpp")

add_executable(${targetTest} ${targetTest_SOURCES})

ul_add_test(${targetTest})

target_link_libraries(
    ${targetTest}
    PRIVATE GTest::gtest GTest::gtest_main ulLinklib
)

ul_set_target_essentials(${targetTest})
