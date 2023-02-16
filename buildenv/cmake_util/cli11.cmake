# #include "CLI/CLI.hpp"
# link with CLI11::CLI11
cmake_minimum_required(VERSION 3.14)

include(FetchContent)

FetchContent_Declare(
        cli11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11
        GIT_TAG        v2.3.2
)

FetchContent_MakeAvailable(cli11)
