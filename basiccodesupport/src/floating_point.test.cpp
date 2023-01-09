#include "ul/floating_point.h"
#include "gtest/gtest.h"

TEST(math_approx_equal_Test, test)
{
    EXPECT_TRUE(ul::math::approx_equal(1.0, 1.001, 0.001));
    EXPECT_FALSE(ul::math::approx_equal(1.0, 1.00011, 0.0001));
}

TEST(math_to_string_Test, test)
{
    EXPECT_STREQ("4.556000", ul::math::to_string(4.556).c_str());
    EXPECT_STREQ("4.6", ul::math::to_string(4.556, 2).c_str());

    EXPECT_STREQ("4.556000", ul::math::to_string<ul::math::FloatFormat::default_>(4.556).c_str());
    EXPECT_STREQ("4.6", ul::math::to_string<ul::math::FloatFormat::default_>(4.556, 2).c_str());

    EXPECT_STREQ("4.556000", ul::math::to_string<ul::math::FloatFormat::fixed>(4.556).c_str());
    EXPECT_STREQ("4.56", ul::math::to_string<ul::math::FloatFormat::fixed>(4.556, 2).c_str());

    const auto s1 = ul::math::to_string<ul::math::FloatFormat::scientific>(4.556);
    EXPECT_EQ(0u, s1.find("4.556000e+00"));
    const auto s2 = ul::math::to_string<ul::math::FloatFormat::scientific>(4.556, 2);
    EXPECT_EQ(0u, s2.find("4.56e+00"));
}
