#include "bits.h"
#include "gtest/gtest.h"


TEST(bits_num, various)
{
    static_assert(too::bits::num<uint8_t>() == 8);
    static_assert(too::bits::num<uint32_t>() == 32);
    static_assert(too::bits::num<uint64_t>() == 64);

    static_assert(too::bits::num<int8_t>() == 8);
    static_assert(too::bits::num<int32_t>() == 32);
    static_assert(too::bits::num<int64_t>() == 64);
}

TEST(bits_makeRangeOfOnes, empty)
{
    EXPECT_EQ(too::bits::makeRangeOfOnes(0, 0), 0u);
    EXPECT_EQ(too::bits::makeRangeOfOnes(42, 42), 0u);
}

TEST(bits_makeRangeOfOnes, bitNr0)
{
    EXPECT_EQ(too::bits::makeRangeOfOnes(0, 1), 0b1u);
}

TEST(bits_makeRangeOfOnes, bitNr1)
{
    EXPECT_EQ(too::bits::makeRangeOfOnes(1, 2), 0b10u);
}

TEST(bits_makeRangeOfOnes, bitNr63)
{
    EXPECT_EQ(too::bits::makeRangeOfOnes(63, 64), uint64_t(1) << uint64_t(63));
}

TEST(bits_makeRangeOfOnes, highestTwoBits)
{
    EXPECT_EQ(too::bits::makeRangeOfOnes(62, 64), (uint64_t(1) << uint64_t(63)) | (uint64_t(1) << uint64_t(62)));
}

TEST(bits_makeRangeOfOnes, lowestTwoBits)
{
    EXPECT_EQ(too::bits::makeRangeOfOnes(0, 2), 0b11u);
}

TEST(bits_makeRangeOfOnes, someMiddleRangeOfBits)
{
    EXPECT_EQ(too::bits::makeRangeOfOnes(3, 9), 0b111111000u);
}

TEST(bits_makeRangeOfOnes, someMiddleRangeOfBits_castedToSufficientSmallerType)
{
    EXPECT_EQ(static_cast<uint32_t>(too::bits::makeRangeOfOnes(3, 9)), 0b111111000u);
}
