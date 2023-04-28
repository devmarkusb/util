#include "ul/number.h"
#include "gtest/gtest.h"

TEST(MathNumberGetDecDigitCountTest, Dec) {
    EXPECT_EQ(1, ul::math::getDigitCount(0));
    EXPECT_EQ(1, ul::math::getDigitCount(1));
    EXPECT_EQ(2, ul::math::getDigitCount(12));
    EXPECT_EQ(3, ul::math::getDigitCount(123));
}

TEST(MathNumberGetDecDigitCountTest, DecNeg) {
    EXPECT_EQ(1, ul::math::getDigitCount(-1));
    EXPECT_EQ(2, ul::math::getDigitCount(-12));
}

TEST(MathNumberGetDecDigitCountTest, Bin) {
    EXPECT_EQ(1, ul::math::getDigitCount(0, ul::math::NumBase::BIN));
    EXPECT_EQ(2, ul::math::getDigitCount(2, ul::math::NumBase::BIN));
    EXPECT_EQ(3, ul::math::getDigitCount(4, ul::math::NumBase::BIN));
}

TEST(MathNumberGetDecDigitCountTest, Oct) {
    EXPECT_EQ(1, ul::math::getDigitCount(0, ul::math::NumBase::OCT));
    EXPECT_EQ(1, ul::math::getDigitCount(7, ul::math::NumBase::OCT));
    EXPECT_EQ(2, ul::math::getDigitCount(8, ul::math::NumBase::OCT));
}

TEST(MathNumberGetDecDigitCountTest, Hex) {
    EXPECT_EQ(1, ul::math::getDigitCount(0, ul::math::NumBase::HEX));
    EXPECT_EQ(1, ul::math::getDigitCount(15, ul::math::NumBase::HEX));
    EXPECT_EQ(2, ul::math::getDigitCount(16, ul::math::NumBase::HEX));
}

TEST(MathNumberGetDigit, Dec) {
    EXPECT_EQ(5, ul::math::getDigit(12345, 0));
    EXPECT_EQ(4, ul::math::getDigit(12345, 1));
    EXPECT_EQ(3, ul::math::getDigit(12345, 2));
    EXPECT_EQ(2, ul::math::getDigit(12345, 3));
    EXPECT_EQ(1, ul::math::getDigit(12345, 4));
    EXPECT_EQ(0, ul::math::getDigit(12345, 5));
}

TEST(MathNumberGetDigit, Hex) {
    EXPECT_EQ(0xf, ul::math::getDigit(0x9abcdef, 0, ul::math::NumBase::HEX));
    EXPECT_EQ(0xe, ul::math::getDigit(0x9abcdef, 1, ul::math::NumBase::HEX));
    EXPECT_EQ(0xd, ul::math::getDigit(0x9abcdef, 2, ul::math::NumBase::HEX));
    EXPECT_EQ(0xc, ul::math::getDigit(0x9abcdef, 3, ul::math::NumBase::HEX));
    EXPECT_EQ(0xb, ul::math::getDigit(0x9abcdef, 4, ul::math::NumBase::HEX));
    EXPECT_EQ(0xa, ul::math::getDigit(0x9abcdef, 5, ul::math::NumBase::HEX));
    EXPECT_EQ(0x9, ul::math::getDigit(0x9abcdef, 6, ul::math::NumBase::HEX));
    EXPECT_EQ(0x0, ul::math::getDigit(0x9abcdef, 7, ul::math::NumBase::HEX));
}

TEST(MathNumberGetDigit, Bin) {
    EXPECT_EQ(0b0, ul::math::getDigit(0b1010, 0, ul::math::NumBase::BIN));
    EXPECT_EQ(0b1, ul::math::getDigit(0b1010, 1, ul::math::NumBase::BIN));
    EXPECT_EQ(0b0, ul::math::getDigit(0b1010, 2, ul::math::NumBase::BIN));
    EXPECT_EQ(0b1, ul::math::getDigit(0b1010, 3, ul::math::NumBase::BIN));
    EXPECT_EQ(0b0, ul::math::getDigit(0b1010, 4, ul::math::NumBase::BIN));
}

TEST(math_sgnTest, Hex) {
    EXPECT_EQ(0, ul::math::sgn(0));
    EXPECT_EQ(+1, ul::math::sgn(1));
    EXPECT_EQ(-1, ul::math::sgn(-1));
    EXPECT_EQ(+1, ul::math::sgn(5));
    EXPECT_EQ(-1, ul::math::sgn(-5));

    EXPECT_EQ(0, ul::math::sgn(0.0));
    EXPECT_EQ(+1, ul::math::sgn(1.0));
    EXPECT_EQ(-1, ul::math::sgn(-1.0));

    EXPECT_EQ(0, ul::math::sgn(0u));
    EXPECT_EQ(+1, ul::math::sgn(1u));

    EXPECT_EQ(-1, ul::math::sgn(-1L));
}

// NOLINTBEGIN

TEST(math_is_power_ofTest, simple) {
    EXPECT_EQ(2, *ul::math::isPowerOf(100, 10));
    if (ul::math::isPowerOf(10, 10))
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
    EXPECT_FALSE(ul::math::isPowerOf(101, 10));
}

TEST(math_is_power_ofTest, floatingpoint) {
    EXPECT_DOUBLE_EQ(2.0, *ul::math::isPowerOf(100.0, 10.0));
    EXPECT_DOUBLE_EQ(-5.0, *ul::math::isPowerOf(0.00001, 10.0));
    EXPECT_FALSE(ul::math::isPowerOf(100.1, 10.0));
}

TEST(math_is_power_ofTest, corners) {
    EXPECT_DOUBLE_EQ(0, *ul::math::isPowerOf(1, 2));

    EXPECT_DOUBLE_EQ(0.0, *ul::math::isPowerOf(1.0, 0.5));
    EXPECT_DOUBLE_EQ(0.0, *ul::math::isPowerOf(1.0, 2.0));

    EXPECT_DOUBLE_EQ(-1.0, *ul::math::isPowerOf(2.0, 0.5));
    EXPECT_DOUBLE_EQ(1.0, *ul::math::isPowerOf(2.0, 2.0));
    EXPECT_FALSE(ul::math::isPowerOf(2.0, 4.0));

    EXPECT_DOUBLE_EQ(-1.0, *ul::math::isPowerOf(0.25, 4.0));
}

// NOLINTEND
