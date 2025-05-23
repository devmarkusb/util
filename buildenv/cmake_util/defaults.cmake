# File to include for various useful cmake default settings.
# Important: it is recommended to include it in the topmost cmake file. The reason is the contained configuration
# of output dirs. So all libs and bins fall into the same designated place.

cmake_minimum_required(VERSION 3.20)

cmake_policy(SET CMP0054 NEW)

include_guard(DIRECTORY)

# to use cmake_print_variables(x) as shortcut for message(STATUS "x: " ${x})
include(CMakePrintHelpers)

######################################################################################################################
# general defaults and helper declarations

# I would like to get sth. like this working:
#add_definitions($<$<NOT:$<CONFIG:Debug>>:-DNDEBUG>)
# but until then...
if (NOT MSVC AND NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DNDEBUG)
endif ()

include(${CMAKE_CURRENT_LIST_DIR}/defs.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/builddir_cfg.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cpp_std_lib.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cpp_features.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/detail/deployment_build.cmake)

include(${CMAKE_CURRENT_LIST_DIR}/cppcheck.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/diagnostics.cmake)

include(GoogleTest)
enable_testing()

option(UL_ENABLE_LTO "enables link time optimization" OFF)

set(UL_BUILD_UNITTESTS ON CACHE BOOL "build (and run) unit tests as postbuild step")
if ("${UL_DEPLOY_TARGET}" STREQUAL "uwp")
    set(UL_BUILD_UNITTESTS OFF CACHE BOOL "do not change for uwp" FORCE)
endif ()
if (UL_ANDROID)
    set(UL_BUILD_UNITTESTS OFF CACHE BOOL "do not change for android" FORCE)
endif ()
option(UL_RUN_UNITTESTS_POSTBUILD "unit tests are run as part of the build - recommended to be disabled for debugging and CI" OFF)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(UL_IS_NON_APPLE_CLANG_COMPILER TRUE)
else()
    set(UL_IS_NON_APPLE_CLANG_COMPILER FALSE)
endif()

option(UL_USE_LLD_LINKER "use lld linker" ${UL_IS_NON_APPLE_CLANG_COMPILER})

# Might be used by clang-tidy and coverage, why not on by default.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# UL_BITS will be 32, 64, ...
math(EXPR UL_BITS "8 * ${CMAKE_SIZEOF_VOID_P}")

# UL_NPROC will contain processor count and 0 if count couldn't be determined
include(ProcessorCount)
ProcessorCount(UL_NPROC)

# this disturbs linking to gtest, expects d there as well...
# so better handle this via set_target_properties
#set(CMAKE_DEBUG_POSTFIX "d")

if (MSVC)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    # this doesn't work disabling specific linker warnings, why not?
    #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ignore:4075")
    add_definitions(-wd4075)
endif ()

if (MINGW)
    set(CMAKE_RC_COMPILER_INIT windres)
    enable_language(RC)
    set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
endif ()

if (UL_USE_LLD_LINKER)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -fuse-ld=lld")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=lld")
endif ()

if (UL_ENABLE_LTO)
    message(STATUS "IPO / LTO enabled")
    get_all_targets(all_targets ${CMAKE_SOURCE_DIR})
    foreach (target IN ITEMS ${all_targets})
        get_target_property(type ${target} TYPE)
        if (type STREQUAL "SHARED_LIBRARY" OR type STREQUAL "STATIC_LIBRARY" OR type STREQUAL "EXECUTABLE")
            set_property(TARGET ${target} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
        endif ()
    endforeach ()
endif ()


######################################################################################################################
# compile options

### general ###

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-builtin \
-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 \
-D_GLIBCXX_ASSERTIONS \
-fstack-clash-protection -fstack-protector-strong \
-fexceptions \
")
    if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
-fno-delete-null-pointer-checks \
-fno-strict-overflow \
-fno-strict-aliasing \
")
    endif ()
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fext-numeric-literals")
    # introduced for gperftools, but shouldn't do any harm generally
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
    if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftrivial-auto-var-init=zero")
    endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # nothing yet
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-limit-debug-info \
-fstrict-flex-arrays=3 \
")
endif ()

### warnings ###

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion -Werror \
-Wno-deprecated-declarations -Wformat -Wformat=2 -Wimplicit-fallthrough \
")

    # for C code
    # -Werror=implicit -Werror=incompatible-pointer-types -Werror=int-conversion
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-comment -Wtrampolines -Wbidi-chars=any")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmissing-prototypes -Wno-c++11-narrowing -Wdocumentation \
-Wno-c++20-compat -Wno-switch-default")
endif ()


