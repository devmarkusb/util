# File refers to recent target given by variable TOO_TEMP_TARGET, also the qml dir of the current
# target needs to be 'passed' by TOO_TEMP_QML_DIR (could be unset also).

if (WIN32)
    set(TargetFileName $<TARGET_FILE_NAME:${TOO_TEMP_TARGET}>)
    
    if (TOO_DEPLOY_TARGET STREQUAL "uwp")
        add_custom_command(TARGET ${TOO_TEMP_TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${OutputDir_bin}/${BIN_PACKAGE_SUBDIR}
        )
        # Note: rename instead of copy hinders MSVC to copy (again)
        add_custom_command(TARGET ${TOO_TEMP_TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${TargetFileName} ${OutputDir_bin}/${BIN_PACKAGE_SUBDIR}
            WORKING_DIRECTORY ${OutputDir_bin}
        )
            
        set(RuntimeOutputDirectoryToUse "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BIN_PACKAGE_SUBDIR}")
    else ()
        set(RuntimeOutputDirectoryToUse "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    endif ()

    if (NOT MSVC)
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(TargetBuildType_for_qtdeploy --debug)
        else ()
            set(TargetBuildType_for_qtdeploy --release)
        endif ()
    else ()
        set(TargetBuildType_for_qtdeploy)
    endif ()
    
    if (DEFINED TOO_TEMP_QML_DIR)
        set(QmlSourcesToUse --qmldir "${TOO_TEMP_QML_DIR}")
    else ()
        set(QmlSourcesToUse)
    endif ()
    
    add_custom_command(
        TARGET ${TOO_TEMP_TARGET} POST_BUILD
        COMMAND "${CMAKE_CURRENT_LIST_DIR}/assets/qt_deploy_autodepends_win.bat" "${RuntimeOutputDirectoryToUse}" "${TOO_QT_VER_COMP_PATH}/bin"
            ${TargetBuildType_for_qtdeploy} -qml -widgets ${QmlSourcesToUse} ${TargetFileName}
    )
endif ()
