add_executable(mb.util.compile-check)
target_sources(mb.util.compile-check PRIVATE compile_check.cpp)
target_link_libraries(mb.util.compile-check PRIVATE mb::util)
mb_devenv_set_target_defaults(mb.util.compile-check)

# cppcheck currently crashes on this translation unit when invoked through
# CMake's per-source lint integration. Keep linting enabled for the rest of
# the test suite.
set_source_files_properties(
    basiccodesupport/bits.test.cpp
    PROPERTIES SKIP_LINTING ON
)

include(mb-devenv-googletest)
mb_devenv_add_test(mb.util.test PRIVATE mb::util)
mb_devenv_set_target_filesystem(mb.util.test)
mb_devenv_set_target_essentials(mb.util.test)
if(MB_DEVENV_SANITIZER)
    target_compile_definitions(mb.util.test PRIVATE MB_DEVENV_SANITIZER=1)
endif()
