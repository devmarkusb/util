cmake_minimum_required(VERSION 3.4.1)

include(cmake_util.cmake)

project(tooBuildEnv VERSION 0.1.0.0)

set(Target_tooBuildEnv ${PROJECT_NAME})

file(GLOB_RECURSE Target_tooBuildEnv_INCS "include/*.h")

add_library(${Target_tooBuildEnv} STATIC ${Target_tooBuildEnv_INCS})

target_link_libraries(${Target_tooBuildEnv})

too_set_target_defaults(${Target_tooBuildEnv})

set(${Target_tooBuildEnv}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL "${Target_tooBuildEnv}: include directories" FORCE)


################################################################################################

if (NOT TOO_BUILD_UNITTESTS)
    return ()
endif ()

set(Target_tooBuildEnvTest ${PROJECT_NAME}Test)

file(GLOB_RECURSE Target_tooBuildEnvTest_SOURCES "include/toolib/*.test.cpp")

add_executable(${Target_tooBuildEnvTest}
    ${Target_tooBuildEnvTest_SOURCES}
    ${Target_tooBuildEnv_INCS}
)

target_include_directories(${Target_tooBuildEnvTest} PRIVATE ${tooBuildEnv_INCLUDE_DIRS})

target_link_libraries(${Target_tooBuildEnvTest} PRIVATE gtest_main) #${Target_tooBuildEnv}

too_set_target_defaults(${Target_tooBuildEnvTest})

too_run_target_postbuild(${Target_tooBuildEnvTest})
