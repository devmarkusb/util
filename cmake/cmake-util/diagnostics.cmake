# Provides switches and tools to run diagnostics on your code.
# *Warning* Please note that this is work in progress. It doesn't contain anything really usable at the moment.
#
# MB_UL_ALL_WARNINGS is defined in defaults.cmake and applied per target via mb_ul_set_target_warnings().

include_guard(DIRECTORY)

option(MB_UL_COVERAGE "coverage" OFF)

# sanitizers, mutually exclusive for now
option(MB_UL_ADDRESS_SAN "address sanitizer (asan)" OFF)
option(MB_UL_THREAD_SAN "thread sanitizer" OFF)
option(MB_UL_MEMORY_SAN "memory sanitizer" OFF)
option(MB_UL_UNDEF_SAN "undefined behavior sanitizer" OFF)
option(
    MB_UL_ENABLE_STACK_PROTECTION
    "enables compiler settings to protect stack corruptions"
    OFF
)

if("${MB_UL_CPP_STD_LIB}" STREQUAL "libc++")
    option(
        MB_UL_LIBCPP_DEBUG
        "enables the _LIBCPP_DEBUG mode, prerequisite: compiler being built with LIBCXX_ENABLE_DEBUG_MODE_SUPPORT"
        OFF
    )
endif()

if(MB_UL_ADDRESS_SAN)
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # nothing yet
        message(
            FATAL_ERROR
            "MB_UL_ADDRESS_SAN not implemented for GNU compiler"
        )
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        # nothing yet
        message(
            FATAL_ERROR
            "MB_UL_ADDRESS_SAN not implemented for MSVC compiler"
        )
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS
            "${CMAKE_CXX_FLAGS} -fsanitize=address \
-O1 -g -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-common"
        )
        set(CMAKE_EXE_LINKER_FLAGS
            "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address"
        )
    endif()
elseif(MB_UL_THREAD_SAN)
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # nothing yet
        message(FATAL_ERROR "MB_UL_THREAD_SAN not implemented for GNU compiler")
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        # nothing yet
        message(
            FATAL_ERROR
            "MB_UL_THREAD_SAN not implemented for MSVC compiler"
        )
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
        set(CMAKE_EXE_LINKER_FLAGS
            "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread"
        )
    endif()
elseif(MB_UL_MEMORY_SAN)
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # nothing yet
        message(FATAL_ERROR "MB_UL_MEMORY_SAN not implemented for GNU compiler")
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        # nothing yet
        message(
            FATAL_ERROR
            "MB_UL_MEMORY_SAN not implemented for MSVC compiler"
        )
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory")
        set(CMAKE_EXE_LINKER_FLAGS
            "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=memory"
        )
    endif()
elseif(MB_UL_UNDEF_SAN)
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # nothing yet
        message(FATAL_ERROR "MB_UL_UNDEF_SAN not implemented for GNU compiler")
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        # nothing yet
        message(FATAL_ERROR "MB_UL_UNDEF_SAN not implemented for MSVC compiler")
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
        set(CMAKE_EXE_LINKER_FLAGS
            "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined"
        )
    endif()
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

if(MB_UL_ENABLE_STACK_PROTECTION)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstack-protector-all")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector-all")
endif(MB_UL_ENABLE_STACK_PROTECTION)
