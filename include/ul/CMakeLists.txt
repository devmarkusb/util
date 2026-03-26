file(GLOB_RECURSE mb_util_public_headers CONFIGURE_DEPENDS "*.hpp" "*.h")

target_sources(
    mb.util
    PUBLIC
        FILE_SET HEADERS
            FILES ${mb_util_public_headers} "${MB_UTIL_GENERATED_CONFIG_HEADER}"
)
