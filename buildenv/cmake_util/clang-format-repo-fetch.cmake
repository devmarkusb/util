cmake_minimum_required(VERSION 3.14)

include(FetchContent)

FetchContent_Declare(mb-clangformat
        GIT_REPOSITORY "https://github.com/devmarkusb/clangformat"
        GIT_TAG origin/HEAD
        GIT_SHALLOW ON
        )

FetchContent_MakeAvailable(mb-clangformat)

# of course the copied file is not intended to be edited
file(COPY ${mb-clangformat_SOURCE_DIR}/.clang-format DESTINATION ${PROJECT_SOURCE_DIR}/)
file(COPY ${mb-clangformat_SOURCE_DIR}/clangformat.sh DESTINATION ${PROJECT_SOURCE_DIR}/
        FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
