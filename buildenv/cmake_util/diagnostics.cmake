# Provides switches and tools to run diagnostics on your code.
# *Warning* Please note that this is work in progress. It doesn't contain anything really usable at the moment.

include_guard(GLOBAL)

option(UL_ALL_WARNINGS "all possible warnings, switch on and adapt details if you want to see \
a maximum of warnings" OFF)

option(UL_COVERAGE "coverage" OFF)

# sanitizers, mutually exclusive for now
option(UL_ADDRESS_SAN "address sanitizer (asan)" OFF)
option(UL_THREAD_SAN "thread sanitizer" OFF)
option(UL_MEMORY_SAN "memory sanitizer" OFF)
option(UL_UNDEF_SAN "undefined behavior sanitizer" OFF)

if (UL_ADDRESS_SAN)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # nothing yet
        message(FATAL_ERROR "UL_ADDRESS_SAN not implemented for GNU compiler")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        # nothing yet
        message(FATAL_ERROR "UL_ADDRESS_SAN not implemented for MSVC compiler")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
    endif ()
elseif (UL_THREAD_SAN)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # nothing yet
        message(FATAL_ERROR "UL_THREAD_SAN not implemented for GNU compiler")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        # nothing yet
        message(FATAL_ERROR "UL_THREAD_SAN not implemented for MSVC compiler")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread")
    endif ()
elseif (UL_MEMORY_SAN)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # nothing yet
        message(FATAL_ERROR "UL_MEMORY_SAN not implemented for GNU compiler")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        # nothing yet
        message(FATAL_ERROR "UL_MEMORY_SAN not implemented for MSVC compiler")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=memory")
    endif ()
elseif (UL_UNDEF_SAN)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # nothing yet
        message(FATAL_ERROR "UL_UNDEF_SAN not implemented for GNU compiler")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        # nothing yet
        message(FATAL_ERROR "UL_UNDEF_SAN not implemented for MSVC compiler")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
    endif ()
endif ()

if (UL_ALL_WARNINGS)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # no more warnings here
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        # not implemented
        message(FATAL_ERROR "UL_ALL_WARNINGS not implemented for MSVC compiler")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        # should include -Wconversion
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything")
        # category of must-be-blacklisted ones
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-member-function -Wno-c++98-compat -Wno-deprecated \
-Wno-weak-vtables -Wno-shadow-field-in-constructor -Wno-undef -Wno-c++98-compat-pedantic -Wno-double-promotion \
-Wmissing-prototypes")
        # category of could-be-useful ones
        # can be interesting, but appears a lot in Qt moc_ files, so...
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-redundant-parens")
        # same here
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-undefined-reinterpret-cast")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-global-constructors -Wno-exit-time-destructors")
        # a no default warning would be more useful
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-switch-enum -Wno-covered-switch-default")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-noreturn")
        # this could be useful to turn on locally (at most) to find out about padding size
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-padded")
    endif ()
endif ()

if (UL_COVERAGE)
    if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(FATAL_ERROR "UL_COVERAGE only working for debug builds, at least non-optimized -O0")
    endif ()
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/3rdparty/bilke/cmake-modules"
                CACHE STRING "" FORCE)
        #include(CodeCoverage)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -coverage")

        find_program(LCOV lcov REQUIRED)
        find_program(GENHTML genhtml REQUIRED)

        # add coverage target
        add_custom_target(coverage
            # gather data
            COMMAND ${LCOV} --directory . --capture --output-file coverage.info
            # generate report
            COMMAND ${GENHTML} --demangle-cpp -o coverage coverage.info
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    else ()
        message(FATAL_ERROR "UL_COVERAGE not implemented for this compiler")
    endif ()
endif()
