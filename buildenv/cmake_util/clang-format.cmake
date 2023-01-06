# include in your project root once; generates a 'clang-format' target

set(cxx_dirs_general "apps;include;libs;sdks;source;src;test")
if (${PROJECT_NAME} STREQUAL "util")
    set(cxx_dirs_util ";basiccodesupport;buildenv;linklib;mem;string")
endif ()

string(CONCAT cxx_dirs "${cxx_dirs_general}" "${cxx_dirs_util}")

add_clang_format_project_target("${cxx_dirs}")
