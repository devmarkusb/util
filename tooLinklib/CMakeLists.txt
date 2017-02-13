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

include(${TOO_CMAKE_INC_DEFAULTS})

file(GLOB_RECURSE Target_tooLinklib_INCS "include/*.h")

add_library(${Target_tooLinklib} STATIC ${Target_tooLinklib_INCS})
set_target_properties(${Target_tooLinklib} PROPERTIES LINKER_LANGUAGE CXX)

set_target_properties(${Target_tooLinklib} PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(${Target_tooLinklib} PUBLIC ${tooBuildEnv_INCLUDE_DIRS})

target_link_libraries(${Target_tooLinklib}) # tooBuildEnv

set(${Target_tooLinklib}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${tooBuildEnv_INCLUDE_DIRS}
    CACHE INTERNAL "${Target_tooLinklib}: include directories" FORCE)


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

target_include_directories(${Target_tooLinklibTest} PRIVATE ${tooLinklib_INCLUDE_DIRS})

target_link_libraries(${Target_tooLinklibTest} PRIVATE gtest_main) #${Target_tooLinklib}


add_custom_command(
  TARGET ${Target_tooLinklibTest} POST_BUILD
  COMMAND ${Target_tooLinklibTest}
)
