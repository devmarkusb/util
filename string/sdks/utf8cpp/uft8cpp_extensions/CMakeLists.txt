cmake_minimum_required(VERSION 3.4.1)

project(utf8cpp VERSION 0.1.0.0)
set(TargetUtf8cpp ${PROJECT_NAME})

file(GLOB_RECURSE TargetUtf8cpp_INCS "../source/*.h")

#add_library(${TargetUtf8cpp} STATIC ${TargetUtf8cpp_INCS})

#target_link_libraries(${TargetUtf8cpp})

#too_set_target_defaults(${TargetUtf8cpp})

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

target_include_directories(${TargetUtf8cppTest} PRIVATE ${PROJECT_SOURCE_DIR}/../../../include)
target_include_directories(${TargetUtf8cppTest} PRIVATE ${tooBasicCodeSupport_INCLUDE_DIRS})
target_include_directories(${TargetUtf8cppTest} PRIVATE ${utf8cpp_INCLUDE_DIRS})

target_link_libraries(${TargetUtf8cppTest} PRIVATE gtest_main) #tooBasicCodeSupport ${TargetUtf8cpp}

too_set_target_defaults(${TargetUtf8cppTest})

too_run_target_postbuild(${TargetUtf8cppTest})
