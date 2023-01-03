#include "ul/math.h"
#include "ul/macros.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(math_isPowerOfTwo, some)
{
    static_assert(!ul::math::isPowerOfTwo(0));
    static_assert(ul::math::isPowerOfTwo(1));
    static_assert(ul::math::isPowerOfTwo(2));
    static_assert(!ul::math::isPowerOfTwo(3));
    static_assert(ul::math::isPowerOfTwo(4));
    static_assert(ul::math::isPowerOfTwo(256));
    static_assert(!ul::math::isPowerOfTwo(257));

    static_assert(!ul::math::isPowerOfTwo(-1));
    static_assert(!ul::math::isPowerOfTwo(-2));
    static_assert(!ul::math::isPowerOfTwo(-256));
}

#if !UL_COMP_MS_VISUAL_STUDIO_CPP
TEST(math_NextGreaterOrEqPow2, int_zero)
{
    static_assert(ul::math::NextGreaterOrEqPow2<int, 0>::value() == 1);
    static_assert(ul::math::NextGreaterOrEqPow2<int, 0>::value() != 0);
}

TEST(math_NextGreaterOrEqPow2, ints)
{
    static_assert(ul::math::NextGreaterOrEqPow2<int, 1>::value() == 1);
    static_assert(ul::math::NextGreaterOrEqPow2<int, 2>::value() == 2);
    static_assert(ul::math::NextGreaterOrEqPow2<int, 3>::value() == 4);
    static_assert(ul::math::NextGreaterOrEqPow2<int, 4>::value() == 4);
    static_assert(ul::math::NextGreaterOrEqPow2<int, 5>::value() == 8);
    static_assert(ul::math::NextGreaterOrEqPow2<int, 6>::value() == 8);
    static_assert(ul::math::NextGreaterOrEqPow2<int, 7>::value() == 8);
    static_assert(ul::math::NextGreaterOrEqPow2<int, 8>::value() == 8);
    static_assert(ul::math::NextGreaterOrEqPow2<int, 9>::value() == 16);
    static_assert(ul::math::NextGreaterOrEqPow2<int, 10>::value() == 16);
}

TEST(math_NextGreaterOrEqPow2, int_neg)
{
    static_assert(ul::math::NextGreaterOrEqPow2<int, -1>::value() == 1);
    static_assert(ul::math::NextGreaterOrEqPow2<int, -2>::value() == 1);
    static_assert(ul::math::NextGreaterOrEqPow2<int, -1000>::value() == 1);
}

TEST(math_NextGreaterOrEqPow2, uint8_t_zero)
{
    static_assert(ul::math::NextGreaterOrEqPow2<uint8_t, 0>::value() == 1);
    static_assert(ul::math::NextGreaterOrEqPow2<uint8_t, 0>::value() != 0);
}

TEST(math_NextGreaterOrEqPow2, uint8_ts)
{
    static_assert(ul::math::NextGreaterOrEqPow2<uint8_t, 1>::value() == 1);
    static_assert(ul::math::NextGreaterOrEqPow2<uint8_t, 2>::value() == 2);
    static_assert(ul::math::NextGreaterOrEqPow2<uint8_t, 3>::value() == 4);
    static_assert(ul::math::NextGreaterOrEqPow2<uint8_t, 4>::value() == 4);
    static_assert(ul::math::NextGreaterOrEqPow2<uint8_t, 5>::value() == 8);
    static_assert(ul::math::NextGreaterOrEqPow2<uint8_t, 6>::value() == 8);
    static_assert(ul::math::NextGreaterOrEqPow2<uint8_t, 7>::value() == 8);
    static_assert(ul::math::NextGreaterOrEqPow2<uint8_t, 8>::value() == 8);
    static_assert(ul::math::NextGreaterOrEqPow2<uint8_t, 9>::value() == 16);
    static_assert(ul::math::NextGreaterOrEqPow2<uint8_t, 10>::value() == 16);
}

TEST(math_NextGreaterOrEqPow2, uint64_t_zero)
{
    static_assert(ul::math::NextGreaterOrEqPow2<int64_t, 0>::value() == 1);
    static_assert(ul::math::NextGreaterOrEqPow2<int64_t, 0>::value() != 0);
}

