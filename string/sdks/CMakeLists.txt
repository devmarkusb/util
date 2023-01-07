cmake_minimum_required(VERSION 3.14)

include(FetchContent)

FetchContent_Declare(utfcpp
        GIT_REPOSITORY "https://github.com/nemtrif/utfcpp"
        GIT_TAG v3.2.3
        GIT_SHALLOW ON
        )

FetchContent_MakeAvailable(utfcpp)

set(utfcpp_INCLUDE_DIRS ${utfcpp_SOURCE_DIR}/source
        CACHE INTERNAL "utfcpp: include directories" FORCE)

add_subdirectory(utfcpp_extensions)
