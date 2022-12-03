set(UL_ANDROID_TOOLCHAIN "qt" CACHE STRING "toolchain implementation to use for Android target")
# Where 'qt' means qt-android-cmake, 'qt_standalone' let's you pass custom parameters to cmake,
# and 'native' loads the toolchain file from NDK.
set_property(CACHE UL_ANDROID_TOOLCHAIN PROPERTY STRINGS qt qt_standalone native)

include(${CMAKE_CURRENT_LIST_DIR}/../defs.cmake)

if ("${UL_ANDROID_TOOLCHAIN}" STREQUAL "qt" OR "${UL_ANDROID_TOOLCHAIN}" STREQUAL "qt_standalone")
    # A minimum API level 13 is suggested by Qt documentation. But qt-android-cmake seems to demand at least 16.
    set(ANDROID_NATIVE_API_LEVEL "16" CACHE STRING "" FORCE)
endif ()

if ("${UL_ANDROID_TOOLCHAIN}" STREQUAL "native")
    include($ENV{ANDROID_NDK}/build/cmake/android.toolchain.cmake)
elseif ("${UL_ANDROID_TOOLCHAIN}" STREQUAL "qt" OR "${UL_ANDROID_TOOLCHAIN}" STREQUAL "qt_standalone")
    if ("${UL_ANDROID_TOOLCHAIN}" STREQUAL "qt")
        if (WIN32)
            set(CMAKE_MAKE_PROGRAM "$ENV{ANDROID_NDK}/prebuilt/windows/bin/make.exe" CACHE STRING "" FORCE)
        elseif (UL_LINUX)
            set(CMAKE_MAKE_PROGRAM "$ENV{ANDROID_NDK}/prebuilt/linux-x86_64/bin/make" CACHE STRING "" FORCE)
        endif ()
    endif ()

    include(${CMAKE_CURRENT_LIST_DIR}/../sdks/qt-android-cmake/toolchain/android.toolchain.cmake)
else ()
    message(FATAL_ERROR "invalid choice of UL_ANDROID_TOOLCHAIN")
endif ()

if ("${UL_ANDROID_TOOLCHAIN}" STREQUAL "qt" OR "${UL_ANDROID_TOOLCHAIN}" STREQUAL "qt_standalone")
    # Qt bug workaround https://bugreports.qt.io/browse/QTBUG-54666
    get_property(cxx_features GLOBAL PROPERTY CMAKE_CXX_KNOWN_FEATURES)
    set(CMAKE_CXX_COMPILE_FEATURES ${cxx_features})
    set(GLOBAL PROPERTY CMAKE_C_COMPILE_FEATURES ${cxx_features})
endif ()
