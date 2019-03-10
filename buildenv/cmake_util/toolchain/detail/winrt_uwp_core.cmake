# Provides mandatory configuration foundations you need to set up UWP projects properly (at least working for MSVC generator).

set(CMAKE_SYSTEM_NAME WindowsStore)

set(TOO_DEPLOY_TARGET "uwp" CACHE STRING "Don't change! UWP is already strictly chosen due to toolchain file." FORCE)
set(impl_BIN_PACKAGE_SUBDIR "AppX")
set(impl_BIN_PACKAGE_SUBDIR_BASE ${impl_BIN_PACKAGE_SUBDIR})
