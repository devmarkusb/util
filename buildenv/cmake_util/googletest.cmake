cmake_minimum_required(VERSION 3.20)

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
