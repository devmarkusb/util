# file to include to add mathgl as subdir plus configurations

if(NOT DEFINED CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
    set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")
endif()
mb_ul_include(mathgl2-defaults.cmake)

add_subdirectory(mathgl)
mb_devenv_set_target_defaults(mgl)
mb_devenv_set_target_defaults(mgl-static)
if(NOT MB_UL_ANDROID)
    # needed for later version of mgl
    #set_target_properties(mgl_translations PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()

if(MB_UL_ANDROID)
    set(mglAdditionalTargetSources
        ${MB_UL_CMAKE_UTIL_DIR}/assets/std_ext/glob.c
        ${MB_UL_CMAKE_UTIL_DIR}/assets/std_ext/glob.hpp
        ${MB_UL_CMAKE_UTIL_DIR}/assets/std_ext/collate.hpp
    )
    target_sources(mgl PRIVATE ${mglAdditionalTargetSources})
    target_sources(mgl-static PRIVATE ${mglAdditionalTargetSources})
    target_include_directories(
        mgl
        SYSTEM
        PRIVATE ${MB_UL_CMAKE_UTIL_DIR}/assets/std_ext
    )
    target_include_directories(
        mgl-static
        SYSTEM
        PRIVATE ${MB_UL_CMAKE_UTIL_DIR}/assets/std_ext
    )
endif()

set(mathgl_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/mathgl/include
    ${CMAKE_CURRENT_BINARY_DIR}/mathgl/include
    CACHE INTERNAL
    "${mathgl}: include directories"
    FORCE
)
