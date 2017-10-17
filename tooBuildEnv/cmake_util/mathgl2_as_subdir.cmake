if(NOT DEFINED CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
     set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")
endif()
include(${TOO_CMAKE_INCDIR}/mathgl2_defaults.cmake)

add_subdirectory(mathgl)
set_target_properties(mgl PROPERTIES DEBUG_POSTFIX "d")
if (ANDROID)
    target_sources(mgl PRIVATE
        ${TOO_CMAKE_INCDIR}/assets/std_ext/glob.c
        ${TOO_CMAKE_INCDIR}/assets/std_ext/glob.h
        ${TOO_CMAKE_INCDIR}/assets/std_ext/collate.h
    )
    target_include_directories(mgl SYSTEM PRIVATE ${TOO_CMAKE_INCDIR}/assets/std_ext)
endif ()

set(mathgl_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/mathgl/include
    ${CMAKE_CURRENT_BINARY_DIR}/mathgl/include
    CACHE INTERNAL "${mathgl}: include directories" FORCE)
