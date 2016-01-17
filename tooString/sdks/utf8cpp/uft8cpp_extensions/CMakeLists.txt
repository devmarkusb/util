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

project(LibUtf8cpp VERSION 0.1.0.0)
set(TargetUtf8cpp "utf8cpp")

include(defaults.cmake)

file(GLOB_RECURSE TargetUtf8cpp_INCS "../source/*.h")

get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
set(utf8cpp_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}../source PARENT_SCOPE)
endif()


################################################################################################

set(TargetUtf8cppTest "utf8cppTest")

file(GLOB_RECURSE TargetUtf8cppTest_SRC_INCS "test/*.h")

set(TargetUtf8cppTest_SOURCES
	"test/misc.test.cpp"
)

add_executable(${TargetUtf8cppTest}
	${TargetUtf8cppTest_SOURCES}
	${TargetUtf8cpp_INCS}
)

if (WIN32)
set_property(TARGET ${TargetUtf8cppTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${TargetUtf8cppTest} PROPERTIES DEBUG_POSTFIX "d")

set(GTEST_ROOT "c:/projects/externlibs/gtest-1.7.0")
set(GTEST_MSVC_SEARCH   MD)
find_package(GTest 1.7.0)

target_include_directories(${TargetUtf8cppTest} PRIVATE ${GTEST_INCLUDE_DIRS})
target_include_directories(${TargetUtf8cppTest} PRIVATE ../source)
target_include_directories(${TargetUtf8cppTest} PRIVATE ../../../../tooBuildEnv/include)
target_include_directories(${TargetUtf8cppTest} PRIVATE ../../../../tooBasicCodeSupport/include)

target_link_libraries(${TargetUtf8cppTest} PRIVATE ${GTEST_BOTH_LIBRARIES})


set(do_auto_test	ON)

if (do_auto_test)
add_custom_command(
  TARGET ${TargetUtf8cppTest} POST_BUILD
  COMMAND ${TargetUtf8cppTest}
)
endif ()
