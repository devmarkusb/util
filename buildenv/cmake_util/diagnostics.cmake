# Provides switches and tools to run diagnostics on your code.
# *Warning* Please note that this is work in progress. It doesn't contain anything really usable at the moment.

# initialize
set(cpp_compile_options)
set(c_compile_options)

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
        set(cpp_compile_options -fsanitize=address)
        set(cpp_link_options -fsanitize=address)
    endif ()
elseif (UL_THREAD_SAN)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # nothing yet
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        # nothing yet
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(cpp_compile_options -fsanitize=thread)
        set(cpp_link_options -fsanitize=thread)
    endif ()
elseif (UL_MEMORY_SAN)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # nothing yet
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        # nothing yet
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(cpp_compile_options -fsanitize=memory)
        set(cpp_link_options -fsanitize=memory)
    endif ()
elseif (UL_UNDEF_SAN)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # nothing yet
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        # nothing yet
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(cpp_compile_options -fsanitize=undefined)
        set(cpp_link_options -fsanitize=undefined)
    endif ()
endif ()


### finally setting compile options ###

add_compile_options(
        "$<$<COMPILE_LANGUAGE:C>:${c_compile_options}>"
        "$<$<COMPILE_LANGUAGE:CXX>:${cpp_compile_options}>")

add_link_options(
        "$<$<COMPILE_LANGUAGE:C>:${c_link_options}>"
        "$<$<COMPILE_LANGUAGE:CXX>:${cpp_link_options}>")
