## build dir naming ###

###  application deployment package dir ###

# impl_BIN_PACKAGE_SUBDIR aims to point right to the dir of the app binary within a app package/bundle dir.
# Often this is also identical to the impl_BIN_PACKAGE_SUBDIR_BASE, the package dir itself.
# Same goes for UL_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR and ..._BASE, which are the full/absolute path
# equivalents and can be used for direct build output.

if("${UL_DEPLOY_TARGET}" STREQUAL "uwp")
    # no-op, don't overwrite, already given by toolchain file
elseif(UL_LINUX AND "${UL_DEPLOY_TARGET}" STREQUAL "linuxAppDir")
    set(impl_BIN_PACKAGE_SUBDIR_BASE "${PROJECT_NAME}.app")
    set(impl_BIN_PACKAGE_SUBDIR "${impl_BIN_PACKAGE_SUBDIR_BASE}/usr/bin")
elseif(UL_MACOS)
    set(impl_BIN_PACKAGE_SUBDIR_BASE "${PROJECT_NAME}.app")
    set(impl_BIN_PACKAGE_SUBDIR
        "${impl_BIN_PACKAGE_SUBDIR_BASE}/Contents/MacOs"
    )
endif()

if(impl_BIN_PACKAGE_SUBDIR)
    set(UL_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${impl_BIN_PACKAGE_SUBDIR_BASE}"
    )
    set(UL_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${impl_BIN_PACKAGE_SUBDIR}"
    )
else()
    set(UL_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    )
    set(UL_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR
        "${UL_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR_BASE}"
    )
endif()
