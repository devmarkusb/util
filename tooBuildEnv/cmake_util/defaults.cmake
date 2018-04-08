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

file(TO_CMAKE_PATH "$ENV{dev_sdk_path}" dev_sdk_path_ENV_CMAKE_PATH)

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

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-fext-numeric-literals>)
    add_compile_options(-fno-omit-frame-pointer) # introduced for gperftools, but shouldn't do any harm generally
    add_compile_options(-Wall -Wextra)
    add_compile_options(-fno-builtin)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_compile_options(/W4)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-Wall)
    add_compile_options(-fno-limit-debug-info)
    add_compile_options(-fno-builtin)
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

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
