cmake_minimum_required(VERSION 3.20)

enable_testing()
# Show stdout/stderr when a test fails (e.g. on CI)
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")

find_package(GTest REQUIRED)
include(GoogleTest)
