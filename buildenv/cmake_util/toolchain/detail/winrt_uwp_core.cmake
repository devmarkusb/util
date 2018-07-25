# Provides mandatory configuration foundations you need to set up UWP projects properly (at least working for MSVC generator).

set(CMAKE_SYSTEM_NAME WindowsStore)

set(TOO_DEPLOY_TARGET "uwp" CACHE STRING "Don't change! UWP is already strictly chosen due to toolchain file." FORCE)
set(BIN_PACKAGE_SUBDIR "AppX" CACHE STRING "Folder below the binary output directory that contains the actual deployment package. (Needs to be AppX - not yet clear how to customize for real.)" FORCE)
