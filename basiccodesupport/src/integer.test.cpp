#include "ul/integer.h" // NOLINT
#include "gtest/gtest.h"
#include <vector>
#include <cstdint>

#if __cpp_concepts && __cpp_lib_concepts

TEST(IntegerTest, BasicOperations) {
    // Test successor
    EXPECT_EQ(ul::successor(5), 6);
    EXPECT_EQ(ul::successor(0), 1);
    EXPECT_EQ(ul::successor(-1), 0);
    
    // Test predecessor
    EXPECT_EQ(ul::predecessor(5), 4);
    EXPECT_EQ(ul::predecessor(0), -1);
    EXPECT_EQ(ul::predecessor(1), 0);
    
    // Test twice
    EXPECT_EQ(ul::twice(5), 10);
    EXPECT_EQ(ul::twice(0), 0);
    EXPECT_EQ(ul::twice(-3), -6);
    
    // Test half_nonnegative
    EXPECT_EQ(ul::half_nonnegative(10), 5);
    EXPECT_EQ(ul::half_nonnegative(0), 0);
    EXPECT_EQ(ul::half_nonnegative(1), 0); // Integer division rounds down
    
    // Test positive/negative/zero/one
    EXPECT_TRUE(ul::positive(5));
    EXPECT_FALSE(ul::positive(0));
    EXPECT_FALSE(ul::positive(-3));
    
    EXPECT_TRUE(ul::negative(-3));
    EXPECT_FALSE(ul::negative(0));
    EXPECT_FALSE(ul::negative(5));
    
    EXPECT_TRUE(ul::zero(0));
    EXPECT_FALSE(ul::zero(1));
    EXPECT_FALSE(ul::zero(-1));
    
    EXPECT_TRUE(ul::one(1));
    EXPECT_FALSE(ul::one(0));
    EXPECT_FALSE(ul::one(2));
}

TEST(IntegerTest, BinaryOperations) {
    // Test binary_scale_down_nonnegative
    EXPECT_EQ(ul::binary_scale_down_nonnegative(10, 1U), 5);
    EXPECT_EQ(ul::binary_scale_down_nonnegative(10, 2U), 2);
    EXPECT_EQ(ul::binary_scale_down_nonnegative(0, 1U), 0);
    
    // Test binary_scale_up_nonnegative
    EXPECT_EQ(ul::binary_scale_up_nonnegative(5, 1U), 10);
    EXPECT_EQ(ul::binary_scale_up_nonnegative(5, 2U), 20);
    EXPECT_EQ(ul::binary_scale_up_nonnegative(0, 1U), 0);
    
    // Test odd/even
    EXPECT_TRUE(ul::odd(5));
    EXPECT_FALSE(ul::odd(4));
    EXPECT_FALSE(ul::odd(0));
    
    EXPECT_TRUE(ul::even(4));
    EXPECT_FALSE(ul::even(5));
    EXPECT_TRUE(ul::even(0));
}

TEST(IntegerTest, DivisibilityAndPrimality) {
    // Test divides
    EXPECT_TRUE(ul::divides(2, 10));
    EXPECT_FALSE(ul::divides(3, 10));
    EXPECT_TRUE(ul::divides(1, 10));
    EXPECT_TRUE(ul::divides(10, 10));
    
    // Test smallest_divisor
    EXPECT_EQ(ul::smallest_divisor(10), 2);
    EXPECT_EQ(ul::smallest_divisor(15), 3);
    EXPECT_EQ(ul::smallest_divisor(17), 17); // Prime number
    
    // Test is_prime
    EXPECT_TRUE(ul::is_prime(2));
    EXPECT_TRUE(ul::is_prime(3));
    EXPECT_TRUE(ul::is_prime(17));
    EXPECT_FALSE(ul::is_prime(4));
    EXPECT_FALSE(ul::is_prime(10));
    EXPECT_FALSE(ul::is_prime(1));
    EXPECT_FALSE(ul::is_prime(0));
}
#else
TEST(IntegerTest, Skipped) {
    GTEST_SKIP() << "C++20 concepts not supported";
}
#endif
