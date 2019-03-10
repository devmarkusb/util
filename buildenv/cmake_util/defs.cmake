### OS ###

if (WIN32)
    # one might still prefer the very common WIN32
    set(TOO_WINDOWS TRUE)
endif ()

if (APPLE OR (${CMAKE_SYSTEM_NAME} MATCHES "Darwin"))
    set(TOO_MACOS TRUE)
endif ()

if (UNIX AND NOT TOO_MACOS)
    set(TOO_LINUX TRUE)
endif ()

if (UNIX)
    set(TOO_UNIX TRUE)
endif ()

# so far defined by toolchain we use
if (ANDROID)
    set(TOO_ANDROID TRUE)
endif ()

# indicating an 'app' in the common restricted sense compared to desktop software
if (TOO_ANDROID)
    set(TOO_APP TRUE)
endif ()


### make sdk path environment variable usable in cmake ###

file(TO_CMAKE_PATH "$ENV{dev_sdk_path}" dev_sdk_path_ENV_CMAKE_PATH_impl)
set(dev_sdk_path_ENV_CMAKE_PATH "${dev_sdk_path_ENV_CMAKE_PATH_impl}" CACHE INTERNAL "cmake normalized path" FORCE)
