## build dir naming ###

# CMAKE_SYSTEM_NAME didn't work to differentiate e.g. windows desktop and windows rt uwp, so introduce a customizable string
set(impl_CustomTargetSpecifierHelpStr "can be used to append the bin dir 'bin_compiler' name by some custom string")
if (TOO_ANDROID)
    set(TOO_CUSTOM_TARGET_SPECIFIER "android" CACHE STRING "${impl_CustomTargetSpecifierHelpStr}" FORCE)
elseif ("${TOO_DEPLOY_TARGET}" STREQUAL "uwp")
    set(TOO_CUSTOM_TARGET_SPECIFIER "winrt" CACHE STRING "${impl_CustomTargetSpecifierHelpStr}" FORCE)
else ()
    set(TOO_CUSTOM_TARGET_SPECIFIER "" CACHE STRING "${impl_CustomTargetSpecifierHelpStr}" FORCE)
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    if (WIN32)
        set(impl_CompilerSubdir "clang_msvc")
    else ()
        set(impl_CompilerSubdir "clang")
    endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    set(impl_CompilerSubdir "appleclang")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if (WIN32)
        set(impl_CompilerSubdir "mingw")
    else ()
        set(impl_CompilerSubdir "gcc")
    endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    if (TOO_BITS EQUAL 64)
        set(impl_CompilerSubdir "msvc64")
    else ()
        set(impl_CompilerSubdir "msvc")
    endif ()
endif ()
if (NOT ${TOO_CUSTOM_TARGET_SPECIFIER} STREQUAL "")
    set(impl_CompilerSubdir "${impl_CompilerSubdir}_${TOO_CUSTOM_TARGET_SPECIFIER}")
endif ()

# Note: An idea was to introduce the choice of building one dir level up (to keep source dir clean)
#       But the major problem is, that we would like to git-check-in some binaries here and there...
set(impl_OutputDir_bin ${CMAKE_SOURCE_DIR}/bin_${impl_CompilerSubdir})
set(impl_OutputDir_lib ${CMAKE_CURRENT_SOURCE_DIR}/lib_${impl_CompilerSubdir})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${impl_OutputDir_bin})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${impl_OutputDir_lib})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${impl_OutputDir_lib})

# workaround for msvc, which adds additional subdirs per config
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    foreach (OutputConfig ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${OutputConfig} OutputConfig)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OutputConfig} ${impl_OutputDir_bin})
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OutputConfig} ${impl_OutputDir_lib})
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OutputConfig} ${impl_OutputDir_lib})
    endforeach ()
endif ()


###  application deployment package dir ###

# impl_BIN_PACKAGE_SUBDIR aims to point right to the dir of the app binary within a app package/bundle dir.
# Often this is also identical to the impl_BIN_PACKAGE_SUBDIR_BASE, the package dir itself.
# Same goes for TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR and ..._BASE, which are the full/absolute path
# equivalents and can be used for direct build output.

if ("${TOO_DEPLOY_TARGET}" STREQUAL "uwp")
    # no-op, don't overwrite, already given by toolchain file
elseif (TOO_LINUX AND "${TOO_DEPLOY_TARGET}" STREQUAL "linuxAppDir")
    set(impl_BIN_PACKAGE_SUBDIR_BASE "${PROJECT_NAME}.app")
    set(impl_BIN_PACKAGE_SUBDIR "${impl_BIN_PACKAGE_SUBDIR_BASE}/usr/bin")
elseif (TOO_MACOS)
    set(impl_BIN_PACKAGE_SUBDIR_BASE "${PROJECT_NAME}.app")
    set(impl_BIN_PACKAGE_SUBDIR "${impl_BIN_PACKAGE_SUBDIR_BASE}/Contents/MacOs")
endif ()

if (impl_BIN_PACKAGE_SUBDIR)
    set(TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${impl_BIN_PACKAGE_SUBDIR_BASE}")
    set(TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${impl_BIN_PACKAGE_SUBDIR}")
else ()
    set(TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    set(TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR "${TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}")
endif ()
