# File to include for using asio (without boost).
# Usage:
# 1) Add to your app's CMakeLists.txt
#       ul_include(asio.cmake)
#   and add to target with e.g.
#       ul_use_asio(${TargetAsLibForTest} ${CMAKE_SOURCE_DIR}/3rdparty/asio-1.12.2.tar.gz
#           MD5=43c53b8d255edc385dc4e704e59b3641)

macro(ul_use_asio target path_to_zip hash)
    if (NOT TARGET asio)
        include(ExternalProject)
        set(asio_PREFIX ${CMAKE_BINARY_DIR}/3rdparty/asio)
        set(asio_INCLUDE ${asio_PREFIX}/src/asio/include CACHE INTERNAL "" FORCE)
        ExternalProject_Add(asio
                URL                ${path_to_zip}
                URL_HASH           ${hash}
                PREFIX             ${asio_PREFIX}
                CONFIGURE_COMMAND ""
                BUILD_COMMAND ""
                INSTALL_COMMAND ""
                DOWNLOAD_EXTRACT_TIMESTAMP true
        )
    endif ()
    add_dependencies(${target} asio)
    target_include_directories(${target} SYSTEM PUBLIC ${asio_INCLUDE})
    target_compile_definitions(${target} PUBLIC -DASIO_STANDALONE -DASIO_HAS_STD_INVOKE_RESULT)
endmacro()
