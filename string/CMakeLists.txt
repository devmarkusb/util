cmake_minimum_required(VERSION 3.4.1)

project(tooString VERSION 0.1.0.0)
set(Target_tooString ${PROJECT_NAME})

add_subdirectory(sdks)

file(GLOB_RECURSE Target_tooString_INCS "include/*.h*")

add_library(${Target_tooString} STATIC ${Target_tooString_INCS})

target_include_directories(${Target_tooString} PUBLIC ${tooBasicCodeSupport_INCLUDE_DIRS})
target_include_directories(${Target_tooString} PUBLIC ${utf8cpp_INCLUDE_DIRS})

target_link_libraries(${Target_tooString}) # tooBasicCodeSupport utf8cpp

too_set_target_defaults(${Target_tooString})

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

target_include_directories(${Target_tooStringTest} PRIVATE ${tooString_INCLUDE_DIRS})

target_link_libraries(${Target_tooStringTest} PRIVATE gtest_main) #${Target_tooString}

too_set_target_defaults(${Target_tooStringTest})

too_run_target_postbuild(${Target_tooStringTest})
