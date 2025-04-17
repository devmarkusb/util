#include "ul/floating_point.h"
#include "gtest/gtest.h"
#include <cmath>
#include <limits>

TEST(FloatingPointTest, approx_equal_misc) {
    EXPECT_TRUE(ul::math::approx_equal(1.0, 1.001, 0.001));
    EXPECT_FALSE(ul::math::approx_equal(1.0, 1.00011, 0.0001));
}

TEST(FloatingPointTest, to_string_misc) {
    EXPECT_STREQ("4.556000", ul::math::to_string(4.556).c_str());
    EXPECT_STREQ("4.6", ul::math::to_string(4.556, 2).c_str());

    EXPECT_STREQ("4.556000", ul::math::to_string<ul::math::FloatFormat::default_choice>(4.556).c_str());
    EXPECT_STREQ("4.6", ul::math::to_string<ul::math::FloatFormat::default_choice>(4.556, 2).c_str());

    EXPECT_STREQ("4.556000", ul::math::to_string<ul::math::FloatFormat::fixed>(4.556).c_str());
    EXPECT_STREQ("4.56", ul::math::to_string<ul::math::FloatFormat::fixed>(4.556, 2).c_str());

    const auto s1 = ul::math::to_string<ul::math::FloatFormat::scientific>(4.556);
    EXPECT_EQ(0u, s1.find("4.556000e+00"));
    const auto s2 = ul::math::to_string<ul::math::FloatFormat::scientific>(4.556, 2);
    EXPECT_EQ(0u, s2.find("4.56e+00"));
}

TEST(FloatingPointTest, ApproxEqual) {
    // Test with double
    EXPECT_TRUE(mb::ul::math::approx_equal(1.0, 1.0, 0.0001));
    EXPECT_TRUE(mb::ul::math::approx_equal(1.0, 1.00001, 0.0001));
    EXPECT_FALSE(mb::ul::math::approx_equal(1.0, 1.1, 0.0001));
    
    // Test with float
    EXPECT_TRUE(mb::ul::math::approx_equal(1.0f, 1.0f, 0.0001f));
    EXPECT_TRUE(mb::ul::math::approx_equal(1.0f, 1.00001f, 0.0001f));
    EXPECT_FALSE(mb::ul::math::approx_equal(1.0f, 1.1f, 0.0001f));
    
    // Test with long double
    EXPECT_TRUE(mb::ul::math::approx_equal(1.0L, 1.0L, 0.0001L));
    EXPECT_TRUE(mb::ul::math::approx_equal(1.0L, 1.00001L, 0.0001L));
    EXPECT_FALSE(mb::ul::math::approx_equal(1.0L, 1.1L, 0.0001L));
    
    // Test with zero
    EXPECT_TRUE(mb::ul::math::approx_equal(0.0, 0.0, 0.0001));
    EXPECT_TRUE(mb::ul::math::approx_equal(0.0, 0.00001, 0.0001));
    
    // Test with negative numbers
    EXPECT_TRUE(mb::ul::math::approx_equal(-1.0, -1.0, 0.0001));
    EXPECT_TRUE(mb::ul::math::approx_equal(-1.0, -1.00001, 0.0001));
    EXPECT_FALSE(mb::ul::math::approx_equal(-1.0, -1.1, 0.0001));
    
    // Test with different signs
    EXPECT_FALSE(mb::ul::math::approx_equal(1.0, -1.0, 0.0001));
    
    // Test with very small epsilon
    EXPECT_TRUE(mb::ul::math::approx_equal(1.0, 1.0, std::numeric_limits<double>::epsilon()));
    EXPECT_FALSE(mb::ul::math::approx_equal(1.0, 1.0 + std::numeric_limits<double>::epsilon(), 
                                           std::numeric_limits<double>::epsilon() / 2));
    
    // Test with NaN
    EXPECT_FALSE(mb::ul::math::approx_equal(std::numeric_limits<double>::quiet_NaN(), 1.0, 0.0001));
    EXPECT_FALSE(mb::ul::math::approx_equal(1.0, std::numeric_limits<double>::quiet_NaN(), 0.0001));
    EXPECT_FALSE(mb::ul::math::approx_equal(std::numeric_limits<double>::quiet_NaN(), 
                                           std::numeric_limits<double>::quiet_NaN(), 0.0001));
    
    // Test with infinity
    EXPECT_TRUE(mb::ul::math::approx_equal(std::numeric_limits<double>::infinity(), 
                                          std::numeric_limits<double>::infinity(), 0.0001));
    EXPECT_TRUE(mb::ul::math::approx_equal(-std::numeric_limits<double>::infinity(), 
                                          -std::numeric_limits<double>::infinity(), 0.0001));
    EXPECT_FALSE(mb::ul::math::approx_equal(std::numeric_limits<double>::infinity(), 
                                           -std::numeric_limits<double>::infinity(), 0.0001));
    EXPECT_FALSE(mb::ul::math::approx_equal(std::numeric_limits<double>::infinity(), 1.0, 0.0001));
    
    // Test with very small numbers
    const double tiny = 1e-20;
    EXPECT_TRUE(mb::ul::math::approx_equal(tiny, tiny, 1e-20));
    EXPECT_FALSE(mb::ul::math::approx_equal(tiny, tiny * 2, 1e-20));
}

