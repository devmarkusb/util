# link matplotlib_cpp
set(MATPLOTLIB_CPP_GENERATE_EXAMPLES OFF CACHE BOOL "example targets" FORCE)

FetchContent_Declare(
     matplotlib-cpp
     GIT_REPOSITORY https://github.com/devmarkusb/matplotlib-cpp
     GIT_TAG        d6d64c9f
)

FetchContent_MakeAvailable(matplotlib-cpp)

set(matplotlib-cpp_INCLUDE_DIRS
        ${matplotlib-cpp_SOURCE_DIR}
        CACHE INTERNAL "matplotlib-cpp: include directories" FORCE)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(UL_matplotlib_cpp_compile_options
            -Wno-sign-conversion
            -Wno-float-conversion -Wno-implicit-float-conversion
            -Wno-unused-parameter
            -Wno-sign-compare -Wno-shorten-64-to-32 -Wno-missing-prototypes)
    if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 10)
        list(APPEND UL_matplotlib_cpp_compile_options -Wno-implicit-int-float-conversion)
    endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(UL_matplotlib_cpp_compile_options)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(UL_matplotlib_cpp_compile_options
            -Wno-conversion -Wno-sign-compare -Wno-unused-parameter)
endif ()
target_compile_options(matplotlib_cpp INTERFACE ${UL_matplotlib_cpp_compile_options})
