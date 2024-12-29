cmake_minimum_required(VERSION 3.14)

find_library(benchmark_LIBRARIES NAMES libbenchmark.a benchmark)
if (benchmark_LIBRARIES)
    add_library(benchmark INTERFACE)
    target_link_libraries(benchmark INTERFACE ${benchmark_LIBRARIES})
    find_library(benchmark_main_LIBRARIES NAMES libbenchmark_main.a benchmark_main)
    add_library(benchmark_main INTERFACE)
    target_link_libraries(benchmark_main INTERFACE ${benchmark_main_LIBRARIES} benchmark)
    return()
endif()
message(STATUS "google benchmark not found, building from source")

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
            -Wno-extra-semi-stmt -Wno-comma
            -Wno-format-nonliteral
            -Wno-unsafe-buffer-usage)
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
if (${UL_CPP_STD_LIB} STREQUAL "libc++")
    add_definitions(-DBENCHMARK_USE_LIBCXX=ON)
endif ()
