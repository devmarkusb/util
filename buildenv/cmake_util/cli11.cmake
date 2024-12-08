# #include "CLI/CLI.hpp"
# link with CLI11::CLI11
cmake_minimum_required(VERSION 3.14)

find_package(CLI11 QUIET)
if (CLI11_FOUND)
    add_library(cli11 INTERFACE)
    return()
endif()
message(STATUS "CLI11 not found, building from source")

include(FetchContent)

FetchContent_Declare(
        cli11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11
        GIT_TAG        v2.3.2
        GIT_SHALLOW    ON
        GIT_PROGRESS   ON
)

FetchContent_MakeAvailable(cli11)
