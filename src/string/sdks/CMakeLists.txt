cmake_minimum_required(VERSION 3.14)

include(FetchContent)

if(POLICY CMP0097)
    cmake_policy(SET CMP0097 NEW)
endif()

FetchContent_Declare(
    utfcpp
    GIT_REPOSITORY "https://github.com/nemtrif/utfcpp"
    GIT_TAG v4.0.6
    GIT_SHALLOW ON
    GIT_PROGRESS ON
    GIT_SUBMODULES ""
)

FetchContent_MakeAvailable(utfcpp)

set(utfcpp_INCLUDE_DIRS
    ${utfcpp_SOURCE_DIR}/source
    CACHE INTERNAL
    "utfcpp: include directories"
    FORCE
)
