# include in your project root once; generates a 'clang-format' target

macro(glob_recurse_append_cxx_sources cxx_sources dir)
    file(GLOB_RECURSE
        s
        ${dir}/*.[chi]pp ${dir}/*.[chi]xx ${dir}/*.cc ${dir}/*.hh ${dir}/*.ii ${dir}/*.[CHI] ${dir}/*.[chi]
    )
    list(APPEND ${cxx_sources} ${s})
endmacro()

#macro(glob_append_cxx_sources cxx_sources dir)
#    file(GLOB
#        s
#        ${dir}/*.[chi]pp ${dir}/*.[chi]xx ${dir}/*.cc ${dir}/*.hh ${dir}/*.ii ${dir}/*.[CHI]
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

find_program(CLANG_FORMAT "clang-format")
if (CLANG_FORMAT)
    add_custom_target(
        clang-format
        COMMAND /usr/bin/clang-format
        -i
        -style=file
        ${all_cxx_sources}
    )
endif ()
