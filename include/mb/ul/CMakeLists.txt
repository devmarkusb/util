file(GLOB_RECURSE mb_util_public_headers CONFIGURE_DEPENDS "*.hpp" "*.h")

list(
    REMOVE_ITEM mb_util_public_headers
    "${CMAKE_CURRENT_SOURCE_DIR}/buildenv/gtest_utils.hpp"
)

set(mb_util_generated_include_dir
    "${CMAKE_CURRENT_BINARY_DIR}/generated/include"
)

set(MB_UTIL_GENERATED_CONFIG_HEADER
    "${mb_util_generated_include_dir}/mb/ul/buildenv/config_gen.hpp"
)
file(MAKE_DIRECTORY "${mb_util_generated_include_dir}/mb/ul/buildenv")
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/buildenv/config_gen.hpp.in"
    "${MB_UTIL_GENERATED_CONFIG_HEADER}"
    @ONLY
)

target_sources(
    mb.util
    PUBLIC
        FILE_SET HEADERS
            BASE_DIRS
                "${PROJECT_SOURCE_DIR}/include"
                "${mb_util_generated_include_dir}"
            FILES ${mb_util_public_headers} "${MB_UTIL_GENERATED_CONFIG_HEADER}"
)
