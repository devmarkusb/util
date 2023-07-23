cmake_minimum_required(VERSION 3.14)

set(BENCHMARK_ENABLE_TESTING OFF)

include(FetchContent)

FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG main
        GIT_SHALLOW  ON
        GIT_PROGRESS ON
) # need master for benchmark::benchmark

FetchContent_MakeAvailable(googlebenchmark)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(UL_google_benchmark_compile_options
            -Wno-zero-as-null-pointer-constant -Wno-sign-conversion -Wno-shift-sign-overflow
            -Wno-missing-variable-declarations -Wno-missing-field-initializers -Wno-used-but-marked-unused
            -Wno-disabled-macro-expansion -Wno-missing-prototypes
            -Wno-extra-semi-stmt -Wno-comma)
    if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 10)
        list(APPEND UL_google_benchmark_compile_options -Wno-implicit-int-float-conversion)
    endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(UL_google_benchmark_compile_options
            /wd4389)
else ()
    set(UL_google_benchmark_compile_options
            -Wno-conversion)
endif ()

target_compile_options(benchmark PUBLIC ${UL_google_benchmark_compile_options})
