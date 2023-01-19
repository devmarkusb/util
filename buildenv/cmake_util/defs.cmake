### OS ###

if (WIN32)
    # one might still prefer the very common WIN32
    set(UL_WINDOWS TRUE)
endif ()

if (APPLE OR (${CMAKE_SYSTEM_NAME} MATCHES "Darwin"))
    set(UL_MACOS TRUE)
endif ()

if (UNIX AND NOT UL_MACOS)
    set(UL_LINUX TRUE)
endif ()

if (UNIX)
    set(UL_UNIX TRUE)
endif ()

# so far defined by toolchain we use
if (ANDROID)
    set(UL_ANDROID TRUE)
endif ()

# indicating an 'app' in the common restricted sense compared to desktop software
if (UL_ANDROID)
    set(UL_APP TRUE)
endif ()
