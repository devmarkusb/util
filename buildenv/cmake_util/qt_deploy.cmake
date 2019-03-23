# File to include for cmake Qt postbuild deployment dependency additions.

# \Param path_to_qml needs to be the one pointing to the root of *all* qml files, containing qml.qrc also.
macro(too_qt_deploy target path_to_qml)
    set(impl_TargetFileName $<TARGET_FILE_NAME:${target}>)
    if (TOO_WINDOWS)
        set(impl_QmlDirParam "--qmldir ")
    else ()
        set(impl_QmlDirParam "-qmldir=")
    endif ()
    if (NOT "${path_to_qml}" STREQUAL "")
        set(impl_QmlSourcesToUse "${impl_QmlDirParam}${path_to_qml}")
    else ()
        set(impl_QmlSourcesToUse)
    endif ()

    if ("${TOO_DEPLOY_TARGET}" STREQUAL "uwp")
        # Note: rename instead of copy hinders MSVC to copy (again)
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${impl_TargetFileName} ${TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR}
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        )
    endif ()

    if (TOO_WINDOWS)
        set(impl_TargetBuildType $<IF:$<CONFIG:Debug>,--debug,--release>)

        add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND "${TOO_CMAKE_UTIL_DIR}/assets/qt_deploy_autodepends_win.bat"
                "${TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR}" "${TOO_QT_VER_COMP_PATH}/bin"
                ${impl_TargetBuildType} -qml -widgets ${impl_QmlSourcesToUse} ${impl_TargetFileName}
        )
    elseif (TOO_LINUX AND "${TOO_DEPLOY_TARGET}" STREQUAL "linuxAppDir")
        add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND "${TOO_CMAKE_UTIL_DIR}/assets/qt_deploy_autodepends_linux.sh"
            "${TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}/usr/share/applications/${impl_TargetFileName}.desktop"
            -bundle-non-qt-libs
            -qmake="${TOO_QT_VER_COMP_PATH}/bin/qmake"
            ${impl_QmlSourcesToUse}
            -verbose=3
            WORKING_DIRECTORY "${TOO_CMAKE_UTIL_DIR}/assets"
        )
        set(TARGET_NAME ${target})
        configure_file(
            "${TOO_CMAKE_UTIL_DIR}/assets/AppRun.in"
            "${CMAKE_CURRENT_BINARY_DIR}/AppRun"
            @ONLY
        )
        file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/cp_perm_apprun.cmake"
            "file(
                COPY ${CMAKE_CURRENT_BINARY_DIR}/AppRun
                DESTINATION ${TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}
                FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
            )"
        )
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cp_perm_apprun.cmake)
        add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND "${TOO_CMAKE_UTIL_DIR}/assets/appimagetool-x86_64.AppImage"
            "${target}.app"
            WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
        )
        if (TOO_DEPLOYMENT_BUILD)
            file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/${target}_artifacts/linux/)
            add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/*.AppImage
                ${CMAKE_SOURCE_DIR}/${target}_artifacts/linux/
            )
        endif ()
    elseif (TOO_MACOS)
        add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND "${TOO_CMAKE_UTIL_DIR}/assets/qt_deploy_autodepends_macos.sh"
                "${TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}" "${TOO_QT_VER_COMP_PATH}/bin"
                -verbose=3 -dmg ${impl_QmlSourcesToUse}
        )
    endif ()
endmacro()
