# Prefer target_link_libraries(... easyloggingpp_lib) over compiling easyloggingpp_SOURCE_FILES.
# Requires mb-devenv-defaults (mb_devenv_suppress_third_party_warnings).
FetchContent_Declare(
    easyloggingpp
    GIT_REPOSITORY https://github.com/devmarkusb/easyloggingpp
    GIT_TAG origin/HEAD
    GIT_SHALLOW ON
    GIT_PROGRESS ON
    UPDATE_DISCONNECTED ON
    # Do not add upstream easyloggingpp's CMake project; this helper defines easyloggingpp_lib below.
    SOURCE_SUBDIR
    cmake/mb-util-skip-add-subdirectory
)

FetchContent_MakeAvailable(easyloggingpp)

set(easyloggingpp_INCLUDE_DIRS
    ${easyloggingpp_SOURCE_DIR}/src
    CACHE INTERNAL
    "easyloggingpp: include directories"
    FORCE
)

set(easyloggingpp_SOURCE_FILES
    ${easyloggingpp_SOURCE_DIR}/src/easylogging++.cc
    CACHE INTERNAL
    "easyloggingpp: source files"
    FORCE
)

if(NOT TARGET easyloggingpp_lib)
    add_library(
        easyloggingpp_lib
        STATIC
        ${easyloggingpp_SOURCE_DIR}/src/easylogging++.cc
    )
    if(MSVC)
        target_compile_definitions(
            easyloggingpp_lib
            PRIVATE WIN32_LEAN_AND_MEAN ELPP_WINSOCK2
        )
    endif()
    target_include_directories(
        easyloggingpp_lib
        SYSTEM
        PUBLIC ${easyloggingpp_SOURCE_DIR}/src
    )
    mb_devenv_suppress_third_party_warnings(easyloggingpp_lib)
endif()
