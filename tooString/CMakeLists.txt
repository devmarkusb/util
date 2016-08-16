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

project(tooString VERSION 0.1.0.0)
set(Target_tooString "tooString")

include(defaults.cmake)

add_subdirectory(sdks)

file(GLOB_RECURSE Target_tooString_INCS "include/*.h*")

get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
set(tooString_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
endif()


################################################################################################

set(Target_tooStringTest "tooStringTest")

file(GLOB_RECURSE Target_tooStringTest_SRC_INCS "test/*.h")

set(Target_tooStringTest_SOURCES
	test/_header_compile.test.cpp
	test/misc.test.cpp
	test/str_convert.test.cpp
)

add_executable(${Target_tooStringTest}
	${Target_tooStringTest_SOURCES}
	${Target_tooString_INCS}
	${Target_tooStringTest_SRC_INCS}
)

if (WIN32)
set_property(TARGET ${Target_tooStringTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${Target_tooStringTest} PROPERTIES DEBUG_POSTFIX "d")

set(GTEST_ROOT $ENV{dev_sdk_paths})
set(GTEST_MSVC_SEARCH   MD)
find_package(GTest 1.7.0)

target_include_directories(${Target_tooStringTest} PRIVATE ${GTEST_INCLUDE_DIRS})
target_include_directories(${Target_tooStringTest} PRIVATE include)
target_include_directories(${Target_tooStringTest} PRIVATE ../tooBuildEnv/include)
target_include_directories(${Target_tooStringTest} PRIVATE ../tooBasicCodeSupport/include)
target_include_directories(${Target_tooStringTest} PRIVATE sdks/utf8cpp/source)

target_link_libraries(${Target_tooStringTest} PRIVATE ${GTEST_BOTH_LIBRARIES})

set(do_auto_test	ON)

if (do_auto_test)
add_custom_command(
  TARGET ${Target_tooStringTest} POST_BUILD
  COMMAND ${Target_tooStringTest}
)
endif ()
