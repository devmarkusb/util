# Provides switches and tools to run diagnostics on your code.
# *Warning* Please note that this is work in progress. It doesn't contain anything really usable at the moment.
#

include_guard(DIRECTORY)

if(MB_DEVENV_USING_LIBCPP)
    option(
        MB_UL_LIBCPP_DEBUG
        "enables the _LIBCPP_DEBUG mode, prerequisite: compiler being built with LIBCXX_ENABLE_DEBUG_MODE_SUPPORT"
        OFF
    )
endif()

if(MB_UL_LIBCPP_DEBUG)
    add_definitions(-D_LIBCPP_DEBUG=1)
endif()
