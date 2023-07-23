# #include "BigInt.hpp"
cmake_minimum_required(VERSION 3.14)

include(FetchContent)

FetchContent_Declare(
        bigint
        GIT_REPOSITORY https://github.com/faheel/BigInt
        GIT_TAG        v0.5.0-dev
        GIT_SHALLOW    ON
        GIT_PROGRESS   ON
)

FetchContent_MakeAvailable(bigint)

set(bigint_INCLUDE_DIRS
    ${bigint_SOURCE_DIR}
    CACHE INTERNAL "bigint: include directories" FORCE)
