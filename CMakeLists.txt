cmake_minimum_required(VERSION 3.4.1)

project(util)

include(buildenv-bootstrap.cmake)
ul_include(googletest.cmake)

add_subdirectory(buildenv)
add_subdirectory(basiccodesupport)
add_subdirectory(string)
add_subdirectory(mem)
add_subdirectory(linklib)

set(ul_INCLUDE_DIRS
        ${ulBuildEnv_INCLUDE_DIRS}
        ${ulBasicCodeSupport_INCLUDE_DIRS}
        ${ulString_INCLUDE_DIRS}
        ${ulMem_INCLUDE_DIRS}
        ${ulLinklib_INCLUDE_DIRS}
        CACHE INTERNAL "ul include directories" FORCE)

set(ul_CMAKE_DIRS
        ${ulBuildEnv_CMAKE_DIRS}
        CACHE INTERNAL "ul cmake util directories" FORCE)

# Note, there are no libs that need linking, everything just header only.
# Internally some static libs are built for testing.
