# Note, if you want to use gmock, you should link to gmock_main or gmock,
# and gtest. This also seems to enable access to the include path.
# If you don't need gmock, linking to gtest or gtest_main is sufficient.

if(NOT DEFINED CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
    set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")
endif()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK ON CACHE BOOL "" FORCE)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)
add_subdirectory(googletest)
set_target_properties(gmock PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(gmock_main PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(gtest PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(gtest_main PROPERTIES DEBUG_POSTFIX "d")
