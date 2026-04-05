# Provides switches and tools to run diagnostics on your code.
# *Warning* Please note that this is work in progress. It doesn't contain anything really usable at the moment.
#

include_guard(DIRECTORY)

option(MB_UL_COVERAGE "coverage" OFF)

if(MB_DEVENV_USING_LIBCPP)
    option(
        MB_UL_LIBCPP_DEBUG
        "enables the _LIBCPP_DEBUG mode, prerequisite: compiler being built with LIBCXX_ENABLE_DEBUG_MODE_SUPPORT"
        OFF
    )
endif()

if(MB_UL_COVERAGE)
    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(
            FATAL_ERROR
            "MB_UL_COVERAGE only working for debug builds, at least non-optimized -O0"
        )
    endif()
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(CMAKE_MODULE_PATH
            ${CMAKE_MODULE_PATH}
            "${CMAKE_CURRENT_LIST_DIR}/3rdparty/bilke/cmake-modules"
            CACHE STRING
            ""
            FORCE
        )
        #include(CodeCoverage)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        # also not working: -fcoverage-mapping -fprofile-instr-generate
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -coverage")
    else()
        message(FATAL_ERROR "MB_UL_COVERAGE not implemented for this compiler")
    endif()
    if(
        "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"
        OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"
    )
        find_program(LCOV lcov REQUIRED)
        find_program(GENHTML genhtml REQUIRED)

        # add coverage target
        add_custom_target(
            coverage
            # gather data
            COMMAND ${LCOV} --directory . --capture --output-file coverage.info
            # generate report
            COMMAND ${GENHTML} --demangle-cpp -o coverage coverage.info
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
    endif()
endif()

if(MB_UL_LIBCPP_DEBUG)
    add_definitions(-D_LIBCPP_DEBUG=1)
endif()
