# File to include for cmake Qt postbuild deployment dependency additions.

get_filename_component(UL_QT_INSTALL_PREFIX "${Qt6_DIR}" DIRECTORY)
get_filename_component(UL_QT_INSTALL_PREFIX "${UL_QT_INSTALL_PREFIX}" DIRECTORY)
get_filename_component(UL_QT_INSTALL_PREFIX "${UL_QT_INSTALL_PREFIX}" DIRECTORY)

# \param path_to_qml needs to be the one pointing to the root of *all* qml files, containing qml.qrc also.
macro(ul_qt_deploy target path_to_qml)
    set(impl_TargetFileName $<TARGET_FILE_NAME:${target}>)
    if (UL_WINDOWS)
        set(impl_QmlDirParam "--qmldir")
        set(impl_QmlSourcesToUse)
    elseif (NOT UL_ANDROID)
        set(impl_QmlDirParam)
        set(impl_QmlSourcesToUse "-qmldir=")
    endif ()
    if (NOT "${path_to_qml}" STREQUAL "")
        set(impl_QmlSourcesToUse ${impl_QmlSourcesToUse}${path_to_qml})
    else ()
        set(impl_QmlDirParam)
        set(impl_QmlSourcesToUse)
    endif ()

    if ("${UL_DEPLOY_TARGET}" STREQUAL "uwp")
        # Note: rename instead of copy hinders MSVC to copy (again)
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${impl_TargetFileName} ${UL_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR}
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        )
    endif ()

    if (UL_WINDOWS)
        set(impl_TargetBuildType $<IF:$<CONFIG:Debug>,--debug,--release>)

        add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND "${UL_CMAKE_UTIL_DIR}/assets/qt_deploy_autodepends_win.bat"
                "${UL_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR}" "${UL_QT_INSTALL_PREFIX}/bin"
                ${impl_TargetBuildType} -qml -widgets ${impl_QmlDirParam} ${impl_QmlSourcesToUse} ${impl_TargetFileName}
        )
    elseif (UL_LINUX AND "${UL_DEPLOY_TARGET}" STREQUAL "linuxAppDir")
        add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND "${UL_CMAKE_UTIL_DIR}/assets/qt_deploy_autodepends_linux.sh"
            "${UL_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}/usr/share/applications/${impl_TargetFileName}.desktop"
            -bundle-non-qt-libs
            -qmake="${UL_QT_INSTALL_PREFIX}/bin/qmake"
            ${impl_QmlSourcesToUse}
            -verbose=3
            WORKING_DIRECTORY "${UL_CMAKE_UTIL_DIR}/assets"
        )
        set(TARGET_NAME ${target})
        configure_file(
            "${UL_CMAKE_UTIL_DIR}/assets/AppRun.in"
            "${CMAKE_CURRENT_BINARY_DIR}/AppRun"
            @ONLY
        )
        file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/cp_perm_apprun.cmake"
            "file(
                COPY ${CMAKE_CURRENT_BINARY_DIR}/AppRun
                DESTINATION ${UL_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}
                FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
            )"
        )
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cp_perm_apprun.cmake)
        add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND "${UL_CMAKE_UTIL_DIR}/assets/appimagetool-x86_64.AppImage"
            "${target}.app"
            WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
        )
        if (UL_DEPLOYMENT_BUILD)
            file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/${target}_artifacts/linux/)
            add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/*.AppImage
                ${CMAKE_SOURCE_DIR}/${target}_artifacts/linux/
            )
        endif ()
    elseif (UL_MACOS)
        add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND "${UL_CMAKE_UTIL_DIR}/assets/qt_deploy_autodepends_macos.sh"
                "${UL_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}" "${UL_QT_INSTALL_PREFIX}/bin"
                -verbose=3 -dmg ${impl_QmlSourcesToUse}
        )
    endif ()
endmacro()
