# tools specific for linux deployment

# inside a macro the wrong (caller's) one would be used
set(impl_CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

# prepares the app dir for linux deployment with the executable
macro(ul_linux_prepare_package TARGET_NAME DISPLAYNAME)
    if (UL_LINUX AND "${UL_DEPLOY_TARGET}" STREQUAL "linuxAppDir")
        file(MAKE_DIRECTORY ${UL_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE})
        file(MAKE_DIRECTORY ${UL_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}/usr)
        file(MAKE_DIRECTORY ${UL_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}/usr/bin)
        file(MAKE_DIRECTORY ${UL_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}/usr/lib)
        file(MAKE_DIRECTORY ${UL_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}/usr/share)
        file(MAKE_DIRECTORY ${UL_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}/usr/share/applications)
        file(MAKE_DIRECTORY ${UL_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}/usr/share/icons)
        file(MAKE_DIRECTORY ${UL_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}/usr/share/icons/hicolor)
        file(MAKE_DIRECTORY ${UL_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}/usr/share/icons/hicolor/256x256)
        file(MAKE_DIRECTORY ${UL_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}/usr/share/icons/hicolor/256x256/apps)

        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${TARGET_NAME}>
            ${UL_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR}
        )

        set(TARGET_NAME ${TARGET_NAME})
        set(DISPLAYNAME ${DISPLAYNAME})
        configure_file(
            "${impl_CURRENT_LIST_DIR}/assets/app.desktop.in"
            "${UL_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}/usr/share/applications/${TARGET_NAME}.desktop"
            @ONLY
        )
    endif ()
endmacro()
