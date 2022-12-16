cmake_minimum_required(VERSION 3.14)

include(FetchContent)

FetchContent_Declare(mb-util
        GIT_REPOSITORY "/home/markus/projects/git/libs/util"
        GIT_TAG HEAD
        GIT_SHALLOW ON
        )

FetchContent_MakeAvailable(mb-util)