TEST(math_NextGreaterOrEqPow2, int64_ts)
{
    static_assert(ul::math::NextGreaterOrEqPow2<int64_t, 1>::value() == 1);
    static_assert(ul::math::NextGreaterOrEqPow2<int64_t, 2>::value() == 2);
    static_assert(ul::math::NextGreaterOrEqPow2<int64_t, 3>::value() == 4);
    static_assert(ul::math::NextGreaterOrEqPow2<int64_t, 4>::value() == 4);
    static_assert(ul::math::NextGreaterOrEqPow2<int64_t, 5>::value() == 8);
    static_assert(ul::math::NextGreaterOrEqPow2<int64_t, 6>::value() == 8);
    static_assert(ul::math::NextGreaterOrEqPow2<int64_t, 7>::value() == 8);
    static_assert(ul::math::NextGreaterOrEqPow2<int64_t, 8>::value() == 8);
    static_assert(ul::math::NextGreaterOrEqPow2<int64_t, 9>::value() == 16);
    static_assert(ul::math::NextGreaterOrEqPow2<int64_t, 10>::value() == 16);
}

TEST(math_NextGreaterPow2, int_zero)
{
    static_assert(ul::math::NextGreaterPow2<int, 0>::value() == 1);
    static_assert(ul::math::NextGreaterPow2<int, 0>::value() != 0);
}

TEST(math_NextGreaterPow2, ints)
{
    static_assert(ul::math::NextGreaterPow2<int, 1>::value() == 2);
    static_assert(ul::math::NextGreaterPow2<int, 2>::value() == 4);
    static_assert(ul::math::NextGreaterPow2<int, 3>::value() == 4);
    static_assert(ul::math::NextGreaterPow2<int, 4>::value() == 8);
    static_assert(ul::math::NextGreaterPow2<int, 5>::value() == 8);
    static_assert(ul::math::NextGreaterPow2<int, 6>::value() == 8);
    static_assert(ul::math::NextGreaterPow2<int, 7>::value() == 8);
    static_assert(ul::math::NextGreaterPow2<int, 8>::value() == 16);
    static_assert(ul::math::NextGreaterPow2<int, 9>::value() == 16);
}

TEST(math_NextGreaterPow2, int_neg)
{
    static_assert(ul::math::NextGreaterPow2<int, -1>::value() == 1);
    static_assert(ul::math::NextGreaterPow2<int, -2>::value() == 1);
    static_assert(ul::math::NextGreaterPow2<int, -1000>::value() == 1);
}
#endif

TEST(math_nextGreaterOrEqPow2, some)
{
    static_assert(ul::math::nextGreaterOrEqPow2(1) == 1);
    static_assert(ul::math::nextGreaterOrEqPow2(2) == 2);
    static_assert(ul::math::nextGreaterOrEqPow2(3) == 4);
    static_assert(ul::math::nextGreaterOrEqPow2(4) == 4);
    static_assert(ul::math::nextGreaterOrEqPow2(5) == 8);
    static_assert(ul::math::nextGreaterOrEqPow2(6) == 8);
    static_assert(ul::math::nextGreaterOrEqPow2(7) == 8);
    static_assert(ul::math::nextGreaterOrEqPow2(8) == 8);
    static_assert(ul::math::nextGreaterOrEqPow2(9) == 16);
    static_assert(ul::math::nextGreaterOrEqPow2(10) == 16);

    EXPECT_TRUE(ul::math::nextGreaterOrEqPow2(1) == 1);
    EXPECT_TRUE(ul::math::nextGreaterOrEqPow2(2) == 2);
    EXPECT_TRUE(ul::math::nextGreaterOrEqPow2(3) == 4);
    EXPECT_TRUE(ul::math::nextGreaterOrEqPow2(4) == 4);
    EXPECT_TRUE(ul::math::nextGreaterOrEqPow2(5) == 8);
    EXPECT_TRUE(ul::math::nextGreaterOrEqPow2(6) == 8);
    EXPECT_TRUE(ul::math::nextGreaterOrEqPow2(7) == 8);
    EXPECT_TRUE(ul::math::nextGreaterOrEqPow2(8) == 8);
    EXPECT_TRUE(ul::math::nextGreaterOrEqPow2(9) == 16);
    EXPECT_TRUE(ul::math::nextGreaterOrEqPow2(10) == 16);
}
