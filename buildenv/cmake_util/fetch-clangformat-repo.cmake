cmake_minimum_required(VERSION 3.14)

include(FetchContent)

FetchContent_Declare(mb-clangformat
        GIT_REPOSITORY "https://github.com/MarkusBorris/clangformat"
        GIT_TAG HEAD
        GIT_SHALLOW ON
        )

FetchContent_MakeAvailable(mb-clangformat)

# of course the copied file is not intended to be edited
file(COPY ${mb-clangformat_SOURCE_DIR}/.clang-format DESTINATION ${PROJECT_SOURCE_DIR}/)
