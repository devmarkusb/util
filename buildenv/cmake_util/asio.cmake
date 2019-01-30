# File to include for using asio (without boost).
# Usage:
# 1) Add to your app's CMakeLists.txt
#       too_include(asio.cmake)
#   and add to target with e.g.
#       too_use_asio(${TargetAppNameAsLibForTest} ${CMAKE_SOURCE_DIR}/sdks/asio-1.12.2.tar.gz)

macro(too_use_asio target path_to_zip hash)
    if (NOT TARGET asio)
        include(ExternalProject)
        set(asio_PREFIX ${PROJECT_BINARY_DIR}/sdks/asio)
        set(asio_INCLUDE ${asio_PREFIX}/src/asio/include)
        ExternalProject_Add(asio
                URL                ${path_to_zip}
                URL_HASH           ${hash}
                PREFIX             ${asio_PREFIX}
                CONFIGURE_COMMAND ""
                BUILD_COMMAND ""
                INSTALL_COMMAND ""
        )
    endif ()
    add_dependencies(${target} asio)
    target_include_directories(${target} SYSTEM PUBLIC ${asio_INCLUDE})
    target_compile_definitions(${target} PUBLIC -DASIO_STANDALONE)
endmacro()
