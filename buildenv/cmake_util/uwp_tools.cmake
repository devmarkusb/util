# file to include to add UWP tools

set(UL_IMPL_UWP_TOOLS_CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})


# can be used as default parameter for ul_obtain_UWP_resources
set(UL_CMAKE_INC_UWP_TOOLS_DEF_ICO ${CMAKE_CURRENT_LIST_DIR}/assets
        CACHE INTERNAL "can be used as default parameter for ul_obtain_UWP_resources of UL_CMAKE_INC_UWP_TOOLS" FORCE)

# Usage (e.g.):
#   ul_obtain_UWP_resources("App name" "Enterprise" "34799Enterprise.Appname" "" "348B82EC-3E83-499C-91B3-A2DAB20E041A" ""
#       "${PROJECT_VERSION}"
#       "${CMAKE_SOURCE_DIR}/installer/_content_for_bindir"
#       "#336699"
#       "<Resource Language=\"en\" /><Resource Language=\"de\" />"
#       Target_UWP_ASSETS)
#   For quick tests you can also use ${UL_CMAKE_INC_UWP_TOOLS_DEF_ICO} for the _ico parent path and pass "" as PUBLISHER_NAME,
#   PHONE_PRODUCT_GUID, PHONE_PUBLISHER_GUID and LANGUAGES.
function(ul_obtain_UWP_resources
            DISPLAYNAME
            PUBLISHER_DISPLAYNAME
            PACKAGE_NAME
            PUBLISHER_NAME
            PHONE_PRODUCT_GUID
            PHONE_PUBLISHER_GUID
            VERSION
            FULLPATH_TO_ico_PARENTDIR
            BKG_COLOR
            LANGUAGES
            retRESOURCES)
    if ("${UL_DEPLOY_TARGET}" STREQUAL "uwp")
        if (NOT UL_DEPLOYMENT_BUILD)
            set(DISPLAYNAME "${DISPLAYNAME} - dev")
            set(PACKAGE_NAME "${PACKAGE_NAME}.dev")
        endif ()
        if ("${PUBLISHER_NAME}" STREQUAL "")
            set(PUBLISHER_NAME "CN=CMake Test Cert")
        endif ()
        if ("${PHONE_PRODUCT_GUID}" STREQUAL "")
            set(PHONE_PRODUCT_GUID "348B82EC-3E83-499C-91B3-A2DAB20E041A")
        endif ()
        if ("${PHONE_PUBLISHER_GUID}" STREQUAL "")
            set(PHONE_PUBLISHER_GUID "00000000-0000-0000-0000-000000000000")
        endif ()
        if ("${LANGUAGES}" STREQUAL "")
            set(LANGUAGES "<Resource Language=\"x-generate\" />")
        endif ()
        set(APP_MANIFEST_NAME appxmanifest.xml)
        configure_file(
            "${UL_IMPL_UWP_TOOLS_CURRENT_LIST_DIR}/assets/${APP_MANIFEST_NAME}.in"
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
