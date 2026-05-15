cmake_minimum_required(VERSION 3.30...4.3)

project(
    mb.util
    DESCRIPTION "C++ utility library (header-only)"
    LANGUAGES CXX
    VERSION 0.1.0
)

option(
    MB_UTIL_BUILD_TESTS
    "Enable building tests and test infrastructure. Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
    ${PROJECT_IS_TOP_LEVEL}
)

option(
    MB_UTIL_BUILD_EXAMPLES
    "Enable building examples. Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
    ${PROJECT_IS_TOP_LEVEL}
)

option(
    MB_UL_DISABLE_NAMESPACE_ALIAS
    "Set to ON if you want the base namespace to be mb::ul instead of ul."
    OFF
)

include(cmake/cmake-util.cmake)
list(PREPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/devenv/cmake")
if(MB_UTIL_BUILD_TESTS)
    include(mb-devenv-ctest-root)
endif()
include(mb-devenv-defaults)

include(mb-devenv-cppcheck)
include(mb-devenv-install-library-config)
include(GNUInstallDirs)

mb_ul_include(gsl.cmake)

# From fetched https://github.com/devmarkusb/pre-commit (see fetchcontent-lockfile.json).
# Subdirectory consumers: skip dev hook setup (they must not depend on mb_pre_commit, and
# mb_pre_commit_setup defaults use CMAKE_SOURCE_DIR = top-level tree, not mb.util alone).
if(PROJECT_IS_TOP_LEVEL)
    mb_pre_commit_setup()
endif()

add_subdirectory(src/string/sdks)

add_library(mb.util INTERFACE)
add_library(mb::util ALIAS mb.util)

add_subdirectory(include/mb/ul)

target_include_directories(
    mb.util
    SYSTEM
    INTERFACE
        "$<BUILD_INTERFACE:${GSL_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${utfcpp_INCLUDE_DIRS}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_libraries(mb.util INTERFACE stdc++exp)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND NOT ANDROID)
    target_link_libraries(mb.util INTERFACE stdc++exp)
    target_compile_options(mb.util INTERFACE -Wno-unsafe-buffer-usage)
endif()

mb_devenv_set_target_defaults(mb.util)

install(
    DIRECTORY "${GSL_SOURCE_DIR}/include/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT "mb.util"
)
install(
    DIRECTORY "${utfcpp_INCLUDE_DIRS}/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT "mb.util"
)

mb_devenv_install_library(mb.util)

if(MB_UTIL_BUILD_TESTS)
    add_subdirectory(src)
endif()

if(MB_UTIL_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
