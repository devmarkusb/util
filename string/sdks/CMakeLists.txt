cmake_minimum_required(VERSION 3.14)

include(FetchContent)

FetchContent_Declare(utfcpp
        GIT_REPOSITORY "https://github.com/nemtrif/utfcpp"
        GIT_TAG v4.0.2
        GIT_SHALLOW  ON
        GIT_PROGRESS ON
        )

FetchContent_MakeAvailable(utfcpp)

set(utfcpp_INCLUDE_DIRS ${utfcpp_SOURCE_DIR}/source
        CACHE INTERNAL "utfcpp: include directories" FORCE)

add_subdirectory(utfcpp_extensions)
