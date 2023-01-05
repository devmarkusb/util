# include in your project root once; generates a 'clang-format' target

macro(glob_recurse_append_cxx_sources cxx_sources dir)
    file(GLOB_RECURSE
        s
        ${dir}/*.[chi]pp ${dir}/*.[chi] ${dir}/*.cu
    )
    list(APPEND ${cxx_sources} ${s})
endmacro()

#macro(glob_append_cxx_sources cxx_sources dir)
#    file(GLOB
#        s
#        ${dir}/*.[chi]pp ${dir}/*.[chi] ${dir}/*.in ${dir}/*.cu
#    )
#    list(APPEND ${cxx_sources} ${s})
#endmacro()

#glob_append_cxx_sources(all_cxx_sources .)
glob_recurse_append_cxx_sources(all_cxx_sources apps)
glob_recurse_append_cxx_sources(all_cxx_sources include)
glob_recurse_append_cxx_sources(all_cxx_sources libs)
glob_recurse_append_cxx_sources(all_cxx_sources sdks)
glob_recurse_append_cxx_sources(all_cxx_sources source)
glob_recurse_append_cxx_sources(all_cxx_sources src)
glob_recurse_append_cxx_sources(all_cxx_sources test)
if (${PROJECT_NAME} STREQUAL "util")
    glob_recurse_append_cxx_sources(all_cxx_sources basiccodesupport)
    glob_recurse_append_cxx_sources(all_cxx_sources buildenv)
    glob_recurse_append_cxx_sources(all_cxx_sources include)
    glob_recurse_append_cxx_sources(all_cxx_sources linklib)
    glob_recurse_append_cxx_sources(all_cxx_sources mem)
    glob_recurse_append_cxx_sources(all_cxx_sources string)
endif ()

message("CLANGFORMAT_VER: ${CLANGFORMAT_VER}")

find_program(CLANG_FORMAT "clang-format-${CLANGFORMAT_VER}")
if (CLANG_FORMAT)
    add_custom_target(
        clang-format-${PROJECT_NAME}
        COMMAND ${CLANG_FORMAT}
        -i
        -style=file
        ${all_cxx_sources}
    )
endif ()
