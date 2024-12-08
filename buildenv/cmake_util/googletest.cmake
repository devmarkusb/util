cmake_minimum_required(VERSION 3.20)

find_package(GTest QUIET)
if (GTest_FOUND)
    add_library(libgtest INTERFACE)
    target_link_libraries(libgtest INTERFACE GTest::gtest)
    add_library(libgtest_main INTERFACE)
    target_link_libraries(libgtest_main INTERFACE GTest::gtest_main)
    add_library(libgmock INTERFACE)
    target_link_libraries(libgmock INTERFACE GTest::gmock)
    add_library(libgmock_main INTERFACE)
    target_link_libraries(libgmock_main INTERFACE GTest::gmock_main)
    return()
endif()
message(STATUS "google test not found, building from source")

enable_testing()

set(CMAKE_CXX_STANDARD ${UL_CXX_STANDARD})
set(CMAKE_CXX_EXTENSIONS OFF)
set(CXX_STANDARD_REQUIRED ON)

include(FindPkgConfig)
include(ExternalProject)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

pkg_search_module(GTestMain gtest_main)
if (NOT GTestMain_FOUND)
    find_package(Git REQUIRED QUIET)

    set(UL_GTEST_BUILD_DIR ${UL_3RDPARTY_BUILD_DIR}/googletest)
    configure_file(${CMAKE_CURRENT_LIST_DIR}/googletest/CMakeLists.txt.in ${UL_GTEST_BUILD_DIR}/googletest-download/CMakeLists.txt)
    execute_process(
            COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
            RESULT_VARIABLE result
            WORKING_DIRECTORY ${UL_GTEST_BUILD_DIR}/googletest-download
    )
    if(result)
        message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()

    execute_process(
            COMMAND ${CMAKE_COMMAND} --build .
            RESULT_VARIABLE result
            WORKING_DIRECTORY ${UL_GTEST_BUILD_DIR}/googletest-download
    )
    if(result)
        message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(GTestMain_LIBRARIES gtest_main)

    add_subdirectory(
            ${UL_GTEST_BUILD_DIR}/googletest-src
            ${UL_GTEST_BUILD_DIR}/googletest-build
            EXCLUDE_FROM_ALL
    )

    include(${CMAKE_CURRENT_LIST_DIR}/googletest_cfg.cmake)
endif()
