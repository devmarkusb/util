cmake_minimum_required(VERSION 3.4.1)

project(util)

include(buildenv-bootstrap.cmake)
ul_include(googletest.cmake)

option(UL_DISABLE_NAMESPACE_ALIAS "Set to ON if you want the base namespace to be mb::ul instead of ul" OFF)
configure_file(
        ${PROJECT_SOURCE_DIR}/buildenv/include/ul/config_gen.h.in
        ${PROJECT_SOURCE_DIR}/buildenv/include/ul/config_gen.h
        @ONLY)

ul_include(clang-tidy.cmake)
ul_include(cppcheck.cmake)

add_subdirectory(buildenv)
add_subdirectory(basiccodesupport)
add_subdirectory(string)
add_subdirectory(mem)
add_subdirectory(linklib)
add_subdirectory(util)

ul_include(clang-format.cmake)
ul_include(githooks.cmake)

set(dirs_to_doc README.md basiccodesupport buildenv linklib mem string util)
ul_include(doxygen.cmake)

set(ul_INCLUDE_DIRS
        ${ulBasicCodeSupport_INCLUDE_DIRS}
        ${ulBuildEnv_INCLUDE_DIRS}
        ${ulLinklib_INCLUDE_DIRS}
        ${ulMem_INCLUDE_DIRS}
        ${ulString_INCLUDE_DIRS}
        ${mb-util_INCLUDE_DIRS}
        CACHE INTERNAL "util include directories" FORCE)

set(ul_CMAKE_DIRS
        ${ulBuildEnv_CMAKE_DIRS}
        CACHE INTERNAL "util cmake util directories" FORCE)

# Note, there are no libs that need linking, everything just header only.
# Internally some static libs are built for testing.
