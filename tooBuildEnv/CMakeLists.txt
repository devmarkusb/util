# Some notes for the occasional CMake user.
#
# 1 Debug or Release or...
# start cmake with arg either
# -DCMAKE_BUILD_TYPE=Debug
# or
# -DCMAKE_BUILD_TYPE=Release
# Other options are: MinSizeRel, RelWithDebInfo
#
# 2 Dynamic vs Static C/C++ runtime lib
# There is no support for static runtime libs! You could achieve this
# for MSVC, but this is not recommended. You or other will end up in
# conflicts soon.

cmake_minimum_required(VERSION 3.4.1)

project(tooBuildEnv VERSION 0.1.0.0)

set(Target_tooBuildEnv ${PROJECT_NAME})

include(defaults.cmake)

file(GLOB_RECURSE Target_tooBuildEnv_INCS "include/*.h")

add_library(${Target_tooBuildEnv} STATIC ${Target_tooBuildEnv_INCS})
set_target_properties(${Target_tooBuildEnv} PROPERTIES LINKER_LANGUAGE CXX)

set_target_properties(${Target_tooBuildEnv}
                      PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(${Target_tooBuildEnv})

set(${Target_tooBuildEnv}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL "${Target_tooBuildEnv}: include directories" FORCE)


################################################################################################

set(Target_tooBuildEnvTest ${PROJECT_NAME}Test)

file(GLOB_RECURSE Target_tooBuildEnvTest_SOURCES "include/Toolib/*.test.cpp")

add_executable(${Target_tooBuildEnvTest}
	${Target_tooBuildEnvTest_SOURCES}
	${Target_tooBuildEnv_INCS}
)

if (WIN32)
set_property(TARGET ${Target_tooBuildEnvTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${Target_tooBuildEnvTest} PROPERTIES DEBUG_POSTFIX "d")

target_include_directories(${Target_tooBuildEnvTest} PRIVATE include)

target_link_libraries(${Target_tooBuildEnvTest} PRIVATE gtest_main)


add_custom_command(
  TARGET ${Target_tooBuildEnvTest} POST_BUILD
  COMMAND ${Target_tooBuildEnvTest}
)
