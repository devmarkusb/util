# Provides switches and tools to run diagnostics on your code.
# *Warning* Please note that this is work in progress. It doesn't contain anything really usable at the moment.

# initialize
set(cpp_compile_options)
set(c_compile_options)

# sanitizers
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(cpp_compile_options
#            -fsanitize=address -static-libasan
    )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # nothing yet
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(cpp_compile_options
#            -fsanitize=address -static-libasan
            )
endif ()


### finally setting compile options ###

add_compile_options(
        "$<$<COMPILE_LANGUAGE:C>:${c_compile_options}>"
        "$<$<COMPILE_LANGUAGE:CXX>:${cpp_compile_options}>")
