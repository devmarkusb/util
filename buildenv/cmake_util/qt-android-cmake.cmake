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

FetchContent_Declare(
     QtAndroidCMake
     GIT_REPOSITORY https://github.com/LaurentGomila/qt-android-cmake
     GIT_TAG        fa363d60e389afbcd56c31ab4dd9723708b79fbe
)

FetchContent_MakeAvailable(QtAndroidCMake)
