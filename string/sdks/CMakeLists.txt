if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-Wno-sign-conversion)
endif ()
add_subdirectory(utf8cpp)
