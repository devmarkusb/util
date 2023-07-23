# Include in your project root(s) once. Generates 'clang-tidy' targets.
# Before that, optionally define
# - UL_CLANG_TIDY_DIRS with a quoted ;-separated list (most reasonable ones are already implicitly set)
# - UL_USE_DEFAULT_CLANG_TIDY_CONFIG ON if you want to overwrite the .clang-tidy file in your project dir by this
#   repos' default.
cmake_minimum_required(VERSION 3.14)

include(FetchContent)

FetchContent_Declare(mb-clangtidy
    GIT_REPOSITORY "https://github.com/devmarkusb/clangtidy"
    GIT_TAG origin/HEAD
    GIT_SHALLOW  ON
    GIT_PROGRESS ON
    )

FetchContent_MakeAvailable(mb-clangtidy)

if (UL_USE_DEFAULT_CLANG_TIDY_CONFIG)
    # of course the copied files are not intended to be edited
    file(COPY ${mb-clangtidy_SOURCE_DIR}/.clang-tidy DESTINATION ${PROJECT_SOURCE_DIR}/)
endif()

# workaround as long as sophisticated regex'es excluding headers don't work in .clang-tidy
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/_deps)
file(COPY ${mb-clangtidy_SOURCE_DIR}/disabled/.clang-tidy DESTINATION ${CMAKE_BINARY_DIR}/_deps/)

set(cxx_dirs_general "apps;include;source;src;test")

string(CONCAT cxx_dirs "${cxx_dirs_general}" ";${UL_CLANG_TIDY_DIRS}")
add_clang_tidy_project_target("${cxx_dirs}")
