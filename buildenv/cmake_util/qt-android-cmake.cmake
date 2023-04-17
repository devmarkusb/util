# Usage
#   After including this file, call add_qt_android_apk like documented in qt-android-cmake README (see FetchContent
#   below).
#   Besides necessary CMake parameters for desktop builds, you need (e.g.):
#     ndk_path=$dev_sdk_path/Android/Sdk/ndk/21.3.6528147
#     -DCMAKE_TOOLCHAIN_FILE=$ndk_path/build/cmake/android.toolchain.cmake
#     -DUL_JAVA_HOME=/usr/lib/jvm/java-11-openjdk-amd64
#     -DUL_ANDROID_NDK=$ndk_path
#     -DANDROID_STL="c++_shared"
#     -DANDROID_ABI="arm64-v8a"
#     -DANDROID_PLATFORM=28
#     -DUL_ANDROID_SIGN_KEY_PATH=$sign_key_path
#     -DUL_ANDROID_SIGN_KEY_PASS=$sign_key_pass
#
# FAQs
#   1. 'STL library does not exist at ...'?
#      There might be libs missing at the place where they are expected. But usually they are there, just at a
#      completely different place but inside the AndroidNDK. Search for the .so, then provide a link to the dir or so.
#   2. 'Command does not exist: /.../android-ndk-r25b/toolchains/llvm-/prebuilt'?
#      What about the 'llvm-'? Just provide a link llvm- -> llvm.
#   3. 'No platform plugin, neither libqtforandroid.so or libqtforandroidGL.so, included in package.'?
#      This might get solved by 2).
#   4. 'Could not determine java version from '11.0.2'?
#      Use an older Java version, gradle has a compatibility matrix:
#      https://docs.gradle.org/current/userguide/compatibility.html
#      Also cf. https://stackoverflow.com/questions/54358107/gradle-could-not-determine-java-version-from-11-0-2
#   5. 'Could not find method google() for arguments [] on repository container'?
#      Try edit distributionUrl in gradle-wrapper.properties to newer gradle version (3.4->4.6?).
#      Be careful with big version jumps because of incompatibilities.
#   6. CMake error 'add_custom_target cannot create target "apk" because another target with' in a Qt cmake file?
#      Manually apply 'if (NOT TARGET apk)...' patches to the mentioned file.
#   7. White screen opn Android?
#      - see 8.
#      - qml-root-path in android_deployment_settings.json?
#   8. I just can't it working. Any general ideas?
#      - best thing you can do: experiment with original Qt examples in QtCreator with qmake, get some version
#        mixture working, compare result build dir with your cmake build (might even want to move the example code
#        over if entirely desperate)
#      - downgrade Qt version, 5.9.1 was working some day
#      - try gradle updates (if you reach that stage at all)

set(UL_ANDROID_NDK_HELPSTR "Path to e.g. android-ndk-r25b dir.")
set(UL_ANDROID_NDK "" CACHE STRING "${UL_ANDROID_NDK_HELPSTR}")

set(UL_ANDROID_SDK_HELPSTR "Path to Android SDK install dir.")
set(UL_ANDROID_SDK "" CACHE STRING "${UL_ANDROID_SDK_HELPSTR}")

set(UL_JAVA_HOME_HELPSTR "Path to a Java jvm like e.g. java-8-openjdk-amd64.")
set(UL_JAVA_HOME "" CACHE STRING "${UL_JAVA_HOME_HELPSTR}")

if (UL_ANDROID_NDK STREQUAL "")
    if (DEFINED ENV{ANDROID_NDK})
        set(UL_ANDROID_NDK $ENV{ANDROID_NDK} CACHE STRING "${UL_ANDROID_NDK_HELPSTR}" FORCE)
    endif()
else()
    set(ENV{ANDROID_NDK} "${UL_ANDROID_NDK}")
endif()

if (NOT DEFINED ENV{ANDROID_NDK})
    message(FATAL_ERROR "Configure cache variable UL_ANDROID_NDK or env variable ANDROID_NDK to point to e.g. \
android-ndk-r25b dir.")
endif()

if (UL_ANDROID_SDK STREQUAL "")
    if (DEFINED ENV{ANDROID_SDK})
        set(UL_ANDROID_SDK $ENV{ANDROID_SDK} CACHE STRING "${UL_ANDROID_SDK_HELPSTR}" FORCE)
    endif()
else()
    set(ENV{ANDROID_SDK} "${UL_ANDROID_SDK}")
endif()

if (NOT DEFINED ENV{ANDROID_SDK})
    message(FATAL_ERROR "Configure cache variable UL_ANDROID_SDK or env variable ANDROID_SDK to point to Android SDK \
install dir.")
endif()

if (UL_JAVA_HOME STREQUAL "")
    if (DEFINED ENV{JAVA_HOME})
        set(UL_JAVA_HOME $ENV{JAVA_HOME} CACHE STRING "${UL_JAVA_HOME_HELPSTR}" FORCE)
    endif()
else()
    set(ENV{JAVA_HOME} "${UL_JAVA_HOME}")
endif()

if (NOT DEFINED ENV{JAVA_HOME})
    message(FATAL_ERROR "Configure cache variable UL_JAVA_HOME or env variable JAVA_HOME to point to a Java jvm like \
e.g. java-8-openjdk-amd64.")
endif()

if (DEFINED ANDROID_PLATFORM)
    set(ANDROID_PLATFORM_LEVEL ${ANDROID_PLATFORM})
endif()

if (NOT DEFINED QT_ANDROID_SCREEN_ORIENTATION)
    set(QT_ANDROID_SCREEN_ORIENTATION "unspecified")
endif()

FetchContent_Declare(
     QtAndroidCMake
     GIT_REPOSITORY https://github.com/devmarkusb/qt-android-cmake
     GIT_TAG        c9a97464
)

FetchContent_MakeAvailable(QtAndroidCMake)
