cmake_minimum_required(VERSION 3.23)

project(
    mb.util
    DESCRIPTION "Header-only C++ utility library"
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

include(buildenv-bootstrap.cmake)
list(PREPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/devenv/cmake")
include(mb-devenv-install-library-config)
include(GNUInstallDirs)

set(MB_UL_BUILD_UNITTESTS
    "${MB_UTIL_BUILD_TESTS}"
    CACHE INTERNAL
    "Backwards-compatible unit test toggle"
    FORCE
)

mb_ul_include(gsl.cmake)
add_subdirectory(src/string/sdks)

set(mb_util_generated_include_dir
    "${CMAKE_CURRENT_BINARY_DIR}/generated/include"
)
set(MB_UTIL_GENERATED_CONFIG_HEADER
    "${mb_util_generated_include_dir}/mb/ul/buildenv/config-gen.hpp"
)
file(MAKE_DIRECTORY "${mb_util_generated_include_dir}/mb/ul/buildenv")
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/mb/ul/buildenv/config-gen.hpp.in"
    "${MB_UTIL_GENERATED_CONFIG_HEADER}"
    @ONLY
)

add_library(mb.util INTERFACE)
add_library(mb::util ALIAS mb.util)

target_sources(
    mb.util
    PUBLIC
        FILE_SET HEADERS
            BASE_DIRS
                "${CMAKE_CURRENT_SOURCE_DIR}/include"
                "${mb_util_generated_include_dir}"
)

add_subdirectory(include/mb/ul)
target_include_directories(
    mb.util
    INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${mb_util_generated_include_dir}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
target_include_directories(
    mb.util
    SYSTEM
    INTERFACE
        "$<BUILD_INTERFACE:${GSL_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${utfcpp_INCLUDE_DIRS}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
target_compile_features(mb.util INTERFACE cxx_std_20)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_libraries(mb.util INTERFACE stdc++exp)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND NOT ANDROID)
    target_link_libraries(mb.util INTERFACE stdc++exp)
    target_compile_options(mb.util INTERFACE -Wno-unsafe-buffer-usage)
endif()

mb_ul_set_target_defaults(mb.util)

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
    enable_testing()
    add_subdirectory(src)
endif()

if(MB_UTIL_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
