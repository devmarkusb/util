include(${CMAKE_CURRENT_LIST_DIR}/detail/defaults.cmake)

# A minimum API level 13 is suggested by Qt documentation. But qt-android-cmake seems to demand at least 16.
set(ANDROID_NATIVE_API_LEVEL "16" CACHE STRING "" FORCE)

if (WIN32)
    set(CMAKE_MAKE_PROGRAM "$ENV{ANDROID_NDK}/prebuilt/windows/bin/make.exe" CACHE STRING "" FORCE)
elseif (LINUX)
    set(CMAKE_MAKE_PROGRAM "$ENV{ANDROID_NDK}/prebuilt/linux-x86_64/bin/make.exe" CACHE STRING "" FORCE)
endif ()

include(${CMAKE_CURRENT_LIST_DIR}/../sdks/qt-android-cmake/toolchain/android.toolchain.cmake)
