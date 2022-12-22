FetchContent_Declare(
     QtAndroidCMake
     GIT_REPOSITORY https://github.com/LaurentGomila/qt-android-cmake
     GIT_TAG        origin/HEAD
)

FetchContent_MakeAvailable(QtAndroidCMake)
message(QtAndroidCMake_SOURCE_DIR: ${QtAndroidCMake_SOURCE_DIR})