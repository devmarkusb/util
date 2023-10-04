cmake_minimum_required(VERSION 3.5)

include(cmake_util.cmake)

project(ulBuildEnv VERSION 0.1.0.0)

ul_include(defaults.cmake)
ul_include(gsl.cmake)

set(target ${PROJECT_NAME})

file(GLOB_RECURSE target_INCS "include/*.h")

add_library(${target} INTERFACE ${target_INCS})

target_include_directories(${target} INTERFACE ${GSL_SOURCE_DIR}/include)

target_link_libraries(${target} INTERFACE Microsoft.GSL::GSL)

ul_set_target_defaults(${target})

get_target_property(interfaceInclDirs ${target} INTERFACE_INCLUDE_DIRECTORIES)

set(${target}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include ${interfaceInclDirs}
    CACHE INTERNAL "${target}: include directories" FORCE)
set(${target}_CMAKE_DIRS ${PROJECT_SOURCE_DIR}
        CACHE INTERNAL "${target}: include directories" FORCE)


################################################################################################

if (NOT UL_BUILD_UNITTESTS)
    return ()
endif ()

set(targetTest ${PROJECT_NAME}Test)

file(GLOB_RECURSE targetTest_SOURCES "src/*.test.cpp")

add_executable(${targetTest}
    ${targetTest_SOURCES}
    ${target_INCS}
)

ul_add_test(${targetTest})

target_include_directories(${targetTest} PRIVATE ${ulBuildEnv_INCLUDE_DIRS})

target_link_libraries(${targetTest} PRIVATE gtest_main) #${target}

ul_set_target_defaults(${targetTest})

ul_run_target_postbuild(${targetTest})
