# Print binaries involved when MB_DEVENV_USING_LIBCPP (libc++ is used)

if(MB_UL_ANDROID)
    return()
endif()

# C++ standard library setup
# Use libc++ with clang and libstdc++ with gcc
if(
    (
        "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"
        OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang"
    )
    AND MB_DEVENV_USING_LIBCPP
)
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -print-effective-triple
        OUTPUT_VARIABLE triple
    )
    string(STRIP ${triple} triple)

    get_filename_component(clang_compiler_path ${CMAKE_CXX_COMPILER} DIRECTORY)
    set(clang_stdlib_search_path ${clang_compiler_path}/../lib/)
    find_library(
        clang_stdlib
        NAMES c++
        PATHS ${clang_stdlib_search_path}
        PATH_SUFFIXES ${triple}
        NO_DEFAULT_PATH
    )
    if(NOT clang_stdlib)
        find_library(clang_stdlib NAMES c++)
    endif()
    if(clang_stdlib)
        message(STATUS "Found libc++: ${clang_stdlib}")
        get_filename_component(clang_stdlib_directory ${clang_stdlib} DIRECTORY)
        get_filename_component(clang_stdlib_fname ${clang_stdlib} NAME)
        find_library(
            clang_stdlib_abi
            NAMES c++abi
            PATHS ${clang_stdlib_directory}
            NO_DEFAULT_PATH
            REQUIRED
        )
        get_filename_component(clang_stdlib_abi_fname ${clang_stdlib_abi} NAME)
        file(
            GLOB clang_stdlib_files
            ${clang_stdlib_directory}/${clang_stdlib_fname}*
        )
        file(
            GLOB clang_stdlib_abi_files
            ${clang_stdlib_directory}/${clang_stdlib_abi_fname}*
        )

        # these perhaps need to be installed together with app binaries
        cmake_print_variables(clang_stdlib_files clang_stdlib_abi_files)
    endif()
endif()
