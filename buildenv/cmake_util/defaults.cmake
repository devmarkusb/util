# File to include for various useful cmake default settings.
# Important: it is recommended to include it in the topmost cmake file. The reason is the contained configuration
# of output dirs. So all libs and bins fall into the same designated place.

######################################################################################################################
# general defaults and helper declarations

if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DNDEBUG)
endif ()

enable_testing()

include(${CMAKE_CURRENT_LIST_DIR}/cpp_std_lib.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cpp_features.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/detail/deployment_build.cmake)

# CMAKE_SYSTEM_NAME didn't work to differentiate e.g. windows desktop and windows rt uwp, so introduce a customizable string
set(TOO_CUSTOM_TARGET_SPECIFIER_IMPL_HELP_STR "can be used to append the bin dir 'bin_compiler' name by some custom string")
if (ANDROID)
    set(TOO_CUSTOM_TARGET_SPECIFIER "android" CACHE STRING "${TOO_CUSTOM_TARGET_SPECIFIER_IMPL_HELP_STR}" FORCE)
elseif ("${TOO_DEPLOY_TARGET}" STREQUAL "uwp")
    set(TOO_CUSTOM_TARGET_SPECIFIER "winrt" CACHE STRING "${TOO_CUSTOM_TARGET_SPECIFIER_IMPL_HELP_STR}" FORCE)
else ()
    set(TOO_CUSTOM_TARGET_SPECIFIER "" CACHE STRING "${TOO_CUSTOM_TARGET_SPECIFIER_IMPL_HELP_STR}" FORCE)
endif ()
set(TOO_BUILD_UNITTESTS ON CACHE BOOL "build (and run) unit tests as postbuild step")

# TOO_BITS will be 32, 64, ...
math(EXPR TOO_BITS "8 * ${CMAKE_SIZEOF_VOID_P}")

# TOO_NPROC will contain processor count and 0 if count couldn't be determined
include(ProcessorCount)
ProcessorCount(TOO_NPROC)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    if (WIN32)
        set(COMPILER_SUBDIR "clang_msvc")
    else ()
        set(COMPILER_SUBDIR "clang")
    endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if (WIN32)
        set(COMPILER_SUBDIR "mingw")
    else ()
        set(COMPILER_SUBDIR "gcc")
    endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    if (TOO_BITS EQUAL 64)
        set(COMPILER_SUBDIR "msvc64")
    else ()
        set(COMPILER_SUBDIR "msvc")
    endif ()
endif ()
if (NOT ${TOO_CUSTOM_TARGET_SPECIFIER} STREQUAL "")
    set(COMPILER_SUBDIR "${COMPILER_SUBDIR}_${TOO_CUSTOM_TARGET_SPECIFIER}")
endif ()

# Note: An idea was to introduce the choice of building one dir level up (to keep source dir clean)
#       But the major problem is, that we would like to git-check-in some binaries here and there...
set(OutputDir_bin ${CMAKE_SOURCE_DIR}/bin_${COMPILER_SUBDIR})
set(OutputDir_lib ${CMAKE_CURRENT_SOURCE_DIR}/lib_${COMPILER_SUBDIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OutputDir_bin})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OutputDir_lib})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OutputDir_lib})

file(TO_CMAKE_PATH "$ENV{dev_sdk_path}" dev_sdk_path_ENV_CMAKE_PATH_impl)
set(dev_sdk_path_ENV_CMAKE_PATH "${dev_sdk_path_ENV_CMAKE_PATH_impl}" CACHE INTERNAL "cmake normalized path" FORCE)

# workaround for msvc, which adds additional subdirs per config
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
foreach (OutputConfig ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OutputConfig} OutputConfig)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OutputConfig} ${OutputDir_bin})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OutputConfig} ${OutputDir_lib})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OutputConfig} ${OutputDir_lib})
endforeach ()
endif ()

# this disturbs linking to gtest, expects d there as well...
# so better handle this via set_target_properties
#set(CMAKE_DEBUG_POSTFIX "d")

if (MSVC)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    # this doesn't work disabling specific linker warnings, why not?
    #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ignore:4075")
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
            -Wall -Wextra)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(cpp_compile_options
            ${cpp_compile_options}
            /W4)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(cpp_compile_options
            ${cpp_compile_options}
            -Wall -Wextra)
endif ()

# switch to 1 and adapt details if you want to see a maximum of warnings
if (0)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # not implemented
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        # not implemented
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(cpp_compile_options
                ${cpp_compile_options}
                -Weverything
                # category of must-be-blacklisted ones
                -Wno-unused-member-function -Wno-c++98-compat -Wno-deprecated -Wno-weak-vtables
                -Wno-shadow-field-in-constructor -Wno-undef -Wno-c++98-compat-pedantic
                -Wno-double-promotion
                # category of could-be-useful ones
                -Wno-redundant-parens # can be interesting, but appears a lot in Qt moc_ files, so...
                -Wno-documentation # might want to turn that on - fires a lot due to misuse of multiline //!'s
                -Wno-documentation-unknown-command # investigate use of \Returns, \params etc., unfortunate
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

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)


######################################################################################################################
# target specific general choices

macro(too_set_target_defaults target)
    if (NOT ANDROID) # easier than to fix the follow-up processes
        set_target_properties(${target} PROPERTIES DEBUG_POSTFIX "d")
    endif ()

    set_target_properties(${target} PROPERTIES POSITION_INDEPENDENT_CODE ON)

    set_target_properties(${target} PROPERTIES LINKER_LANGUAGE CXX)

    if (WIN32)
        set_property(TARGET ${target} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
    endif ()

    if ("${TOO_DEPLOY_TARGET}" STREQUAL "uwp")
        target_compile_definitions(${target} PUBLIC _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
    endif ()
endmacro()

macro(too_set_target_filesystem target)
    if (TOO_HAS_CPP_FILESYSTEM)
        if ("${TOO_CPP_STD_LIB}" STREQUAL "libstdc++")
            target_link_libraries(${target} PUBLIC stdc++fs)
        endif ()
    endif ()
endmacro()

macro(too_set_target_pthread target)
    if (LINUX)
        target_link_libraries(${target} PUBLIC pthread)
    endif ()
endmacro()

macro(too_set_target_openmp target)
    target_link_libraries(${target} PUBLIC -fopenmp)
endmacro()

macro(too_set_target_cuda_separable_compilation target)
    set_target_properties(${target} PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
endmacro()

macro(too_run_target_postbuild target)
    add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND ${target})
endmacro()

# shared lib export define (such that the build know whether to export or import symbols)
macro(too_set_target_lib_export_def target define)
    set_property(TARGET ${target} APPEND PROPERTY COMPILE_DEFINITIONS ${define})
endmacro()

macro(too_add_test target)
    add_test(NAME ${target} COMMAND ${target})
endmacro()
