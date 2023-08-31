# Declares variables UL_USE_CLANG_STDLIB and UL_CPP_STD_LIB to set and detect the C++ standard library type to be used
# for linking currently. Possible values are "libstdc++" for the GNU lib, "libc++" for the Clang lib and "msvc" for
# Microsoft's implementation.

include(CheckCXXSourceCompiles)

if (UL_ANDROID)
    return()
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    set(default_use_clang_stdlib ON)
else()
    set(default_use_clang_stdlib OFF)
endif()

# Doesn't work reliably, also manually specify -DCMAKE_CXX_FLAGS="-stdlib=libc++"
# as some compiler tests seem to start before anything else.
option(UL_USE_CLANG_STDLIB "use libc++ instead of libstdc++" ${default_use_clang_stdlib})

# default, to be overwritten in case
set(UL_CPP_STD_LIB_IMPL "libstdc++")

# C++ standard library setup
# Use libc++ with clang and libstdc++ with gcc
if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    AND UL_USE_CLANG_STDLIB)
    set(STDLIB_CXX_FLAGS "-stdlib=libc++" CACHE STRING "internal")
    # to use the clang c++ stdlib we must set the flag -stdlib=libc++
    # see https://libcxx.llvm.org/docs/UsingLibcxx.html
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${STDLIB_CXX_FLAGS}")

    execute_process(COMMAND ${CMAKE_CXX_COMPILER} -print-effective-triple OUTPUT_VARIABLE triple)
    string(STRIP ${triple} triple)

    get_filename_component(clang_compiler_path ${CMAKE_CXX_COMPILER} DIRECTORY)
    set(clang_stdlib_search_path ${clang_compiler_path}/../lib/)
    find_library(clang_stdlib NAMES c++ PATHS ${clang_stdlib_search_path} PATH_SUFFIXES ${triple} NO_DEFAULT_PATH)
    if (NOT clang_stdlib)
        find_library(clang_stdlib NAMES c++)
    endif()
    if (clang_stdlib)
        set(UL_CPP_STD_LIB_IMPL "libc++")
        message(STATUS "Found libc++: ${clang_stdlib}")
        get_filename_component(clang_stdlib_directory ${clang_stdlib} DIRECTORY)
        get_filename_component(clang_stdlib_fname ${clang_stdlib} NAME)
        find_library(clang_stdlib_abi NAMES c++abi PATHS ${clang_stdlib_directory} NO_DEFAULT_PATH REQUIRED)
        get_filename_component(clang_stdlib_abi_fname ${clang_stdlib_abi} NAME)
        file(GLOB clang_stdlib_files ${clang_stdlib_directory}/${clang_stdlib_fname}*)
        file(GLOB clang_stdlib_abi_files ${clang_stdlib_directory}/${clang_stdlib_abi_fname}*)

        # these perhaps need to be installed together with app binaries
        cmake_print_variables(clang_stdlib_files clang_stdlib_abi_files)
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(UL_CPP_STD_LIB_IMPL "msvc")
endif()

check_cxx_source_compiles(
    "
    #include <ciso646>
    int a =
    #ifdef _LIBCPP_VERSION
        1;
    #else
        somethingthatdoesntcompile;
    #endif
    int main() { return 0; }
    "
    UL_CHECK_USING_CLANG_CPP_STD_LIB)

check_cxx_source_compiles(
    "
    #include <cstddef>
    int a =
    #ifdef __GLIBCXX__
        1;
    #else
        somethingthatdoesntcompile;
    #endif
    int main() { return 0; }
    "
    UL_CHECK_USING_GNU_CPP_STD_LIB)

if (UL_CHECK_USING_CLANG_CPP_STD_LIB)
    set(UL_CPP_STD_LIB_IMPL2 "libc++")
elseif (UL_CHECK_USING_GNU_CPP_STD_LIB)
    set(UL_CPP_STD_LIB_IMPL2 "libstdc++")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(UL_CPP_STD_LIB_IMPL2 "msvc")
else ()
    set(UL_CPP_STD_LIB_IMPL2 "?")
endif ()

if (NOT "${UL_CPP_STD_LIB_IMPL2}" STREQUAL "${UL_CPP_STD_LIB_IMPL}")
    cmake_print_variables(UL_CPP_STD_LIB_IMPL2)
    cmake_print_variables(UL_CPP_STD_LIB_IMPL)
    message(FATAL_ERROR "Above vars don't match, consistency check needs update.")
endif()

set(UL_CPP_STD_LIB ${UL_CPP_STD_LIB_IMPL} CACHE INTERNAL "indicates C++ standard library currently in use" FORCE)

cmake_print_variables(UL_CPP_STD_LIB)

