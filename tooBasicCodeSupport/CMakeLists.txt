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

project(tooBasicCodeSupport VERSION 0.1.0.0)

set(Target_tooBasicCodeSupport "tooBasicCodeSupport")

include(defaults.cmake)

add_subdirectory(sdks/ToolsFL/tooBuildEnv)

file(GLOB_RECURSE Target_tooBasicCodeSupport_INCS "include/*.h")
file(GLOB_RECURSE Target_tooBasicCodeSupport_SRC_INCS "src/*.h")

get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
set(tooBasicCodeSupport_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
#set(tooBasicCodeSupport_LIBRARY_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/lib PARENT_SCOPE)
endif()


################################################################################################

set(Target_tooBasicCodeSupportTest "tooBasicCodeSupportTest")

file(GLOB_RECURSE Target_tooBasicCodeSupportTest_SRC_INCS "test/*.h")

set(Target_tooBasicCodeSupportTest_SOURCES
	test/assert.test.cpp
	test/finally.test.cpp
	test/narrow.test.cpp
)

add_executable(${Target_tooBasicCodeSupportTest}
	${Target_tooBasicCodeSupportTest_SOURCES}
	${Target_tooBasicCodeSupport_INCS}
	${Target_tooBasicCodeSupportTest_SRC_INCS}
)

if (WIN32)
set_property(TARGET ${Target_tooBasicCodeSupportTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${Target_tooBasicCodeSupportTest} PROPERTIES DEBUG_POSTFIX "d")

set(GTEST_ROOT "c:/projects/externlibs/gtest-1.7.0")
set(GTEST_MSVC_SEARCH   MD)
find_package(GTest 1.7.0)

target_include_directories(${Target_tooBasicCodeSupportTest} PRIVATE ${GTEST_INCLUDE_DIRS})
target_include_directories(${Target_tooBasicCodeSupportTest} PRIVATE include)
target_include_directories(${Target_tooBasicCodeSupportTest} PRIVATE sdks/ToolsFL/tooBuildEnv/include)

target_link_libraries(${Target_tooBasicCodeSupportTest} PRIVATE ${GTEST_BOTH_LIBRARIES})
#target_link_libraries(${Target_tooBasicCodeSupportTest} PRIVATE ${Target_tooBasicCodeSupport})


add_custom_command(
  TARGET ${Target_tooBasicCodeSupportTest} POST_BUILD
  COMMAND ${Target_tooBasicCodeSupportTest}
)
