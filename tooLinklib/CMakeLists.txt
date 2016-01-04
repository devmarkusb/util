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
set(Target_tooLinklib "tooLinklib")

include(defaults.cmake)

file(GLOB_RECURSE Target_tooLinklib_INCS "include/*.h")
file(GLOB_RECURSE Target_tooLinklib_SRC_INCS "src/*.h")

#[[
set(Target_tooLinklib_SOURCES
    "src/non-empty folder.cpp"
)

add_library(${Target_tooLinklib} SHARED
	${Target_tooLinklib_SOURCES}
	${Target_tooLinklib_INCS}
	${Target_tooLinklib_SRC_INCS}
)

if (WIN32)
set_property(TARGET ${Target_tooLinklib} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

# dll export define
set_property(TARGET ${Target_tooLinklib} APPEND PROPERTY COMPILE_DEFINITIONS TOOBUILDENV_LIBRARY)

set_target_properties(${Target_tooLinklib} PROPERTIES DEBUG_POSTFIX "d")

target_include_directories(${Target_tooLinklib} PRIVATE .)
target_include_directories(${Target_tooLinklib} PRIVATE include)
target_include_directories(${Target_tooLinklib} PRIVATE src)
]]

get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
set(tooLinklib_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
#set(tooLinklib_LIBRARY_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/lib PARENT_SCOPE)
else()
set(tooLinklib_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
#set(tooLinklib_LIBRARY_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/lib)
endif()


################################################################################################

set(Target_tooLinklibTest "tooLinklibTest")

file(GLOB_RECURSE Target_tooLinklibTest_SRC_INCS "test/*.h")

set(Target_tooLinklibTest_SOURCES
	"test/non-empty folder.test.cpp"
)

add_executable(${Target_tooLinklibTest}
	${Target_tooLinklibTest_SOURCES}
	${Target_tooLinklib_INCS}
	${Target_tooLinklibTest_SRC_INCS}
)

if (WIN32)
set_property(TARGET ${Target_tooLinklibTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${Target_tooLinklibTest} PROPERTIES DEBUG_POSTFIX "d")

set(GTEST_ROOT "c:/projects/externlibs/gtest-1.7.0")
set(GTEST_MSVC_SEARCH   MD)
find_package(GTest 1.7.0)

target_include_directories(${Target_tooLinklibTest} PRIVATE ${GTEST_INCLUDE_DIRS})
target_include_directories(${Target_tooLinklibTest} PRIVATE ${tooLinklib_INCLUDE_DIRS})

target_link_libraries(${Target_tooLinklibTest} PRIVATE ${GTEST_BOTH_LIBRARIES})
#target_link_libraries(${Target_tooLinklibTest} PRIVATE ${Target_tooLinklib})


add_custom_command(
  TARGET ${Target_tooLinklibTest} POST_BUILD
  COMMAND ${Target_tooLinklibTest}
)
