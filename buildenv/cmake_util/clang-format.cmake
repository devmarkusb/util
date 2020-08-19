# include in your project root once; generates a 'clang-format' target

set(CLANG_FORMAT_VER 10)

macro(glob_recurse_append_cxx_sources cxx_sources dir)
    file(GLOB_RECURSE
        s
        ${dir}/*.[chi]pp ${dir}/*.[chi] ${dir}/*.in ${dir}/*.cu
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

find_program(CLANG_FORMAT "clang-format-${CLANG_FORMAT_VER}")
if (CLANG_FORMAT)
    add_custom_target(
        clang-format
        COMMAND ${CLANG_FORMAT}
        -i
        -style=file
        ${all_cxx_sources}
    )
endif ()
