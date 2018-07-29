cmake_minimum_required(VERSION 3.4.1)

project(tooLinklib VERSION 0.1.0.0)

set(Target_tooLinklib ${PROJECT_NAME})

file(GLOB_RECURSE Target_tooLinklib_INCS "include/*.h")

add_library(${Target_tooLinklib} STATIC ${Target_tooLinklib_INCS})

target_include_directories(${Target_tooLinklib} PUBLIC ${tooBuildEnv_INCLUDE_DIRS})

target_link_libraries(${Target_tooLinklib}) # tooBuildEnv

too_set_target_defaults(${Target_tooLinklib})

set(${Target_tooLinklib}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${tooBuildEnv_INCLUDE_DIRS}
    CACHE INTERNAL "${Target_tooLinklib}: include directories" FORCE)


################################################################################################

if (NOT TOO_BUILD_UNITTESTS)
    return ()
endif ()

set(Target_tooLinklibTest ${PROJECT_NAME}Test)

file(GLOB_RECURSE Target_tooLinklibTest_SOURCES "include/toolib/*.test.cpp")

add_executable(${Target_tooLinklibTest}
    ${Target_tooLinklibTest_SOURCES}
    ${Target_tooLinklib_INCS}
)

too_add_test(${Target_tooLinklibTest})

target_include_directories(${Target_tooLinklibTest} PRIVATE ${tooLinklib_INCLUDE_DIRS})

target_link_libraries(${Target_tooLinklibTest} PRIVATE gtest_main) #${Target_tooLinklib}

too_set_target_defaults(${Target_tooLinklibTest})

too_run_target_postbuild(${Target_tooLinklibTest})
