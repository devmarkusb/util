FetchContent_Declare(
    easyloggingpp
    GIT_REPOSITORY https://github.com/devmarkusb/easyloggingpp
    GIT_TAG origin/HEAD
    GIT_SHALLOW ON
    GIT_PROGRESS ON
    UPDATE_DISCONNECTED ON
    SOURCE_SUBDIR cmake/wealthplanner-skip-add-subdirectory
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

if(MSVC)
    set_source_files_properties(
        ${easyloggingpp_SOURCE_FILES}
        DIRECTORY ${CMAKE_SOURCE_DIR}
        PROPERTIES
            COMPILE_DEFINITIONS "WIN32_LEAN_AND_MEAN;ELPP_WINSOCK2"
            COMPILE_OPTIONS /w
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "AppleClang|Clang|GNU")
    set_source_files_properties(
        ${easyloggingpp_SOURCE_FILES}
        DIRECTORY ${CMAKE_SOURCE_DIR}
        PROPERTIES COMPILE_OPTIONS -w
    )
endif()