TEST(FloatingPointTest, ToStringDefault) {
    // Test with double
    EXPECT_EQ(mb::ul::math::to_string(3.14159), "3.141590");
    EXPECT_EQ(mb::ul::math::to_string(3.14159, 3), "3.14");
    EXPECT_EQ(mb::ul::math::to_string(3.14159, 5), "3.1416");
    
    // Test with float
    EXPECT_EQ(mb::ul::math::to_string(3.14159f), "3.141590");
    EXPECT_EQ(mb::ul::math::to_string(3.14159f, 3), "3.14");
    EXPECT_EQ(mb::ul::math::to_string(3.14159f, 5), "3.1416");
    
    // Test with long double
    EXPECT_EQ(mb::ul::math::to_string(3.14159L), "3.141590");
    EXPECT_EQ(mb::ul::math::to_string(3.14159L, 3), "3.14");
    EXPECT_EQ(mb::ul::math::to_string(3.14159L, 5), "3.1416");
    
    // Test with zero
    EXPECT_EQ(mb::ul::math::to_string(0.0), "0.000000");
    EXPECT_EQ(mb::ul::math::to_string(0.0, 3), "0");
    
    // Test with negative numbers
    EXPECT_EQ(mb::ul::math::to_string(-3.14159), "-3.141590");
    EXPECT_EQ(mb::ul::math::to_string(-3.14159, 3), "-3.14");
    
    // Test with very large numbers
    EXPECT_EQ(mb::ul::math::to_string(1e10), "10000000000.000000");
    EXPECT_EQ(mb::ul::math::to_string(1e10, 3), "1e+10");
    
    // Test with very small numbers
    EXPECT_EQ(mb::ul::math::to_string(1e-10), "0.000000");
    EXPECT_EQ(mb::ul::math::to_string(1e-10, 15), "1e-10");
    
    // Test with NaN
    EXPECT_EQ(mb::ul::math::to_string(std::numeric_limits<double>::quiet_NaN()), "nan");
    EXPECT_EQ(mb::ul::math::to_string(std::numeric_limits<double>::quiet_NaN(), 3), "nan");
    
    // Test with infinity
    EXPECT_EQ(mb::ul::math::to_string(std::numeric_limits<double>::infinity()), "inf");
    EXPECT_EQ(mb::ul::math::to_string(-std::numeric_limits<double>::infinity()), "-inf");
    EXPECT_EQ(mb::ul::math::to_string(std::numeric_limits<double>::infinity(), 3), "inf");
    
    // Test with numbers that should use scientific notation
    EXPECT_EQ(mb::ul::math::to_string(1e-5), "0.000010");
    EXPECT_EQ(mb::ul::math::to_string(1e-5, 3), "1e-05");
    EXPECT_EQ(mb::ul::math::to_string(1e-5, 15), "1e-05");
    
    // Test with numbers that should use fixed notation
    EXPECT_EQ(mb::ul::math::to_string(1e5), "100000.000000");
    EXPECT_EQ(mb::ul::math::to_string(1e5, 3), "1e+05");
    
    // Test with numbers that should use fixed notation with precision
    EXPECT_EQ(mb::ul::math::to_string(1.23456, 3), "1.23");
    EXPECT_EQ(mb::ul::math::to_string(1.23456, 5), "1.2346");
}

