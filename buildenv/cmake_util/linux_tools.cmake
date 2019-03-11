# tools specific for linux deployment

# inside a macro the wrong (caller's) one would be used
set(impl_CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

# prepares the app dir for linux deployment with the executable
macro(too_linux_prepare_package TARGET_NAME DISPLAYNAME)
    if (TOO_LINUX AND "${TOO_DEPLOY_TARGET}" STREQUAL "linuxAppDir")
        file(MAKE_DIRECTORY ${TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE})
        file(MAKE_DIRECTORY ${TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}/usr)
        file(MAKE_DIRECTORY ${TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}/usr/bin)
        file(MAKE_DIRECTORY ${TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}/usr/lib)
        file(MAKE_DIRECTORY ${TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}/usr/share)
        file(MAKE_DIRECTORY ${TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}/usr/share/applications)
        file(MAKE_DIRECTORY ${TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}/usr/share/icons)
        file(MAKE_DIRECTORY ${TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}/usr/share/icons/hicolor)
        file(MAKE_DIRECTORY ${TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}/usr/share/icons/hicolor/256x256)
        file(MAKE_DIRECTORY ${TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}/usr/share/icons/hicolor/256x256/apps)

        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${TARGET_NAME}>
            ${TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR}
        )

        set(TARGET_NAME ${TARGET_NAME})
        set(DISPLAYNAME ${DISPLAYNAME})
        configure_file(
            "${impl_CURRENT_LIST_DIR}/assets/app.desktop"
            "${TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}/share/applications/${target}.desktop"
            @ONLY
        )
    endif ()
endmacro()
