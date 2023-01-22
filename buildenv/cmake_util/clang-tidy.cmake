# include in your project root once and before any add_subdirectory calls (to get your hands on everything
# starting from there)
# *IMPORTANT* This is wip, not yet working.

macro(glob_recurse_append_cxx_sources cxx_sources dir)
    file(GLOB_RECURSE
        s
        ${dir}/*.[chi]pp ${dir}/*.[chi]xx ${dir}/*.cc ${dir}/*.hh ${dir}/*.ii ${dir}/*.[CHI] ${dir}/*.[chi]
    )
    list(APPEND ${cxx_sources} ${s})
endmacro()

glob_recurse_append_cxx_sources(all_cxx_sources apps)
glob_recurse_append_cxx_sources(all_cxx_sources include)
glob_recurse_append_cxx_sources(all_cxx_sources libs)
glob_recurse_append_cxx_sources(all_cxx_sources sdks)
glob_recurse_append_cxx_sources(all_cxx_sources source)
glob_recurse_append_cxx_sources(all_cxx_sources src)
glob_recurse_append_cxx_sources(all_cxx_sources test)

set(CLANG_TIDY_VER 15)

## generates a 'clang-tidy' target
#if (CMAKE_BUILD_TYPE STREQUAL "Debug")
#    find_program(CLANG_TIDY "clang-tidy-${CLANG_TIDY_VER}")
#    if (CLANG_TIDY)
#        if (NOT CMAKE_EXPORT_COMPILE_COMMANDS)
#            message(SEND_ERROR "You need to run cmake with -DCMAKE_EXPORT_COMPILE_COMMANDS=ON fo clang-tidy to recognise "
#                    "include paths")
#        endif ()
#        add_custom_target(
#            clang-tidy
#            COMMAND /usr/bin/clang-tidy-${CLANG_TIDY_VER}
#            --config=''
#            -p ${CMAKE_BINARY_DIR}
#            ${all_cxx_sources}
#        )
#    endif ()
#endif ()

# unfortunately I don't know how to restrict that to relevant dirs
set(CMAKE_CXX_CLANG_TIDY
        clang-tidy-${CLANG_TIDY_VER};
        -header-filter=.*;)
