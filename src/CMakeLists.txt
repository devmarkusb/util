find_package(GTest REQUIRED)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    foreach(_mb_util_gtest_target IN ITEMS gtest gtest_main gmock gmock_main)
        if(TARGET ${_mb_util_gtest_target})
            target_compile_options(
                ${_mb_util_gtest_target}
                PRIVATE -Wno-character-conversion
            )
        endif()
    endforeach()
endif()

add_executable(mb.util.compile-check)
target_sources(mb.util.compile-check PRIVATE compile-check.cpp)
target_link_libraries(mb.util.compile-check PRIVATE mb::util)
ul_set_target_defaults(mb.util.compile-check)

file(GLOB_RECURSE mb_util_test_sources CONFIGURE_DEPENDS "*.test.cpp")

add_executable(mb.util.test)
target_sources(mb.util.test PRIVATE ${mb_util_test_sources})
target_link_libraries(
    mb.util.test
    PRIVATE mb::util GTest::gtest GTest::gtest_main
)

if(MB_DEVENV_SANITIZER)
    target_compile_definitions(mb.util.test PRIVATE MB_DEVENV_SANITIZER=1)
endif()

ul_set_target_filesystem(mb.util.test)
ul_set_target_essentials(mb.util.test)

include(GoogleTest)
gtest_discover_tests(mb.util.test)
