set(TOO_IMPL_UWP_TOOLS_CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

if ("${DEPLOY_TARGET}" STREQUAL "uwp")
    set(TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BIN_PACKAGE_SUBDIR}")
else ()
    set(TOO_RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endif ()

# Usage (e.g.):
#   obtain_UWP_resources("348B82EC-3E83-499C-91B3-A2DAB20E041A" "${CMAKE_SOURCE_DIR}/installer/_content_for_bindir" "#336699" TargetAppName_UWP_ASSETS)
function(obtain_UWP_resources PACKAGE_GUID FULLPATH_TO_ico_PARENTDIR BackgroundColor retRESOURCES)
    if ("${DEPLOY_TARGET}" STREQUAL "uwp")
        set(APP_MANIFEST_NAME package.appxmanifest)
        configure_file(
            "${TOO_IMPL_UWP_TOOLS_CURRENT_LIST_DIR}/assets/${APP_MANIFEST_NAME}.in"
            "${CMAKE_CURRENT_BINARY_DIR}/${APP_MANIFEST_NAME}"
            @ONLY
        )
        file(COPY "${FULLPATH_TO_ico_PARENTDIR}/_ico" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
        file(GLOB retRESOURCES_IMPL "${CMAKE_CURRENT_BINARY_DIR}/_ico/*.png")
        list(APPEND retRESOURCES_IMPL "${CMAKE_CURRENT_BINARY_DIR}/${APP_MANIFEST_NAME}")
        set(${retRESOURCES} ${retRESOURCES_IMPL} PARENT_SCOPE)
    else ()
        set(${retRESOURCES} "" PARENT_SCOPE)
    endif ()
endfunction()
