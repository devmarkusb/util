# file to include to add mathgl as subdir plus configurations

if(NOT DEFINED CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
     set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")
endif()
ul_include(mathgl2_defaults.cmake)

add_subdirectory(mathgl)
ul_set_target_defaults(mgl)
ul_set_target_defaults(mgl-static)
set_target_properties(mgl_translations PROPERTIES EXCLUDE_FROM_ALL TRUE)

if (UL_ANDROID)
    set(mglAdditionalTargetSources
            ${UL_CMAKE_UTIL_DIR}/assets/std_ext/glob.c
            ${UL_CMAKE_UTIL_DIR}/assets/std_ext/glob.h
            ${UL_CMAKE_UTIL_DIR}/assets/std_ext/collate.h
    )
    target_sources(mgl PRIVATE ${mglAdditionalTargetSources})
    target_sources(mgl-static PRIVATE ${mglAdditionalTargetSources})
    target_include_directories(mgl SYSTEM PRIVATE ${UL_CMAKE_UTIL_DIR}/assets/std_ext)
    target_include_directories(mgl-static SYSTEM PRIVATE ${UL_CMAKE_UTIL_DIR}/assets/std_ext)
endif ()

set(mathgl_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/mathgl/include
    ${CMAKE_CURRENT_BINARY_DIR}/mathgl/include
    CACHE INTERNAL "${mathgl}: include directories" FORCE)
