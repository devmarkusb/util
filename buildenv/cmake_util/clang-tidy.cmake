# Include in your project root once and before any add_subdirectory calls (to get your hands on everything
# starting from there). Then configure UL_CLANGTIDY=ON.

option(UL_CLANGTIDY "Run build with clang-tidy. A .clang_tidy template is copied to your project root \
if not there yet. Feel free to put it under version control, adapt and use with more sophisticated tools." OFF)

if (NOT UL_CLANGTIDY)
    return()
endif()

set(projsrc ${PROJECT_SOURCE_DIR})
set(currlst ${CMAKE_CURRENT_LIST_DIR})
if (NOT EXISTS ${projsrc}/.clang_tidy)
    file(COPY ${currlst}/clangtidy/.clang-tidy DESTINATION ${projsrc}/)
endif()

# just a preferred version
set(CLANGTIDY_VER 15)

find_program(CLANGTIDY_EXE NAMES clang-tidy-${CLANGTIDY_VER} clang-tidy)

# For all the options see .clang-tidy file. There you can also specify something like
# HeaderFilter: "^.*(?:_deps).*"gm
# which seems to work there but not here.
if (CLANGTIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY
            ${CLANGTIDY_EXE};)
endif()
