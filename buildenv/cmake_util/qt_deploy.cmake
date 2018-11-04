# File to include for cmake Qt postbuild deployment dependency additions.

# \Param path_to_qml needs to be the one pointing to the root of *all* qml files, containing qml.qrc also.
macro(too_qt_deploy target path_to_qml)
    if (WIN32)
        set(TargetFileName $<TARGET_FILE_NAME:${target}>)
        
        if (TOO_DEPLOY_TARGET STREQUAL "uwp")
            add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory ${OutputDir_bin}/${BIN_PACKAGE_SUBDIR}
            )
            # Note: rename instead of copy hinders MSVC to copy (again)
            add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy ${TargetFileName} ${OutputDir_bin}/${BIN_PACKAGE_SUBDIR}
                WORKING_DIRECTORY ${OutputDir_bin}
            )
                
            set(RuntimeOutputDirectoryToUse "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BIN_PACKAGE_SUBDIR}")
        else ()
            set(RuntimeOutputDirectoryToUse "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
        endif ()

        set(TargetBuildType_for_qtdeploy $<IF:$<CONFIG:Debug>:--debug,--release>)

        if (NOT "${path_to_qml}" STREQUAL "")
            set(QmlSourcesToUse --qmldir "${path_to_qml}")
        else ()
            set(QmlSourcesToUse)
        endif ()
        
        add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND "${TOO_CMAKE_UTIL_DIR}/assets/qt_deploy_autodepends_win.bat" "${RuntimeOutputDirectoryToUse}" "${TOO_QT_VER_COMP_PATH}/bin"
                ${TargetBuildType_for_qtdeploy} -qml -widgets ${QmlSourcesToUse} ${TargetFileName}
        )
    endif ()
endmacro()
