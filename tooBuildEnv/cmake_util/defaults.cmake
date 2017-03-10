# BITS will be 32, 64, ...
math(EXPR BITS "8 * ${CMAKE_SIZEOF_VOID_P}")

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
    if (BITS EQUAL 64)
        set(COMPILER_SUBDIR "msvc64")
    else ()
        set(COMPILER_SUBDIR "msvc")
    endif ()
endif ()
# CMAKE_SYSTEM_NAME didn't work to differentiate e.ge. windows desktop and windows rt uwp, so introduce a customizable string
set(_CustomTargetSpecifier  "" CACHE STRING "can be used to append the bin dir 'bin_compiler' name by some custom string")
if (NOT ${_CustomTargetSpecifier} STREQUAL "")
    set(COMPILER_SUBDIR "${COMPILER_SUBDIR}_${_CustomTargetSpecifier}")
endif ()
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

# following 2 lines don't work as expected, why?
#set_property(GLOBAL PROPERTY CXX_STANDARD 11)
#set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options("-fext-numeric-literals")
    add_compile_options("-std=c++11")
    add_compile_options("-std=gnu++11")
    add_compile_options("-std=c++14")
    add_compile_options("-std=gnu++14")
    add_compile_options("-Wall")
    add_compile_options("-Wextra")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_compile_options("/W4")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options("-std=c++11")
    add_compile_options("-Wall")
endif ()

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
