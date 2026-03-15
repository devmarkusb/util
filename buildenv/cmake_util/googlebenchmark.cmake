cmake_minimum_required(VERSION 3.14)

if(ANDROID)
    return()
endif()

find_library(benchmark_LIBRARIES NAMES libbenchmark.a benchmark)
if(benchmark_LIBRARIES)
    add_library(benchmark INTERFACE)
    target_link_libraries(benchmark INTERFACE ${benchmark_LIBRARIES})
    find_library(
        benchmark_main_LIBRARIES
        NAMES libbenchmark_main.a benchmark_main
    )
    add_library(benchmark_main INTERFACE)
    target_link_libraries(
        benchmark_main
        INTERFACE ${benchmark_main_LIBRARIES} benchmark
    )
    return()
endif()
message(STATUS "google benchmark not found, building from source")

set(BENCHMARK_ENABLE_TESTING OFF)

# Google Benchmark's regex backend detection uses try_compile(), which inherits
# CMAKE_CXX_FLAGS. With -Werror, -Wall, -Wextra, or other strict flags, those
# checks can fail due to warnings in the test code or system headers, leading to
# "Failed to determine the source files for the regular expression backend".
# Clear all CXX flags for the dependency's configure and build so detection and
# compilation succeed on all CI platforms (Linux gcc/clang, macOS, Windows).
set(UL_SAVED_CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(UL_SAVED_CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
set(UL_SAVED_CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
set(UL_SAVED_CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
set(UL_SAVED_CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL}")
set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_CXX_FLAGS_RELEASE "")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "")
set(CMAKE_CXX_FLAGS_MINSIZEREL "")

include(FetchContent)

# Mitigate transient "Failed to connect to github.com" (VPN/firewall/DNS/network):
# pre-clone with retries so a single flaky attempt doesn't fail the configure.
set(UL_benchmark_src "${CMAKE_BINARY_DIR}/_deps/googlebenchmark-src")
if(EXISTS "${UL_benchmark_src}/CMakeLists.txt")
    set(FETCHCONTENT_SOURCE_DIR_GOOGLEBENCHMARK "${UL_benchmark_src}" CACHE PATH "")
else()
    find_package(Git REQUIRED QUIET)
    set(UL_clone_result 1)
    set(UL_attempt 0)
    while(UL_attempt LESS 5)
        math(EXPR UL_attempt_1 "${UL_attempt} + 1")
        message(STATUS "Fetching google/benchmark (attempt ${UL_attempt_1}/5)...")
        file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/_deps")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} clone --depth 1 -b main
                https://github.com/google/benchmark.git googlebenchmark-src
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/_deps"
            RESULT_VARIABLE UL_clone_result
            OUTPUT_QUIET
            ERROR_QUIET
        )
        if(UL_clone_result EQUAL 0)
            break()
        endif()
        file(REMOVE_RECURSE "${UL_benchmark_src}")
        math(EXPR UL_attempt "${UL_attempt} + 1")
        if(UL_attempt LESS 5)
            message(STATUS "Clone failed, retrying in 10 seconds...")
            execute_process(COMMAND ${CMAKE_COMMAND} -E sleep 10)
        endif()
    endwhile()
    if(NOT UL_clone_result EQUAL 0)
        message(FATAL_ERROR
            "Failed to clone google/benchmark after 5 attempts. "
            "Check network, VPN, firewall, or install system libbenchmark-dev.")
    endif()
    set(FETCHCONTENT_SOURCE_DIR_GOOGLEBENCHMARK "${UL_benchmark_src}" CACHE PATH "")
endif()

FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG main
    GIT_SHALLOW ON
    GIT_PROGRESS ON
) # need master for benchmark::benchmark

FetchContent_MakeAvailable(googlebenchmark)

set(CMAKE_CXX_FLAGS "${UL_SAVED_CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${UL_SAVED_CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "${UL_SAVED_CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${UL_SAVED_CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${UL_SAVED_CMAKE_CXX_FLAGS_MINSIZEREL}")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(UL_google_benchmark_compile_options
        -Wno-zero-as-null-pointer-constant
        -Wno-sign-conversion
        -Wno-shift-sign-overflow
        -Wno-missing-variable-declarations
        -Wno-missing-field-initializers
        -Wno-used-but-marked-unused
        -Wno-disabled-macro-expansion
        -Wno-missing-prototypes
        -Wno-extra-semi-stmt
        -Wno-comma
        -Wno-format-nonliteral
        -Wno-unsafe-buffer-usage
    )
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 10)
        list(
            APPEND UL_google_benchmark_compile_options
            -Wno-implicit-int-float-conversion
        )
    endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(UL_google_benchmark_compile_options /wd4389)
else()
    set(UL_google_benchmark_compile_options -Wno-conversion)
endif()

target_compile_options(benchmark PUBLIC ${UL_google_benchmark_compile_options})
if("${UL_CPP_STD_LIB}" STREQUAL "libc++")
    add_definitions(-DBENCHMARK_USE_LIBCXX=ON)
endif()
