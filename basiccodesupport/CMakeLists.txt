cmake_minimum_required(VERSION 3.4.1)

include(${ulBuildEnv_CMAKE_DIRS}/cmake_util.cmake)

project(ulBasicCodeSupport VERSION 0.1.0.0)

ul_include(defaults.cmake)

set(target ${PROJECT_NAME})

file(GLOB_RECURSE target_INCS "include/*.h")

add_library(${target} STATIC ${target_INCS} src/empty.cpp)

set(${target}_INCLUDE_DIRS
        ${PROJECT_SOURCE_DIR}/include
        ${ulBuildEnv_INCLUDE_DIRS}
        CACHE INTERNAL "${target}: include directories" FORCE)

target_include_directories(${target} PUBLIC ${ulBuildEnv_INCLUDE_DIRS} ${ulBasicCodeSupport_INCLUDE_DIRS})

target_link_libraries(${target} PUBLIC ulBuildEnv)

#ul_set_target_defaults(${target})


################################################################################################

if (NOT TOO_BUILD_UNITTESTS)
    return ()
endif ()

set(targetTest ${PROJECT_NAME}Test)

file(GLOB_RECURSE targetTest_SOURCES "src/*.test.cpp")

add_executable(${targetTest}
    ${targetTest_SOURCES}
    ${target_INCS}
)

ul_add_test(${targetTest})

target_include_directories(${targetTest} PRIVATE ${ulBasicCodeSupport_INCLUDE_DIRS})

target_link_libraries(${targetTest} PRIVATE gtest_main) #${target}

ul_set_target_filesystem(${targetTest})

ul_set_target_defaults(${targetTest})

ul_run_target_postbuild(${targetTest})

#ul_target_coverage(${PROJECT_NAME})
