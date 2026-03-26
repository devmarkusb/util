### OS ###
# Note, in code we call them MB_UL_OS_... instead of MB_UL_...

if(WIN32)
    # one might still prefer the very common WIN32
    set(MB_UL_WINDOWS TRUE)
endif()

if(APPLE OR (${CMAKE_SYSTEM_NAME} MATCHES "Darwin"))
    set(MB_UL_MACOS TRUE)
endif()

if(UNIX AND NOT MB_UL_MACOS)
    set(MB_UL_LINUX TRUE)
endif()

if(UNIX)
    set(MB_UL_UNIX TRUE)
endif()

# so far defined by toolchain we use
if(ANDROID)
    set(MB_UL_ANDROID TRUE)
endif()

# indicating an 'app' in the common restricted sense compared to desktop software
if(MB_UL_ANDROID)
    set(MB_UL_APP TRUE)
endif()
