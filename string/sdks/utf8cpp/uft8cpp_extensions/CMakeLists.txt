cmake_minimum_required(VERSION 3.4.1)

project(utf8cpp VERSION 0.1.0.0)
set(TargetUtf8cpp ${PROJECT_NAME})

include(${TOO_CMAKE_INC_DEFAULTS})

file(GLOB_RECURSE TargetUtf8cpp_INCS "../source/*.h")

add_library(${TargetUtf8cpp} STATIC ${TargetUtf8cpp_INCS})
set_target_properties(${TargetUtf8cpp} PROPERTIES LINKER_LANGUAGE CXX)

set_target_properties(${TargetUtf8cpp} PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(${TargetUtf8cpp})

set(${TargetUtf8cpp}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/../source
    CACHE INTERNAL "${TargetUtf8cpp}: include directories" FORCE)


################################################################################################

if (NOT TOO_BUILD_UNITTESTS)
    return ()
endif ()

set(TargetUtf8cppTest ${PROJECT_NAME}Test)

file(GLOB_RECURSE TargetUtf8cppTest_SRC_INCS "test/*.h")
file(GLOB_RECURSE TargetUtf8cppTest_SOURCES "test/*.test.cpp")

add_executable(${TargetUtf8cppTest}
	${TargetUtf8cppTest_SOURCES}
	${TargetUtf8cpp_INCS}
)

if (WIN32)
set_property(TARGET ${TargetUtf8cppTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${TargetUtf8cppTest} PROPERTIES DEBUG_POSTFIX "d")

target_include_directories(${TargetUtf8cppTest} PRIVATE ${tooBasicCodeSupport_INCLUDE_DIRS})
target_include_directories(${TargetUtf8cppTest} PRIVATE ${utf8cpp_INCLUDE_DIRS})

target_link_libraries(${TargetUtf8cppTest} PRIVATE gtest_main) #tooBasicCodeSupport ${TargetUtf8cpp}


set(do_auto_test	ON)

if (do_auto_test)
add_custom_command(
  TARGET ${TargetUtf8cppTest} POST_BUILD
  COMMAND ${TargetUtf8cppTest}
)
endif ()
