# Should be included into your CMakeLists.txt with 'add_subdirectory' even before the 'project' directive.

cmake_minimum_required(VERSION 3.4.1)

project(tooBuildEnv VERSION 0.1.0.0)

set(Target_tooBuildEnv ${PROJECT_NAME})

include(cmake_util/defaults.cmake)

file(GLOB_RECURSE Target_tooBuildEnv_INCS "include/*.h")

add_library(${Target_tooBuildEnv} STATIC ${Target_tooBuildEnv_INCS})
set_target_properties(${Target_tooBuildEnv} PROPERTIES LINKER_LANGUAGE CXX)

set_target_properties(${Target_tooBuildEnv} PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(${Target_tooBuildEnv})

set(${Target_tooBuildEnv}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL "${Target_tooBuildEnv}: include directories" FORCE)

include(cmake_util.cmake)


################################################################################################

if (NOT TOO_BUILD_UNITTESTS)
    return ()
endif ()

set(Target_tooBuildEnvTest ${PROJECT_NAME}Test)

file(GLOB_RECURSE Target_tooBuildEnvTest_SOURCES "include/Toolib/*.test.cpp")

add_executable(${Target_tooBuildEnvTest}
	${Target_tooBuildEnvTest_SOURCES}
	${Target_tooBuildEnv_INCS}
)

if (WIN32)
    set_property(TARGET ${Target_tooBuildEnvTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${Target_tooBuildEnvTest} PROPERTIES DEBUG_POSTFIX "d")

target_include_directories(${Target_tooBuildEnvTest} PRIVATE ${tooBuildEnv_INCLUDE_DIRS})

target_link_libraries(${Target_tooBuildEnvTest} PRIVATE gtest_main) #${Target_tooBuildEnv}


add_custom_command(
  TARGET ${Target_tooBuildEnvTest} POST_BUILD
  COMMAND ${Target_tooBuildEnvTest}
)
