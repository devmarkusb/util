# Include in your project root(s) once. Generates 'clang-format' targets and more, see clangformat/README.md
cmake_minimum_required(VERSION 3.14)

set(UL_ADDITIONAL_CLANG_FORMAT_DIRS "" CACHE STRING "provide ;-separated list in quotes as a whole")

include(FetchContent)

FetchContent_Declare(mb-clangformat
    GIT_REPOSITORY "https://github.com/devmarkusb/clangformat"
    GIT_TAG origin/HEAD
    GIT_SHALLOW  ON
    GIT_PROGRESS ON
    )

FetchContent_MakeAvailable(mb-clangformat)

# of course the copied files are not intended to be edited
file(COPY ${mb-clangformat_SOURCE_DIR}/.clang-format DESTINATION ${PROJECT_SOURCE_DIR}/)
file(COPY ${mb-clangformat_SOURCE_DIR}/clangformat.sh DESTINATION ${PROJECT_SOURCE_DIR}/
        FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

set(cxx_dirs_general "apps;include;libs;sdks;source;src;test")
if ("${PROJECT_NAME}" STREQUAL "util")
    set(cxx_dirs_util ";basiccodesupport;buildenv;linklib;mem;string")
endif ()

string(CONCAT cxx_dirs "${cxx_dirs_general}" "${cxx_dirs_util}")
string(CONCAT cxx_dirs "${cxx_dirs}" ";${UL_ADDITIONAL_CLANG_FORMAT_DIRS}")

add_clang_format_project_target("${cxx_dirs}")
