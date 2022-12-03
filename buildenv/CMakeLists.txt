cmake_minimum_required(VERSION 3.4.1)

include(cmake_util.cmake)

project(tooBuildEnv VERSION 0.1.0.0)

too_include(defaults.cmake)
too_include(gsl.cmake)

set(target ${PROJECT_NAME})

file(GLOB_RECURSE target_INCS "include/*.h")

add_library(${target} INTERFACE ${target_INCS})

target_include_directories(${target} INTERFACE ${GSL_SOURCE_DIR}/include)

target_link_libraries(${target} INTERFACE Microsoft.GSL::GSL)

#too_set_target_defaults(${target})

get_target_property(interfaceInclDirs ${target} INTERFACE_INCLUDE_DIRECTORIES)

set(${target}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include ${interfaceInclDirs}
    CACHE INTERNAL "${target}: include directories" FORCE)
set(${target}_CMAKE_DIRS ${PROJECT_SOURCE_DIR}
        CACHE INTERNAL "${target}: include directories" FORCE)


################################################################################################

if (NOT TOO_BUILD_UNITTESTS)
    return ()
endif ()

set(targetTest ${PROJECT_NAME}Test)

file(GLOB_RECURSE targetTest_SOURCES "include/toolib/*.test.cpp")

add_executable(${targetTest}
    ${targetTest_SOURCES}
    ${target_INCS}
)

too_add_test(${targetTest})

target_include_directories(${targetTest} PRIVATE ${tooBuildEnv_INCLUDE_DIRS})

target_link_libraries(${targetTest} PRIVATE gtest_main) #${target}

too_set_target_defaults(${targetTest})

too_run_target_postbuild(${targetTest})
