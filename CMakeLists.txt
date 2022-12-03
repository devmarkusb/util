cmake_minimum_required(VERSION 3.4.1)

project(ul)

include(buildenv-bootstrap.cmake)
too_include(googletest.cmake)

add_subdirectory(buildenv)
add_subdirectory(basiccodesupport)
add_subdirectory(string)
add_subdirectory(mem)
add_subdirectory(linklib)

set(ul_INCLUDE_DIRS
        ${tooBuildEnv_INCLUDE_DIRS}
        ${tooBasicCodeSupport_INCLUDE_DIRS}
        ${tooString_INCLUDE_DIRS}
        ${tooMem_INCLUDE_DIRS}
        ${tooLinklib_INCLUDE_DIRS}
        CACHE INTERNAL "ul include directories" FORCE)

set(ul_CMAKE_DIRS
        ${tooBuildEnv_CMAKE_DIRS}
        CACHE INTERNAL "ul cmake util directories" FORCE)

# Note, there are no libs that need linking, everything just header only.
# Internally some static libs are built for testing.
