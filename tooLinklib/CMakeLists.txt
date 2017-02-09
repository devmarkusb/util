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

project(tooLinklib VERSION 0.1.0.0)

set(Target_tooLinklib ${PROJECT_NAME})

include(defaults.cmake)

file(GLOB_RECURSE Target_tooLinklib_INCS "include/*.h")

get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
set(tooLinklib_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
#set(tooLinklib_LIBRARY_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/lib PARENT_SCOPE)
endif()


################################################################################################

set(Target_tooLinklibTest ${PROJECT_NAME}Test)

file(GLOB_RECURSE Target_tooLinklibTest_SOURCES "include/Toolib/*.test.cpp")

add_executable(${Target_tooLinklibTest}
	${Target_tooLinklibTest_SOURCES}
	${Target_tooLinklib_INCS}
)

if (WIN32)
set_property(TARGET ${Target_tooLinklibTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${Target_tooLinklibTest} PROPERTIES DEBUG_POSTFIX "d")

target_include_directories(${Target_tooLinklibTest} PRIVATE include)
target_include_directories(${Target_tooLinklibTest} PRIVATE ../tooBuildEnv/include)

target_link_libraries(${Target_tooLinklibTest} PRIVATE gtest_main)


add_custom_command(
  TARGET ${Target_tooLinklibTest} POST_BUILD
  COMMAND ${Target_tooLinklibTest}
)
