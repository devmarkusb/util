cmake_minimum_required(VERSION 3.4.1)

include(${tooBuildEnv_CMAKE_DIRS}/cmake_util.cmake)

project(tooLinklib VERSION 0.1.0.0)

too_include(defaults.cmake)

set(target ${PROJECT_NAME})

file(GLOB_RECURSE target_INCS "include/*.h")

#add_library(${target} STATIC ${target_INCS})

#target_include_directories(${target} PUBLIC ${tooBuildEnv_INCLUDE_DIRS})

#target_link_libraries(${target}) # tooBuildEnv

#too_set_target_defaults(${target})

set(${target}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${tooBuildEnv_INCLUDE_DIRS}
    CACHE INTERNAL "${target}: include directories" FORCE)


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

too_add_test(${targetTest})

target_include_directories(${targetTest} PRIVATE ${tooLinklib_INCLUDE_DIRS})

target_link_libraries(${targetTest} PRIVATE gtest_main) #${target}

too_set_target_defaults(${targetTest})

too_run_target_postbuild(${targetTest})
