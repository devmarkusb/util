# File to include for cmake Qt postbuild deployment dependency additions.

# \Param path_to_qml needs to be the one pointqt_deployImpl_TargetFileNameing to the root of *all* qml files, containing qml.qrc also.
macro(too_qt_deploy target path_to_qml)
    if (TOO_WINDOWS)
        set(qt_deployImpl_TargetFileName $<TARGET_FILE_NAME:${target}>)
        set(qt_deployImpl_QmlDirParam "--qmldir ")
    else ()
        set(qt_deployImpl_QmlDirParam "-qmldir=")
    endif ()
    if (NOT "${path_to_qml}" STREQUAL "")
        set(qt_deployImpl_QmlSourcesToUse "${qt_deployImpl_QmlDirParam}${path_to_qml}")
    else ()
        set(qt_deployImpl_QmlSourcesToUse)
    endif ()

    if ("${TOO_DEPLOY_TARGET}" STREQUAL "uwp")
        # Note: rename instead of copy hinders MSVC to copy (again)
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${qt_deployImpl_TargetFileName} ${TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR}
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        )
    endif ()

    if (TOO_WINDOWS)
        set(qt_deployImpl_TargetBuildType $<IF:$<CONFIG:Debug>,--debug,--release>)

        add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND "${TOO_CMAKE_UTIL_DIR}/assets/qt_deploy_autodepends_win.bat"
                "${TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR}" "${TOO_QT_VER_COMP_PATH}/bin"
                ${qt_deployImpl_TargetBuildType} -qml -widgets ${qt_deployImpl_QmlSourcesToUse} ${qt_deployImpl_TargetFileName}
        )
    elseif (TOO_MACOS)
        add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND "${TOO_CMAKE_UTIL_DIR}/assets/qt_deploy_autodepends_macos.sh"
                "${TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}" "${TOO_QT_VER_COMP_PATH}/bin"
                -verbose=3 -dmg ${qt_deployImpl_QmlSourcesToUse}
        )
    endif ()
endmacro()
