# tools specific for macos deployment

# inside a macro the wrong (caller's) one would be used
set(macos_toolsImpl_CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

# prepares the 'manifest' file for macos deployment as a app bundle/package and puts it into the
# base cmake binary dir, ready to be forwarded into the package afterwards
function(too_obtain_macos_resources target TOO_DISPLAYNAME TOO_PACKAGE_NAME)
    if (TOO_MACOS)
        set(macos_toolsImpl_Info_plist Info.plist)
        configure_file(
            "${macos_toolsImpl_CURRENT_LIST_DIR}/assets/${macos_toolsImpl_Info_plist}.in"
            "${CMAKE_BINARY_DIR}/${macos_toolsImpl_Info_plist}"
            @ONLY
        )
    endif ()
endfunction()