TEST(FloatingPointTest, ToStringFixed) {
    // Test with double
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::fixed>(3.14159), "3.141590");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::fixed>(3.14159, 3), "3.142");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::fixed>(3.14159, 5), "3.14159");
    
    // Test with float
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::fixed>(3.14159f), "3.141590");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::fixed>(3.14159f, 3), "3.142");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::fixed>(3.14159f, 5), "3.14159");
    
    // Test with long double
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::fixed>(3.14159L), "3.141590");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::fixed>(3.14159L, 3), "3.142");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::fixed>(3.14159L, 5), "3.14159");
    
    // Test with zero
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::fixed>(0.0), "0.000000");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::fixed>(0.0, 3), "0.000");
    
    // Test with negative numbers
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::fixed>(-3.14159), "-3.141590");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::fixed>(-3.14159, 3), "-3.142");
    
    // Test with very large numbers
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::fixed>(1e10), "10000000000.000000");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::fixed>(1e10, 3), "10000000000.000");
    
    // Test with very small numbers
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::fixed>(1e-10), "0.000000");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::fixed>(1e-10, 15), "0.000000000100000");
    
    // Test with NaN
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::fixed>(
        std::numeric_limits<double>::quiet_NaN()), "nan");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::fixed>(
        std::numeric_limits<double>::quiet_NaN(), 3), "nan");
    
    // Test with infinity
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::fixed>(
        std::numeric_limits<double>::infinity()), "inf");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::fixed>(
        -std::numeric_limits<double>::infinity()), "-inf");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::fixed>(
        std::numeric_limits<double>::infinity(), 3), "inf");
}

TEST(FloatingPointTest, ToStringScientific) {
    // Test with double
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::scientific>(3.14159), "3.141590e+00");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::scientific>(3.14159, 3), "3.142e+00");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::scientific>(3.14159, 5), "3.14159e+00");
    
    // Test with float
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::scientific>(3.14159f), "3.141590e+00");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::scientific>(3.14159f, 3), "3.142e+00");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::scientific>(3.14159f, 5), "3.14159e+00");
    
    // Test with long double
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::scientific>(3.14159L), "3.141590e+00");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::scientific>(3.14159L, 3), "3.142e+00");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::scientific>(3.14159L, 5), "3.14159e+00");
    
    // Test with zero
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::scientific>(0.0), "0.000000e+00");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::scientific>(0.0, 3), "0.000e+00");
    
    // Test with negative numbers
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::scientific>(-3.14159), "-3.141590e+00");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::scientific>(-3.14159, 3), "-3.142e+00");
    
    // Test with very large numbers
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::scientific>(1e10), "1.000000e+10");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::scientific>(1e10, 3), "1.000e+10");
    
    // Test with very small numbers
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::scientific>(1e-10), "1.000000e-10");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::scientific>(1e-10, 15), "1.000000000000000e-10");
    
    // Test with NaN
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::scientific>(
        std::numeric_limits<double>::quiet_NaN()), "nan");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::scientific>(
        std::numeric_limits<double>::quiet_NaN(), 3), "nan");
    
    // Test with infinity
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::scientific>(
        std::numeric_limits<double>::infinity()), "inf");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::scientific>(
        -std::numeric_limits<double>::infinity()), "-inf");
    EXPECT_EQ(mb::ul::math::to_string<mb::ul::math::FloatFormat::scientific>(
        std::numeric_limits<double>::infinity(), 3), "inf");
}
