# file to include for OpenMP settings

set(cpp_compile_options)
set(c_compile_options)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(cpp_compile_options
            -fopenmp)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(cpp_compile_options
            -fopenmp)
endif ()

add_compile_options(
        "$<$<COMPILE_LANGUAGE:C>:${c_compile_options}>"
        "$<$<COMPILE_LANGUAGE:CXX>:${cpp_compile_options}>")
