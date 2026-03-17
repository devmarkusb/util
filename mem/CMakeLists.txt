cmake_minimum_required(VERSION 3.23)

include(${ulBuildEnv_CMAKE_DIRS}/cmake_util.cmake)

project(ulMem VERSION 0.1.0.0)

ul_include(defaults.cmake)

set(target ${PROJECT_NAME})

file(GLOB_RECURSE target_INCS "include/*.h")
file(GLOB_RECURSE target_SRC_INCS "src/*.h*")

set(target_SOURCES src/new_statistics.cpp)

add_library(${target} STATIC ${target_SOURCES})
target_sources(${target} PUBLIC
    FILE_SET headers TYPE HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
    FILES ${target_INCS}
)
target_sources(${target} PRIVATE
    FILE_SET src_headers TYPE HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src"
    FILES ${target_SRC_INCS}
)
target_include_directories(
    ${target}
    PUBLIC ${ulBuildEnv_INCLUDE_DIRS} ${ulBasicCodeSupport_INCLUDE_DIRS}
)

target_link_libraries(${target} PUBLIC ulBuildEnv ulBasicCodeSupport)

ul_set_target_defaults(${target})

set(${target}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${ulBasicCodeSupport_INCLUDE_DIRS}
    CACHE INTERNAL
    "${target}: include directories"
    FORCE
)

################################################################################################

#todo
# from here on we have crashes using libc++
#if (${UL_CPP_STD_LIB} STREQUAL "libc++")
#    return ()
#endif ()
if(NOT UL_BUILD_UNITTESTS)
    return()
endif()

set(targetTest ${PROJECT_NAME}-test)

file(GLOB_RECURSE targetTest_SOURCES "src/*.test.cpp")
list(
    REMOVE_ITEM targetTest_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/src/new_statistics.test.cpp"
)
list(
    REMOVE_ITEM targetTest_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/src/new_statistics.applied.test.cpp"
)
add_executable(${targetTest} ${targetTest_SOURCES})

ul_add_test(${targetTest})

# using own main, implemented in new_statistics.applied.test.cpp, in order to test new_statistics in an 'application'
# like environment
target_link_libraries(${targetTest} PRIVATE gtest ${target})

ul_set_target_defaults(${targetTest})

ul_run_target_postbuild(${targetTest})

###

add_executable(${targetTest}_new_statistics src/new_statistics.test.cpp)

ul_add_simple_test(${targetTest}_new_statistics)
# LeakSanitizer can report false positives with our custom operator new (malloc + header offset);
# avoid overriding exit code so test pass/fail = main() return value.
set_tests_properties(
    ${targetTest}_new_statistics
    PROPERTIES ENVIRONMENT "LSAN_OPTIONS=detect_leaks=0"
)

# Whole archive so operator new/delete from new_statistics.cpp are actually linked (replace global ones)
target_link_libraries(
    ${targetTest}_new_statistics
    PRIVATE
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wl,--whole-archive>
    ${target}
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wl,--no-whole-archive>
)
ul_set_target_defaults(${targetTest}_new_statistics)

# *WARNING* This doesn't run in Release mode starting with gcc-10?!
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    ul_run_target_postbuild(${targetTest}_new_statistics)
endif()

###

add_executable(${targetTest}_new_statistics_applied src/new_statistics.applied.test.cpp)

ul_add_simple_test(${targetTest}_new_statistics_applied)
set_tests_properties(
    ${targetTest}_new_statistics_applied
    PROPERTIES ENVIRONMENT "LSAN_OPTIONS=detect_leaks=0"
)

# Whole archive so operator new/delete from new_statistics.cpp are actually linked (replace global ones)
target_link_libraries(
    ${targetTest}_new_statistics_applied
    PRIVATE
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wl,--whole-archive>
    ${target}
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wl,--no-whole-archive>
)
ul_set_target_defaults(${targetTest}_new_statistics_applied)

ul_run_target_postbuild(${targetTest}_new_statistics_applied)
