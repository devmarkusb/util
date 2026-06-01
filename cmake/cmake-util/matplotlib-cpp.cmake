# link matplotlib_cpp (header-only; warnings apply in TUs that #include matplotlibcpp.h)
set(MATPLOTLIB_CPP_GENERATE_EXAMPLES OFF CACHE BOOL "example targets" FORCE)

include(FetchContent)

FetchContent_Declare(
    matplotlib-cpp
    GIT_REPOSITORY https://github.com/devmarkusb/matplotlib-cpp
    GIT_TAG d6d64c9f
    GIT_SHALLOW ON
    GIT_PROGRESS ON
)

FetchContent_MakeAvailable(matplotlib-cpp)

# MSVC Debug + release python3.lib (python.org / setup-python): see matplotlibcpp_msvc_embed.hpp
set(_mb_mplcpp_has_debug_python_lib FALSE)
if(TARGET Python3::Python)
    get_target_property(
        _mb_mplcpp_implib_dbg
        Python3::Python
        IMPORTED_IMPLIB_DEBUG
    )
    if(_mb_mplcpp_implib_dbg AND _mb_mplcpp_implib_dbg MATCHES "_d\\.lib$")
        set(_mb_mplcpp_has_debug_python_lib TRUE)
    endif()
endif()

set(matplotlib-cpp_INCLUDE_DIRS
    ${matplotlib-cpp_SOURCE_DIR}
    CACHE INTERNAL
    "matplotlib-cpp: include directories"
    FORCE
)

# Upstream BUILD_INTERFACE points at examples/; header lives at repo root.
target_include_directories(
    matplotlib_cpp
    SYSTEM
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
        $<BUILD_INTERFACE:${matplotlib-cpp_SOURCE_DIR}>
)

if(MSVC AND NOT _mb_mplcpp_has_debug_python_lib)
    target_compile_definitions(
        matplotlib_cpp
        INTERFACE $<$<CONFIG:Debug>:MB_MATPLOTLIBCPP_MSVC_RELEASE_PYTHON>
    )
endif()

mb_devenv_suppress_third_party_warnings(matplotlib_cpp)
