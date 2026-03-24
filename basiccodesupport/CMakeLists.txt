cmake_minimum_required(VERSION 3.23)

include(${ulBuildEnv_CMAKE_DIRS}/cmake_util.cmake)

ul_include(defaults.cmake)

file(GLOB_RECURSE target_INCS "include/*.h")

add_library(ulBasicCodeSupport INTERFACE)
target_sources(
    ulBasicCodeSupport
    INTERFACE
        FILE_SET headers
            TYPE HEADERS
            BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
            FILES ${target_INCS}
)

set(ulBasicCodeSupport_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${ulBuildEnv_INCLUDE_DIRS}
    CACHE INTERNAL
    "ulBasicCodeSupport: include directories"
    FORCE
)

target_include_directories(
    ulBasicCodeSupport
    INTERFACE ${ulBuildEnv_INCLUDE_DIRS} ${ulBasicCodeSupport_INCLUDE_DIRS}
)

target_link_libraries(ulBasicCodeSupport INTERFACE ulBuildEnv)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_libraries(ulBasicCodeSupport INTERFACE stdc++exp)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND NOT ANDROID)
    # workaround
    # needed for stacktrace
    target_link_libraries(ulBasicCodeSupport INTERFACE stdc++exp)
endif()

ul_set_target_defaults(ulBasicCodeSupport)

################################################################################################

if(NOT UL_BUILD_UNITTESTS)
    return()
endif()

set(targetTest ulBasicCodeSupport-test)

file(GLOB_RECURSE targetTest_SOURCES "src/*.test.cpp")

add_executable(${targetTest} ${targetTest_SOURCES})

ul_add_test(${targetTest})

target_link_libraries(
    ${targetTest}
    PRIVATE GTest::gtest GTest::gtest_main ulBasicCodeSupport
)

if(MB_DEVENV_SANITIZER)
    target_compile_definitions(${targetTest} PRIVATE MB_DEVENV_SANITIZER=1)
endif()

ul_set_target_filesystem(${targetTest})

ul_set_target_essentials(${targetTest})

#ul_target_coverage(ulBasicCodeSupport)
