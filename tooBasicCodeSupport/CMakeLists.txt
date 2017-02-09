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

set(Target_tooBasicCodeSupport ${PROJECT_NAME})

include(defaults.cmake)

file(GLOB_RECURSE Target_tooBasicCodeSupport_INCS "include/*.h")

add_library(${Target_tooBasicCodeSupport} STATIC ${Target_tooBasicCodeSupport_INCS})
set_target_properties(${Target_tooBasicCodeSupport} PROPERTIES LINKER_LANGUAGE CXX)

set_target_properties(${Target_tooBasicCodeSupport} PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(${Target_tooBasicCodeSupport} PUBLIC ${tooBuildEnv_INCLUDE_DIRS})

target_link_libraries(${Target_tooBasicCodeSupport} tooBuildEnv)

set(${Target_tooBasicCodeSupport}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${tooBuildEnv_INCLUDE_DIRS}
    CACHE INTERNAL "${Target_tooBasicCodeSupport}: include directories" FORCE)


################################################################################################

set(Target_tooBasicCodeSupportTest ${PROJECT_NAME}Test)

file(GLOB_RECURSE Target_tooBasicCodeSupportTest_SOURCES "include/Toolib/*.test.cpp")

add_executable(${Target_tooBasicCodeSupportTest}
	${Target_tooBasicCodeSupportTest_SOURCES}
	${Target_tooBasicCodeSupport_INCS}
)

if (WIN32)
set_property(TARGET ${Target_tooBasicCodeSupportTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${Target_tooBasicCodeSupportTest} PROPERTIES DEBUG_POSTFIX "d")

target_include_directories(${Target_tooBasicCodeSupportTest} PRIVATE ${tooBasicCodeSupport_INCLUDE_DIRS})

target_link_libraries(${Target_tooBasicCodeSupportTest} PRIVATE gtest_main) #${Target_tooBasicCodeSupport}


set(do_auto_test	ON)

if (do_auto_test)
add_custom_command(
  TARGET ${Target_tooBasicCodeSupportTest} POST_BUILD
  COMMAND ${Target_tooBasicCodeSupportTest}
)
endif ()
