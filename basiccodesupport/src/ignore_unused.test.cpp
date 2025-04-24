#include "ul/ignore_unused.h" // NOLINT
#include "gtest/gtest.h"

namespace {
// Test function with unused parameter
void test_function_with_unused_param(int unused_param) {
    mb::ul::ignore_unused(unused_param);
    // This should compile without warnings
}

// Test with different types
template <typename T>
void test_with_type(T value) {
    mb::ul::ignore_unused(value);
    // This should compile without warnings
}

TEST(IgnoreUnusedTest, BasicFunctionality) {
    // Test with integer
    test_function_with_unused_param(42);

    // Test with different types
    test_with_type(42); // int
    test_with_type(3.14); // double
    test_with_type("test"); // const char*
    test_with_type(std::string("test")); // std::string

    // Test with rvalue
    mb::ul::ignore_unused(42);

    // Test with multiple parameters
    int a = 1, b = 2, c = 3;
    mb::ul::ignore_unused(a);
    mb::ul::ignore_unused(b);
    mb::ul::ignore_unused(c);

    // Test with class instance
    struct TestClass {};

    TestClass test_obj;
    mb::ul::ignore_unused(test_obj);

    // Test with enum
    enum TestEnum {
        A,
        B,
        C
    };

    TestEnum test_enum = A;
    mb::ul::ignore_unused(test_enum);

    // All of these should compile without warnings
    EXPECT_TRUE(true); // Just to have an assertion
}
}
