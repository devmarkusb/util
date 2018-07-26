cmake_minimum_required(VERSION 3.4.1)

add_subdirectory(buildenv)
add_subdirectory(basiccodesupport)
add_subdirectory(string)
add_subdirectory(mem)
add_subdirectory(linklib)

set(tfl_INCLUDE_DIRS
        ${tooBuildEnv_INCLUDE_DIRS}
        ${tooBasicCodeSupport_INCLUDE_DIRS}
        ${tooString_INCLUDE_DIRS}
        ${tooMem_INCLUDE_DIRS}
        ${tooLinklib_INCLUDE_DIRS}
        CACHE INTERNAL "tfl include directories" FORCE)

# Note, there are no libs that need linking, everything just header only.
# Internally some static libs are built for testing.
