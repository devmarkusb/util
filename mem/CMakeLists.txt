cmake_minimum_required(VERSION 3.4.1)

include(${tooBuildEnv_CMAKE_DIRS}/cmake_util.cmake)

project(tooMem VERSION 0.1.0.0)

too_include(defaults.cmake)

set(target ${PROJECT_NAME})

file(GLOB_RECURSE target_INCS "include/*.h")
file(GLOB_RECURSE target_SRC_INCS "src/*.h*")

set(target_SOURCES
        src/new_statistics.cpp
)

add_library(${target} STATIC
        ${target_SOURCES}
        ${target_INCS}
        ${target_SRC_INCS}
)

target_include_directories(${target} PUBLIC
        include
        ${tooBuildEnv_INCLUDE_DIRS}
        ${tooBasicCodeSupport_INCLUDE_DIRS}
)

too_set_target_defaults(${target})

set(${target}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${tooBasicCodeSupport_INCLUDE_DIRS}
    CACHE INTERNAL "${target}: include directories" FORCE)


################################################################################################

if (NOT TOO_BUILD_UNITTESTS)
    return ()
endif ()

set(targetTest ${PROJECT_NAME}Test)

file(GLOB_RECURSE targetTest_SOURCES "include/toolib/*.test.cpp")
list(REMOVE_ITEM targetTest_SOURCES "${CMAKE_CURRENT_LIST_DIR}/include/toolib/mem/new_statistics.test.cpp")
add_executable(${targetTest}
    ${targetTest_SOURCES}
    ${target_INCS}
)

too_add_test(${targetTest})

target_include_directories(${targetTest} PRIVATE ${tooMem_INCLUDE_DIRS})

# using own main, implemented in new_statistics.applied.test.cpp, in order to test new_statistics in an 'application'
# like environment
target_link_libraries(${targetTest} PRIVATE gtest ${target})

too_set_target_defaults(${targetTest})

too_run_target_postbuild(${targetTest})


add_executable(${targetTest}_new_statistics
        include/toolib/mem/new_statistics.test.cpp
        ${target_INCS}
)

too_add_test(${targetTest}_new_statistics)

target_include_directories(${targetTest}_new_statistics PRIVATE ${tooMem_INCLUDE_DIRS})

target_link_libraries(${targetTest}_new_statistics PRIVATE ${target})

too_set_target_defaults(${targetTest}_new_statistics)

# *WARNING* This doesn't run in Release mode starting with gcc-10?!
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    too_run_target_postbuild(${targetTest}_new_statistics)
endif ()
