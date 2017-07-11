# Needed e.g. if you want to build with msvc2015 (supports no newer SDK than this) on a host that has
# a newer SDK installed and also runs at the according Windows version. Then CMake would choose the most
# modern possible SDK.

set(CMAKE_SYSTEM_VERSION 10.0.14393.0)
include(${CMAKE_CURRENT_LIST_DIR}/detail/winrt_uwp_core.cmake)
