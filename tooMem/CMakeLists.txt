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

project(tooMem VERSION 0.1.0.0)

set(Target_tooMem "tooMem")

include(defaults.cmake)

file(GLOB_RECURSE Target_tooMem_INCS "include/*.h")
file(GLOB_RECURSE Target_tooMem_SRC_INCS "src/*.h")

get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
set(tooMem_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
#set(tooMem_LIBRARY_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/lib PARENT_SCOPE)
endif()


################################################################################################

set(Target_tooMemTest "tooMemTest")

file(GLOB_RECURSE Target_tooMemTest_SRC_INCS "test/*.h")

set(Target_tooMemTest_SOURCES
	test/misc.test.helper.cpp
    test/checked_delete.test.cpp
    test/heap_tracked.test.cpp
    test/new_handler.test.cpp
)

add_executable(${Target_tooMemTest}
	${Target_tooMemTest_SOURCES}
	${Target_tooMem_INCS}
	${Target_tooMemTest_SRC_INCS}
)

if (WIN32)
set_property(TARGET ${Target_tooMemTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${Target_tooMemTest} PROPERTIES DEBUG_POSTFIX "d")

set(GTEST_ROOT $ENV{dev_sdk_paths})
set(GTEST_MSVC_SEARCH   MD)
find_package(GTest 1.7.0)

target_include_directories(${Target_tooMemTest} PRIVATE ${GTEST_INCLUDE_DIRS})
target_include_directories(${Target_tooMemTest} PRIVATE include)
target_include_directories(${Target_tooMemTest} PRIVATE ../tooBasicCodeSupport/include)
target_include_directories(${Target_tooMemTest} PRIVATE ../tooBuildEnv/include)

target_link_libraries(${Target_tooMemTest} PRIVATE ${GTEST_BOTH_LIBRARIES})


add_custom_command(
  TARGET ${Target_tooMemTest} POST_BUILD
  COMMAND ${Target_tooMemTest}
)
