# Provides mandatory configuration foundations you need to set up UWP projects properly (at least working for MSVC generator).

set(CMAKE_SYSTEM_VERSION 10.0.15063.0)
include(${CMAKE_CURRENT_LIST_DIR}/detail/winrt_uwp_core.cmake)