######################################################################################################################
# cpp standard

set(UL_CXX_STANDARD "23" CACHE STRING "Use C++ Standard" FORCE)
set(CMAKE_CXX_STANDARD ${UL_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)


######################################################################################################################
# target specific general choices

macro(ul_set_target_defaults target)
    if (NOT UL_ANDROID) # easier than to fix the follow-up processes
        set_target_properties(${target} PROPERTIES DEBUG_POSTFIX "d")
    endif ()

    set_target_properties(${target} PROPERTIES POSITION_INDEPENDENT_CODE ON)

    set_target_properties(${target} PROPERTIES LINKER_LANGUAGE CXX)

    if (WIN32)
        set_property(TARGET ${target} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
    endif ()

    if ("${UL_DEPLOY_TARGET}" STREQUAL "uwp")
        target_compile_definitions(${target} INTERFACE _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
    endif ()

    get_target_property(target_type ${target} TYPE)
    if (target_type STREQUAL "EXECUTABLE" OR target_type STREQUAL "SHARED_LIBRARY")
        if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang"
            AND NOT ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"
            AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 17)
            AND NOT ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC"))
            target_link_options(${target} PUBLIC "LINKER:-z,nodlopen" "LINKER:-z,noexecstack" "LINKER:-z,relro"
                "LINKER:-z,now" "LINKER:--as-needed" "LINKER:--no-copy-dt-needed-entries")
        endif ()
    endif ()
endmacro()

macro(ul_set_target_filesystem target)
    if (UL_HAS_CPP_FILESYSTEM)
        if ("${UL_CPP_STD_LIB}" STREQUAL "libstdc++")
            target_link_libraries(${target} PUBLIC stdc++fs)
        else ()
            #target_link_libraries(${target} PUBLIC c++fs)
        endif ()
    endif ()
endmacro()

macro(ul_set_target_pthread target)
    if (UL_LINUX)
        target_link_libraries(${target} PUBLIC pthread)
    endif ()
endmacro()

macro(ul_set_target_cuda_separable_compilation target)
    set_target_properties(${target} PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
endmacro()

macro(ul_run_target_postbuild target)
    if (UL_RUN_UNITTESTS_POSTBUILD)
#    add_custom_command(TARGET ${target}
#            POST_BUILD
#            COMMAND ctest -V -C $<CONFIGURATION>)
        add_custom_command(
                TARGET ${target} POST_BUILD
                COMMAND ${target})
    endif ()
endmacro()

# shared lib export define (such that the build know whether to export or import symbols)
macro(ul_set_target_lib_export_def target define)
    set_property(TARGET ${target} APPEND PROPERTY COMPILE_DEFINITIONS ${define})
endmacro()

macro(ul_add_test target)
    gtest_discover_tests(${target})
endmacro()

# a platform independent 'executable', extra args are just all sources, lead by optional additional switches
macro(ul_add_executable target)
    set(impl_target_input)
    foreach(arg ${ARGN})
        list(APPEND impl_target_input ${arg})
    endforeach()
    if (UL_ANDROID)
        add_library(${target} SHARED ${impl_target_input})
    elseif (UL_MACOS)
        add_executable(${target} MACOSX_BUNDLE ${impl_target_input})
        set_target_properties(${target} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_BINARY_DIR}/Info.plist)
    else ()
        add_executable(${target} ${impl_target_input})
    endif ()
endmacro()

macro(ul_target_coverage target)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        setup_target_for_coverage_lcov(NAME ${target}_coverage EXECUTABLE ${target}-test)
    endif ()
endmacro()
