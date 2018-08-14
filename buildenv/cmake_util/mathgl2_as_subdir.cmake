# file to include to add mathgl as subdir plus configurations

if(NOT DEFINED CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
     set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")
endif()
too_include(mathgl2_defaults.cmake)

add_subdirectory(mathgl)
too_set_target_defaults(mgl)
too_set_target_defaults(mgl-static)
if (ANDROID)
    set(mglAdditionalTargetSources
            ${TOO_CMAKE_UTIL_DIR}/assets/std_ext/glob.c
            ${TOO_CMAKE_UTIL_DIR}/assets/std_ext/glob.h
            ${TOO_CMAKE_UTIL_DIR}/assets/std_ext/collate.h
    )
    target_sources(mgl PRIVATE ${mglAdditionalTargetSources})
    target_sources(mgl-static PRIVATE ${mglAdditionalTargetSources})
    target_include_directories(mgl SYSTEM PRIVATE ${TOO_CMAKE_UTIL_DIR}/assets/std_ext)
    target_include_directories(mgl-static SYSTEM PRIVATE ${TOO_CMAKE_UTIL_DIR}/assets/std_ext)
endif ()

set(mathgl_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/mathgl/include
    ${CMAKE_CURRENT_BINARY_DIR}/mathgl/include
    CACHE INTERNAL "${mathgl}: include directories" FORCE)
