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
set(HAVE_STD_REGEX ON CACHE BOOL "Use std::regex for benchmark" FORCE)
set(RUN_HAVE_STD_REGEX 1 CACHE STRING "Use std::regex at runtime" FORCE)

# Google Benchmark's regex backend detection uses try_compile(), which inherits
# CMAKE_CXX_FLAGS. With -Werror, -Wall, -Wextra, or other strict flags, those
# checks can fail due to warnings in the test code or system headers, leading to
# "Failed to determine the source files for the regular expression backend".
# Replace C and CXX flags with a minimal set for the dependency's configure and
# build so detection and compilation succeed on all CI platforms (Linux gcc/clang
# with sanitizers, macOS, Windows). FindThreads uses C try_compile; HAVE_STEADY_CLOCK
# and regex use C++. Use CACHE FORCE so try_compile() and the benchmark subproject
# see these flags; restore cache after so the main project keeps its flags.
# FindThreads try_compile also links an executable, so linker flags need -pthread.
set(THREADS_PREFER_PTHREAD_FLAG
    ON
    CACHE BOOL
    "Prefer -pthread for FindThreads"
    FORCE
)
set(UL_SAVED_CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(UL_SAVED_CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
set(UL_SAVED_CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
set(UL_SAVED_CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
set(UL_SAVED_CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL}")
set(UL_SAVED_CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(UL_SAVED_CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
set(UL_SAVED_CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
set(UL_SAVED_CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
set(UL_SAVED_CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL}")
set(UL_SAVED_CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
set(UL_SAVED_CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
if(MSVC)
    set(UL_BENCHMARK_MINIMAL_C_FLAGS "")
    set(UL_BENCHMARK_MINIMAL_CXX_FLAGS "/std:c++${CMAKE_CXX_STANDARD}")
    set(UL_BENCHMARK_MINIMAL_EXE_LINKER_FLAGS "")
    set(UL_BENCHMARK_MINIMAL_SHARED_LINKER_FLAGS "")
else()
    # -pthread required so FindThreads (C try_compile) and HAVE_STEADY_CLOCK (C++)
    # try_compile succeed in the benchmark subproject (e.g. clang-debug with sanitizers).
    # FindThreads try_compile links an executable, so linker flags need -pthread too.
    # Keep saved linker flags (e.g. -fuse-ld=lld from defaults.cmake): replacing the
    # cache with only -pthread breaks try_compile/link on images where the default
    # system linker is missing but lld is used.
    set(UL_BENCHMARK_MINIMAL_C_FLAGS "-pthread")
    set(UL_BENCHMARK_MINIMAL_CXX_FLAGS "-std=c++${CMAKE_CXX_STANDARD} -pthread")
    set(UL_BENCHMARK_MINIMAL_EXE_LINKER_FLAGS
        "${UL_SAVED_CMAKE_EXE_LINKER_FLAGS} -pthread"
    )
    set(UL_BENCHMARK_MINIMAL_SHARED_LINKER_FLAGS
        "${UL_SAVED_CMAKE_SHARED_LINKER_FLAGS} -pthread"
    )
endif()
set(CMAKE_C_FLAGS
    "${UL_BENCHMARK_MINIMAL_C_FLAGS}"
    CACHE STRING
    "Minimal for benchmark FindThreads"
    FORCE
)
set(CMAKE_C_FLAGS_DEBUG
    "${UL_BENCHMARK_MINIMAL_C_FLAGS}"
    CACHE STRING
    "Minimal for benchmark FindThreads"
    FORCE
)
set(CMAKE_C_FLAGS_RELEASE
    "${UL_BENCHMARK_MINIMAL_C_FLAGS}"
    CACHE STRING
    "Minimal for benchmark FindThreads"
    FORCE
)
set(CMAKE_C_FLAGS_RELWITHDEBINFO
    "${UL_BENCHMARK_MINIMAL_C_FLAGS}"
    CACHE STRING
    "Minimal for benchmark FindThreads"
    FORCE
)
set(CMAKE_C_FLAGS_MINSIZEREL
    "${UL_BENCHMARK_MINIMAL_C_FLAGS}"
    CACHE STRING
    "Minimal for benchmark FindThreads"
    FORCE
)
set(CMAKE_CXX_FLAGS
    "${UL_BENCHMARK_MINIMAL_CXX_FLAGS}"
    CACHE STRING
    "Minimal for benchmark regex detection"
    FORCE
)
set(CMAKE_CXX_FLAGS_DEBUG
    "${UL_BENCHMARK_MINIMAL_CXX_FLAGS}"
    CACHE STRING
    "Minimal for benchmark regex detection"
    FORCE
)
set(CMAKE_CXX_FLAGS_RELEASE
    "${UL_BENCHMARK_MINIMAL_CXX_FLAGS}"
    CACHE STRING
    "Minimal for benchmark regex detection"
    FORCE
)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO
    "${UL_BENCHMARK_MINIMAL_CXX_FLAGS}"
    CACHE STRING
    "Minimal for benchmark regex detection"
    FORCE
)
set(CMAKE_CXX_FLAGS_MINSIZEREL
    "${UL_BENCHMARK_MINIMAL_CXX_FLAGS}"
    CACHE STRING
    "Minimal for benchmark regex detection"
    FORCE
)
set(CMAKE_EXE_LINKER_FLAGS
    "${UL_BENCHMARK_MINIMAL_EXE_LINKER_FLAGS}"
    CACHE STRING
    "Minimal for benchmark FindThreads link"
    FORCE
)
set(CMAKE_SHARED_LINKER_FLAGS
    "${UL_BENCHMARK_MINIMAL_SHARED_LINKER_FLAGS}"
    CACHE STRING
    "Minimal for benchmark FindThreads link"
    FORCE
)

include(FetchContent)

# Mitigate transient "Failed to connect to github.com" (VPN/firewall/DNS/network):
# pre-clone with retries so a single flaky attempt doesn't fail the configure.
set(UL_benchmark_src "${CMAKE_BINARY_DIR}/_deps/googlebenchmark-src")
if(EXISTS "${UL_benchmark_src}/CMakeLists.txt")
    set(FETCHCONTENT_SOURCE_DIR_GOOGLEBENCHMARK
        "${UL_benchmark_src}"
        CACHE PATH
        ""
    )
else()
    find_package(Git REQUIRED QUIET)
    set(UL_clone_result 1)
    set(UL_attempt 0)
    while(UL_attempt LESS 5)
        math(EXPR UL_attempt_1 "${UL_attempt} + 1")
        message(
            STATUS
            "Fetching google/benchmark (attempt ${UL_attempt_1}/5)..."
        )
        file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/_deps")
        execute_process(
            COMMAND
                ${GIT_EXECUTABLE} clone --depth 1 -b main
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
        message(
            FATAL_ERROR
            "Failed to clone google/benchmark after 5 attempts. "
            "Check network, VPN, firewall, or install system libbenchmark-dev."
        )
    endif()
    set(FETCHCONTENT_SOURCE_DIR_GOOGLEBENCHMARK
        "${UL_benchmark_src}"
        CACHE PATH
        ""
    )
endif()

# GoogleTest calls find_package(Threads) without REQUIRED. When that fails, CMake
# caches Threads_FOUND=OFF; benchmark then calls find_package(Threads REQUIRED)
# and fails without re-running detection. Clear thread probe results so benchmark
# configures with the flags set above.
foreach(
    _ul_threads_cache_var
    IN
    ITEMS
        Threads_FOUND
        CMAKE_HAVE_LIBC_PTHREAD
        CMAKE_HAVE_PTHREAD_H
        CMAKE_HAVE_PTHREAD_CREATE
        THREADS_HAVE_PTHREAD_ARG
        CMAKE_THREAD_LIBS_INIT
        CMAKE_USE_PTHREADS_INIT
        CMAKE_USE_WIN32_THREADS_INIT
)
    unset(${_ul_threads_cache_var} CACHE)
endforeach()

FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG main
    GIT_SHALLOW ON
    GIT_PROGRESS ON
) # need master for benchmark::benchmark

FetchContent_MakeAvailable(googlebenchmark)

set(CMAKE_C_FLAGS
    "${UL_SAVED_CMAKE_C_FLAGS}"
    CACHE STRING
    "C compiler flags"
    FORCE
)
set(CMAKE_C_FLAGS_DEBUG
    "${UL_SAVED_CMAKE_C_FLAGS_DEBUG}"
    CACHE STRING
    "C compiler flags (Debug)"
    FORCE
)
set(CMAKE_C_FLAGS_RELEASE
    "${UL_SAVED_CMAKE_C_FLAGS_RELEASE}"
    CACHE STRING
    "C compiler flags (Release)"
    FORCE
)
set(CMAKE_C_FLAGS_RELWITHDEBINFO
    "${UL_SAVED_CMAKE_C_FLAGS_RELWITHDEBINFO}"
    CACHE STRING
    "C compiler flags (RelWithDebInfo)"
    FORCE
)
set(CMAKE_C_FLAGS_MINSIZEREL
    "${UL_SAVED_CMAKE_C_FLAGS_MINSIZEREL}"
    CACHE STRING
    "C compiler flags (MinSizeRel)"
    FORCE
)
set(CMAKE_CXX_FLAGS
    "${UL_SAVED_CMAKE_CXX_FLAGS}"
    CACHE STRING
    "C++ compiler flags"
    FORCE
)
set(CMAKE_CXX_FLAGS_DEBUG
    "${UL_SAVED_CMAKE_CXX_FLAGS_DEBUG}"
    CACHE STRING
    "C++ compiler flags (Debug)"
    FORCE
)
set(CMAKE_CXX_FLAGS_RELEASE
    "${UL_SAVED_CMAKE_CXX_FLAGS_RELEASE}"
    CACHE STRING
    "C++ compiler flags (Release)"
    FORCE
)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO
    "${UL_SAVED_CMAKE_CXX_FLAGS_RELWITHDEBINFO}"
    CACHE STRING
    "C++ compiler flags (RelWithDebInfo)"
    FORCE
)
set(CMAKE_CXX_FLAGS_MINSIZEREL
    "${UL_SAVED_CMAKE_CXX_FLAGS_MINSIZEREL}"
    CACHE STRING
    "C++ compiler flags (MinSizeRel)"
    FORCE
)
set(CMAKE_EXE_LINKER_FLAGS
    "${UL_SAVED_CMAKE_EXE_LINKER_FLAGS}"
    CACHE STRING
    "Executable linker flags"
    FORCE
)
set(CMAKE_SHARED_LINKER_FLAGS
    "${UL_SAVED_CMAKE_SHARED_LINKER_FLAGS}"
    CACHE STRING
    "Shared library linker flags"
    FORCE
)

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
