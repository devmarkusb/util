cmake_minimum_required(VERSION 3.4.1)

project(tooBasicCodeSupport VERSION 0.1.0.0)

set(Target_tooBasicCodeSupport ${PROJECT_NAME})

file(GLOB_RECURSE Target_tooBasicCodeSupport_INCS "include/*.h")

add_library(${Target_tooBasicCodeSupport} STATIC ${Target_tooBasicCodeSupport_INCS})

target_include_directories(${Target_tooBasicCodeSupport} PUBLIC ${tooBuildEnv_INCLUDE_DIRS})

target_link_libraries(${Target_tooBasicCodeSupport}) # tooBuildEnv

too_set_target_defaults(${Target_tooBasicCodeSupport})

set(${Target_tooBasicCodeSupport}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${tooBuildEnv_INCLUDE_DIRS}
    CACHE INTERNAL "${Target_tooBasicCodeSupport}: include directories" FORCE)


################################################################################################

if (NOT TOO_BUILD_UNITTESTS)
    return ()
endif ()

set(Target_tooBasicCodeSupportTest ${PROJECT_NAME}Test)

file(GLOB_RECURSE Target_tooBasicCodeSupportTest_SOURCES "include/toolib/*.test.cpp")

add_executable(${Target_tooBasicCodeSupportTest}
    ${Target_tooBasicCodeSupportTest_SOURCES}
    ${Target_tooBasicCodeSupport_INCS}
)

target_include_directories(${Target_tooBasicCodeSupportTest} PRIVATE ${tooBasicCodeSupport_INCLUDE_DIRS})

target_link_libraries(${Target_tooBasicCodeSupportTest} PRIVATE gtest_main) #${Target_tooBasicCodeSupport}

too_set_target_defaults(${Target_tooBasicCodeSupportTest})

too_run_target_postbuild(${Target_tooBasicCodeSupportTest})
