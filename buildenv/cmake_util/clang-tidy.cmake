# Include in your project root(s) once. Generates 'clang-tidy' targets.
# Before that, optionally define
# - UL_ADDITIONAL_CLANG_TIDY_DIRS with a quoted ;-separated list
# - UL_USE_DEFAULT_CLANG_TIDY_CONFIG ON if you want to overwrite the .clang-tidy file in your project dir by this
#   repos' default.
cmake_minimum_required(VERSION 3.14)

include(FetchContent)

FetchContent_Declare(mb-clangtidy
    GIT_REPOSITORY "https://github.com/devmarkusb/clangtidy"
    GIT_TAG origin/HEAD
    GIT_SHALLOW ON
    )

FetchContent_MakeAvailable(mb-clangtidy)

if (UL_USE_DEFAULT_CLANG_TIDY_CONFIG)
    # of course the copied files are not intended to be edited
    file(COPY ${mb-clangtidy_SOURCE_DIR}/.clang-tidy DESTINATION ${PROJECT_SOURCE_DIR}/)
endif()

set(cxx_dirs_general "apps;include;libs;sdks;source;src;test")
if (${PROJECT_NAME} STREQUAL "util")
    set(cxx_dirs_util ";basiccodesupport;buildenv;linklib;mem;string")
endif ()

string(CONCAT cxx_dirs "${cxx_dirs_general}" "${cxx_dirs_util}")
if (UL_ADDITIONAL_CLANG_TIDY_DIRS)
    string(CONCAT cxx_dirs "${cxx_dirs}" ";${UL_ADDITIONAL_CLANG_TIDY_DIRS}")
endif()

add_clang_tidy_project_target("${cxx_dirs}")
