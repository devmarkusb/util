#include "ul/round.h"
#include "gtest/gtest.h"
#include <cstdint>

TEST(MathRoundTest, Dbl) {
    EXPECT_DOUBLE_EQ(1.0, ul::math::round(0.5, 0));
    EXPECT_DOUBLE_EQ(0.0, ul::math::round(0.4, 0));
    EXPECT_DOUBLE_EQ(0.5, ul::math::round(0.54, 1));
    EXPECT_DOUBLE_EQ(0.6, ul::math::round(0.55, 1));
    EXPECT_DOUBLE_EQ(0.54, ul::math::round(0.54, 2));
    EXPECT_DOUBLE_EQ(0.54, ul::math::round(0.54, 4));
}

TEST(MathRoundTest, DblNeg) {
    EXPECT_DOUBLE_EQ(-1.0, ul::math::round(-0.5, 0));
    EXPECT_DOUBLE_EQ(0.0, ul::math::round(-0.4, 0));
    EXPECT_DOUBLE_EQ(-0.5, ul::math::round(-0.54, 1));
    EXPECT_DOUBLE_EQ(-0.6, ul::math::round(-0.55, 1));
}

TEST(MathRoundToTest, ToIntegralType) {
    EXPECT_DOUBLE_EQ(1, ul::math::round_to<short>(0.5));
    EXPECT_DOUBLE_EQ(0, ul::math::round_to<short>(0.4));
}

TEST(MathRoundToTest, ToIntegralTypeInf) {
    EXPECT_DOUBLE_EQ(127, ul::math::round_to<int8_t>(127.9));
    EXPECT_DOUBLE_EQ(-128, ul::math::round_to<int8_t>(-128.9));
}

TEST(MathRoundToTest, ToIntegralTypeIgnoreDecimalPlaces) {
    EXPECT_DOUBLE_EQ(3, ul::math::round_to<int8_t>(2.9, 1));
    EXPECT_DOUBLE_EQ(2, ul::math::round_to<int8_t>(2.1, 1));
}

TEST(MathRoundToTest, ToFloat) {
    EXPECT_DOUBLE_EQ(2.9f, ul::math::round_to<float>(2.9, 1));
    EXPECT_DOUBLE_EQ(2.0f, ul::math::round_to<float>(2.1, 0));
}
