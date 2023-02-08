# Provides switches and tools to run diagnostics on your code.
# *Warning* Please note that this is work in progress. It doesn't contain anything really usable at the moment.

# initialize
set(cpp_compile_options)
set(c_compile_options)

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
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        # nothing yet
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(cpp_compile_options ${cpp_compile_options} -fsanitize=address)
        set(cpp_link_options ${cpp_link_options}-fsanitize=address)
    endif ()
elseif (UL_THREAD_SAN)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # nothing yet
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        # nothing yet
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(cpp_compile_options ${cpp_compile_options} -fsanitize=thread)
        set(cpp_link_options ${cpp_link_options} -fsanitize=thread)
    endif ()
elseif (UL_MEMORY_SAN)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # nothing yet
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        # nothing yet
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(cpp_compile_options ${cpp_compile_options} -fsanitize=memory)
        set(cpp_link_options ${cpp_link_options} -fsanitize=memory)
    endif ()
elseif (UL_UNDEF_SAN)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # nothing yet
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        # nothing yet
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(cpp_compile_options ${cpp_compile_options} -fsanitize=undefined)
        set(cpp_link_options ${cpp_link_options} -fsanitize=undefined)
    endif ()
endif ()


if (UL_ALL_WARNINGS)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # partially implemented
        set(cpp_compile_options
                ${cpp_compile_options}
                -Wconversion
                )
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        # not implemented
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(cpp_compile_options
                ${cpp_compile_options}
                -Weverything # should include -Wconversion
                # category of must-be-blacklisted ones
                -Wno-unused-member-function -Wno-c++98-compat -Wno-deprecated -Wno-weak-vtables
                -Wno-shadow-field-in-constructor -Wno-undef -Wno-c++98-compat-pedantic
                -Wno-double-promotion -Wmissing-prototypes
                # category of could-be-useful ones
                -Wno-redundant-parens # can be interesting, but appears a lot in Qt moc_ files, so...
                -Wno-undefined-reinterpret-cast # same here
                -Wno-documentation # might want to turn that on - fires a lot due to misuse of multiline //!'s
                -Wno-documentation-unknown-command # investigate use of \return, \param etc., unfortunate
                -Wno-global-constructors -Wno-exit-time-destructors
                -Wno-switch-enum -Wno-covered-switch-default # a no default warning would be more useful
                -Wno-missing-noreturn
                -Wno-padded # this could be useful to turn on locally (at most) to find out about padding size
                )
    endif ()
endif ()

if (UL_COVERAGE)
    set(cpp_compile_options ${cpp_compile_options})
    set(cpp_link_options ${cpp_link_options})
    if (NOT MSVC)
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/3rdparty/bilke/cmake-modules"
                    CACHE STRING "" FORCE)
            include(CodeCoverage)
        endif ()
    endif ()
endif()

### finally setting compile options ###

add_compile_options(
        "$<$<COMPILE_LANGUAGE:C>:${c_compile_options}>"
        "$<$<COMPILE_LANGUAGE:CXX>:${cpp_compile_options}>")

add_link_options(
        "$<$<COMPILE_LANGUAGE:C>:${c_link_options}>"
        "$<$<COMPILE_LANGUAGE:CXX>:${cpp_link_options}>")
