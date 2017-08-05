# Needed e.g. if you want to build with msvc2015 a newer SDK installed and also runs
# at the according Windows version. Then CMake would choose the most modern possible SDK.
# Note that the below chosen 8.1 SDK is also compatible to build for Windows 7 or Vista
# according to MSDN.

set(CMAKE_SYSTEM_VERSION 8.1)
