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

set(Target_tooMem ${PROJECT_NAME})

include(defaults.cmake)

file(GLOB_RECURSE Target_tooMem_INCS "include/*.h")

add_library(${Target_tooMem} STATIC ${Target_tooMem_INCS})
set_target_properties(${Target_tooMem} PROPERTIES LINKER_LANGUAGE CXX)

set_target_properties(${Target_tooMem} PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(${Target_tooMem} PUBLIC ${tooBasicCodeSupport_INCLUDE_DIRS})

target_link_libraries(${Target_tooMem} tooBasicCodeSupport)

set(${Target_tooMem}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${tooBasicCodeSupport_INCLUDE_DIRS}
    CACHE INTERNAL "${Target_tooMem}: include directories" FORCE)


################################################################################################

set(Target_tooMemTest ${PROJECT_NAME}Test)

file(GLOB_RECURSE Target_tooMemTest_SOURCES "include/Toolib/*.test.cpp")

add_executable(${Target_tooMemTest}
	${Target_tooMemTest_SOURCES}
	${Target_tooMem_INCS}
)

if (WIN32)
set_property(TARGET ${Target_tooMemTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${Target_tooMemTest} PROPERTIES DEBUG_POSTFIX "d")

target_include_directories(${Target_tooMemTest} PRIVATE ${tooMem_INCLUDE_DIRS})

target_link_libraries(${Target_tooMemTest} PRIVATE gtest_main) #${Target_tooMem}


add_custom_command(
  TARGET ${Target_tooMemTest} POST_BUILD
  COMMAND ${Target_tooMemTest}
)
