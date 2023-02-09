set_target_properties(gmock PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(gmock_main PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(gtest PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(gtest_main PROPERTIES DEBUG_POSTFIX "d")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(UL_gtest_compile_options
            -Wno-zero-as-null-pointer-constant -Wno-sign-conversion -Wno-shift-sign-overflow
            -Wno-missing-variable-declarations -Wno-missing-field-initializers -Wno-used-but-marked-unused
            -Wno-disabled-macro-expansion -Wno-missing-prototypes
            -Wno-extra-semi-stmt -Wno-comma
            -Wno-reserved-identifier)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(UL_gtest_compile_options
            /wd4389)
else ()
    set(UL_gtest_compile_options)
endif ()
target_compile_options(gmock PUBLIC ${UL_gtest_compile_options})
target_compile_options(gtest PUBLIC ${UL_gtest_compile_options})
