cmake_minimum_required(VERSION 3.5)

include(${ulBuildEnv_CMAKE_DIRS}/cmake_util.cmake)

project(ulMem VERSION 0.1.0.0)

ul_include(defaults.cmake)

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
        ${ulBuildEnv_INCLUDE_DIRS}
        ${ulBasicCodeSupport_INCLUDE_DIRS}
)

target_link_libraries(${target} PUBLIC ulBuildEnv ulBasicCodeSupport)

ul_set_target_defaults(${target})

set(${target}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${ulBasicCodeSupport_INCLUDE_DIRS}
    CACHE INTERNAL "${target}: include directories" FORCE)


################################################################################################

#todo
# from here on we have crashes using libc++
#if (${UL_CPP_STD_LIB} STREQUAL "libc++")
#    return ()
#endif ()
if (NOT UL_BUILD_UNITTESTS)
    return ()
endif ()

set(targetTest ${PROJECT_NAME}-test)

file(GLOB_RECURSE targetTest_SOURCES "src/*.test.cpp")
list(REMOVE_ITEM targetTest_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/new_statistics.test.cpp")
list(REMOVE_ITEM targetTest_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/new_statistics.applied.test.cpp")
add_executable(${targetTest}
    ${targetTest_SOURCES}
    ${target_INCS}
)

ul_add_test(${targetTest})

target_include_directories(${targetTest} PRIVATE ${ulMem_INCLUDE_DIRS})

# using own main, implemented in new_statistics.applied.test.cpp, in order to test new_statistics in an 'application'
# like environment
target_link_libraries(${targetTest} PRIVATE gtest ${target})

ul_set_target_defaults(${targetTest})

ul_run_target_postbuild(${targetTest})

###

add_executable(${targetTest}_new_statistics
        src/new_statistics.test.cpp
        ${target_INCS}
)

ul_add_test(${targetTest}_new_statistics)

target_include_directories(${targetTest}_new_statistics PRIVATE ${ulMem_INCLUDE_DIRS})

target_link_libraries(${targetTest}_new_statistics PRIVATE ${target})

ul_set_target_defaults(${targetTest}_new_statistics)

# *WARNING* This doesn't run in Release mode starting with gcc-10?!
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    ul_run_target_postbuild(${targetTest}_new_statistics)
endif ()

###

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    return ()
endif ()

add_executable(${targetTest}_new_statistics_applied
        src/new_statistics.applied.test.cpp
        ${target_INCS}
        )

ul_add_test(${targetTest}_new_statistics_applied)

target_include_directories(${targetTest}_new_statistics_applied PRIVATE ${ulMem_INCLUDE_DIRS})

target_link_libraries(${targetTest}_new_statistics_applied PRIVATE ${target})

ul_set_target_defaults(${targetTest}_new_statistics_applied)

ul_run_target_postbuild(${targetTest}_new_statistics_applied)
