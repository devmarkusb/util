# Include in your project root once and before any add_subdirectory calls (to get your hands on everything
# starting from there). Then configure UL_CLANGTIDY=ON.

option(UL_CLANGTIDY "Run build with clang-tidy. A .clang_tidy template is copied to your project root \
if not there yet. Feel free to put it under version control, adapt and use with more sophisticated tools." OFF)

if (NOT UL_CLANGTIDY)
    return()
endif()

if (NOT EXISTS ${PROJECT_SOURCE_DIR}/.clang_tidy)
    file(COPY ${CMAKE_CURRENT_LIST_DIR}/clangtidy/.clang-tidy DESTINATION ${PROJECT_SOURCE_DIR}/)
endif()

# just a preferred version
set(CLANGTIDY_VER 15)

find_program(CLANGTIDY_EXE NAMES clang-tidy-${CLANGTIDY_VER} clang-tidy)

if (CLANGTIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY
            ${CLANGTIDY_EXE};
            -header-filter=.*;)
endif()
