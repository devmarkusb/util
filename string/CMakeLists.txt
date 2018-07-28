cmake_minimum_required(VERSION 3.4.1)

project(tooString VERSION 0.1.0.0)
set(Target_tooString ${PROJECT_NAME})

too_include(defaults.cmake)

add_subdirectory(sdks)

file(GLOB_RECURSE Target_tooString_INCS "include/*.h*")

add_library(${Target_tooString} STATIC ${Target_tooString_INCS})
set_target_properties(${Target_tooString} PROPERTIES LINKER_LANGUAGE CXX)

set_target_properties(${Target_tooString} PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(${Target_tooString} PUBLIC ${tooBasicCodeSupport_INCLUDE_DIRS})
target_include_directories(${Target_tooString} PUBLIC ${utf8cpp_INCLUDE_DIRS})

target_link_libraries(${Target_tooString}) # tooBasicCodeSupport utf8cpp

set(${Target_tooString}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${utf8cpp_INCLUDE_DIRS}
    ${tooBasicCodeSupport_INCLUDE_DIRS}
    CACHE INTERNAL "${Target_tooString}: include directories" FORCE)


################################################################################################

if (NOT TOO_BUILD_UNITTESTS)
    return ()
endif ()

set(Target_tooStringTest ${PROJECT_NAME}Test)

file(GLOB_RECURSE Target_tooStringTest_SOURCES "include/toolib/*.test.cpp")

add_executable(${Target_tooStringTest}
	${Target_tooStringTest_SOURCES}
	${Target_tooString_INCS}
)

if (WIN32)
    set_property(TARGET ${Target_tooStringTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${Target_tooStringTest} PROPERTIES DEBUG_POSTFIX "d")

target_include_directories(${Target_tooStringTest} PRIVATE ${tooString_INCLUDE_DIRS})

target_link_libraries(${Target_tooStringTest} PRIVATE gtest_main) #${Target_tooString}

set(do_auto_test    ON)

if (do_auto_test)
add_custom_command(
  TARGET ${Target_tooStringTest} POST_BUILD
  COMMAND ${Target_tooStringTest}
)
endif ()
