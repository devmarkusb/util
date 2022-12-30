cmake_minimum_required(VERSION 3.12.4)

# File to include for various useful cmake default settings.
# Important: it is recommended to include it in the topmost cmake file. The reason is the contained configuration
# of output dirs. So all libs and bins fall into the same designated place.

######################################################################################################################
# general defaults and helper declarations

# I would like to get sth. like this working:
#add_definitions($<$<NOT:$<CONFIG:Debug>>:-DNDEBUG>)
# but until then...
if (NOT MSVC AND NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DNDEBUG)
endif ()

enable_testing()

include(${CMAKE_CURRENT_LIST_DIR}/defs.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/builddir_cfg.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cpp_std_lib.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cpp_features.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/detail/deployment_build.cmake)

set(UL_BUILD_UNITTESTS ON CACHE BOOL "build (and run) unit tests as postbuild step")
if ("${UL_DEPLOY_TARGET}" STREQUAL "uwp")
    set(UL_BUILD_UNITTESTS OFF CACHE BOOL "do not change for uwp" FORCE)
endif ()
if (UL_ANDROID)
    set(UL_BUILD_UNITTESTS OFF CACHE BOOL "do not change for android" FORCE)
endif ()

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

# cotire needs this
#add_definitions(-D__STRICT_ANSI__)


######################################################################################################################
# compile options

### general ###

set(cpp_compile_options)
set(c_compile_options)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(cpp_compile_options
            -fext-numeric-literals
            # introduced for gperftools, but shouldn't do any harm generally
            -fno-omit-frame-pointer
            -fno-builtin)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # nothing yet
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(cpp_compile_options
            -fno-limit-debug-info
            -fno-builtin)
endif ()


### warnings ###

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(cpp_compile_options
            ${cpp_compile_options}
            -Wall -Wextra -Wconversion -Werror)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(cpp_compile_options
            ${cpp_compile_options}
            /W4)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(cpp_compile_options
            ${cpp_compile_options}
            -Wall -Wextra -Wconversion -Werror
            -Wmissing-prototypes
            -Wno-c++11-narrowing
    )
endif ()

# switch to 1 and adapt details if you want to see a maximum of warnings
if (0)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # partially implemented
        set(cpp_compile_options
                ${cpp_compile_options}
                -Wconversion
        )
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        # not implemented
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(cpp_compile_options
                ${cpp_compile_options}
                -Weverything # should include -Wconversion
                # category of must-be-blacklisted ones
                -Wno-unused-member-function -Wno-c++98-compat -Wno-deprecated -Wno-weak-vtables
                -Wno-shadow-field-in-constructor -Wno-undef -Wno-c++98-compat-pedantic
                -Wno-double-promotion -Wmissing-prototypes
                # category of could-be-useful ones
                -Wno-redundant-parens # can be interesting, but appears a lot in Qt moc_ files, so...
                -Wno-undefined-reinterpret-cast # same here
                -Wno-documentation # might want to turn that on - fires a lot due to misuse of multiline //!'s
                -Wno-documentation-unknown-command # investigate use of \return, \param etc., unfortunate
                -Wno-global-constructors -Wno-exit-time-destructors
                -Wno-switch-enum -Wno-covered-switch-default # a no default warning would be more useful
                -Wno-missing-noreturn
                -Wno-padded # this could be useful to turn on locally (at most) to find out about padding size
        )
    endif ()
endif ()


### finally setting compile options ###

add_compile_options(
        "$<$<COMPILE_LANGUAGE:C>:${c_compile_options}>"
        "$<$<COMPILE_LANGUAGE:CXX>:${cpp_compile_options}>")


######################################################################################################################
# cpp standard

set(UL_CXX_STANDARD "20" CACHE STRING "Use C++ Standard")
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
        target_compile_definitions(${target} PUBLIC _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
    endif ()
endmacro()

macro(ul_set_target_filesystem target)
    if (UL_HAS_CPP_FILESYSTEM)
        if ("${UL_CPP_STD_LIB}" STREQUAL "libstdc++")
            target_link_libraries(${target} PUBLIC stdc++fs)
        endif ()
    endif ()
endmacro()

macro(ul_set_target_pthread target)
    if (UL_LINUX)
        target_link_libraries(${target} PUBLIC pthread)
    endif ()
endmacro()

macro(ul_set_target_openmp target)
    target_link_libraries(${target} PUBLIC -fopenmp)
endmacro()

macro(ul_set_target_cuda_separable_compilation target)
    set_target_properties(${target} PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
endmacro()

macro(ul_run_target_postbuild target)
#    add_custom_command(TARGET ${target}
#            POST_BUILD
#            COMMAND ctest -V -C $<CONFIGURATION>)
    add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND ${target})
endmacro()

# shared lib export define (such that the build know whether to export or import symbols)
macro(ul_set_target_lib_export_def target define)
    set_property(TARGET ${target} APPEND PROPERTY COMPILE_DEFINITIONS ${define})
endmacro()

macro(ul_add_test target)
    add_test(NAME ${target} COMMAND ${target})
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
        setup_target_for_coverage_lcov(NAME ${target}_coverage EXECUTABLE ${target}Test)
    endif ()
endmacro()

if (NOT MSVC)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        include(${CMAKE_CURRENT_LIST_DIR}/3rdparty/bilke/cmake-modules/CodeCoverage.cmake)
    endif ()
endif ()
