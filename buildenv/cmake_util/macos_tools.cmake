# tools specific for macos deployment

# inside a macro the wrong (caller's) one would be used
set(impl_CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

# prepares the 'manifest' file for macos deployment as a app bundle/package and puts it into the
# base cmake binary dir, ready to be forwarded into the package afterwards
function(ul_macos_prepare_package TARGET_NAME DISPLAYNAME PACKAGE_NAME)
    if (TOO_MACOS)
        set(impl_Info_plist Info.plist)
        configure_file(
            "${impl_CURRENT_LIST_DIR}/assets/${impl_Info_plist}.in"
            "${CMAKE_BINARY_DIR}/${impl_Info_plist}"
            @ONLY
        )
    endif ()
endfunction()
