cmake_minimum_required(VERSION 3.4.1)

project(tooMem VERSION 0.1.0.0)

set(Target_tooMem ${PROJECT_NAME})

file(GLOB_RECURSE Target_tooMem_INCS "include/*.h")

#add_library(${Target_tooMem} STATIC ${Target_tooMem_INCS})

#target_include_directories(${Target_tooMem} PUBLIC ${tooBasicCodeSupport_INCLUDE_DIRS})

#target_link_libraries(${Target_tooMem}) # tooBasicCodeSupport

#too_set_target_defaults(${Target_tooMem})

set(${Target_tooMem}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${tooBasicCodeSupport_INCLUDE_DIRS}
    CACHE INTERNAL "${Target_tooMem}: include directories" FORCE)


################################################################################################

if (NOT TOO_BUILD_UNITTESTS)
    return ()
endif ()

set(Target_tooMemTest ${PROJECT_NAME}Test)

file(GLOB_RECURSE Target_tooMemTest_SOURCES "include/toolib/*.test.cpp")
list(REMOVE_ITEM Target_tooMemTest_SOURCES "${CMAKE_CURRENT_LIST_DIR}/include/toolib/mem/new_statistics.test.cpp")
add_executable(${Target_tooMemTest}
    ${Target_tooMemTest_SOURCES}
    ${Target_tooMem_INCS}
)

too_add_test(${Target_tooMemTest})

target_include_directories(${Target_tooMemTest} PRIVATE ${tooMem_INCLUDE_DIRS})

target_link_libraries(${Target_tooMemTest} PRIVATE gtest_main) #${Target_tooMem}

too_set_target_defaults(${Target_tooMemTest})

too_run_target_postbuild(${Target_tooMemTest})


add_executable(${Target_tooMemTest}_new_statistics
        include/toolib/mem/new_statistics.test.cpp
        ${Target_tooMem_INCS}
)

too_add_test(${Target_tooMemTest}_new_statistics)

target_include_directories(${Target_tooMemTest}_new_statistics PRIVATE ${tooMem_INCLUDE_DIRS})

too_set_target_defaults(${Target_tooMemTest}_new_statistics)

too_run_target_postbuild(${Target_tooMemTest}_new_statistics)
