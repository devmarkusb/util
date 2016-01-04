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
set(Target_tooBuildEnv "tooBuildEnv")

include(defaults.cmake)

file(GLOB_RECURSE Target_tooBuildEnv_INCS "include/*.h")
file(GLOB_RECURSE Target_tooBuildEnv_SRC_INCS "src/*.h")

#[[
set(Target_tooBuildEnv_SOURCES
    "src/non-empty folder.cpp"
)

add_library(${Target_tooBuildEnv} SHARED
	${Target_tooBuildEnv_SOURCES}
	${Target_tooBuildEnv_INCS}
	${Target_tooBuildEnv_SRC_INCS}
)

if (WIN32)
set_property(TARGET ${Target_tooBuildEnv} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

# dll export define
set_property(TARGET ${Target_tooBuildEnv} APPEND PROPERTY COMPILE_DEFINITIONS TOOBUILDENV_LIBRARY)

set_target_properties(${Target_tooBuildEnv} PROPERTIES DEBUG_POSTFIX "d")

target_include_directories(${Target_tooBuildEnv} PRIVATE .)
target_include_directories(${Target_tooBuildEnv} PRIVATE include)
target_include_directories(${Target_tooBuildEnv} PRIVATE src)
]]

get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
set(tooBuildEnv_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
#set(tooBuildEnv_LIBRARY_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/lib PARENT_SCOPE)
else()
set(tooBuildEnv_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
#set(tooBuildEnv_LIBRARY_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/lib)
endif()


################################################################################################

set(Target_tooBuildEnvTest "tooBuildEnvTest")

file(GLOB_RECURSE Target_tooBuildEnvTest_SRC_INCS "test/*.h")

set(Target_tooBuildEnvTest_SOURCES
	test/dummy.cpp
)

add_executable(${Target_tooBuildEnvTest}
	${Target_tooBuildEnvTest_SOURCES}
	${Target_tooBuildEnv_INCS}
	${Target_tooBuildEnvTest_SRC_INCS}
)

if (WIN32)
set_property(TARGET ${Target_tooBuildEnvTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${Target_tooBuildEnvTest} PROPERTIES DEBUG_POSTFIX "d")

set(GTEST_ROOT "c:/projects/externlibs/gtest-1.7.0")
set(GTEST_MSVC_SEARCH   MD)
find_package(GTest 1.7.0)

target_include_directories(${Target_tooBuildEnvTest} PRIVATE ${GTEST_INCLUDE_DIRS})
message(${tooBuildEnv_INCLUDE_DIRS})
target_include_directories(${Target_tooBuildEnvTest} PRIVATE ${tooBuildEnv_INCLUDE_DIRS})

target_link_libraries(${Target_tooBuildEnvTest} PRIVATE ${GTEST_BOTH_LIBRARIES})
#target_link_libraries(${Target_tooBuildEnvTest} PRIVATE ${Target_tooBuildEnv})


add_custom_command(
  TARGET ${Target_tooBuildEnvTest} POST_BUILD
  COMMAND ${Target_tooBuildEnvTest}
)
