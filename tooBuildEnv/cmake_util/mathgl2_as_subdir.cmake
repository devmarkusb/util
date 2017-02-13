include(${TOO_CMAKE_INCDIR}/mathgl2_defaults.cmake)

add_subdirectory(mathgl)
set_target_properties(mgl PROPERTIES DEBUG_POSTFIX "d")

set(mathgl_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/mathgl/include
    CACHE INTERNAL "${mathgl}: include directories" FORCE)
