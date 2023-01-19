# Usage
#   After including this file, call add_qt_android_apk like documented in qt-android-cmake README.
#   Besides necessary CMake parameters for desktop builds, you need (e.g.):
#     -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake
#     -DANDROID_STL="c++_shared"
#     -DANDROID_ABI="x86"
#     -DANDROID_PLATFORM=19
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
#   7. I just can't it working. Any general ideas?
#      - downgrade Qt version, 5.9.1 was working some day
#      - try gradle updates (if you reach that stage at all)

if (NOT DEFINED ENV{ANDROID_NDK})
    message(FATAL_ERROR "Configure env variable ANDROID_NDK to point to e.g. android-ndk-r25b dir.")
endif()

if (NOT DEFINED ENV{ANDROID_SDK})
    message(FATAL_ERROR "Configure env variable ANDROID_SDK to point to Android SDK install dir.")
endif()

if (NOT DEFINED ENV{JAVA_HOME})
    message(FATAL_ERROR "Configure env variable JAVA_HOME to point to a Java jvm like e.g. java-8-openjdk-amd64.")
endif()

FetchContent_Declare(
     QtAndroidCMake
     GIT_REPOSITORY https://github.com/devmarkusb/qt-android-cmake
     GIT_TAG        c9a97464
)

FetchContent_MakeAvailable(QtAndroidCMake)
