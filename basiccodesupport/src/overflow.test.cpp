#include "ul/overflow.h"
#include "gtest/gtest.h"

namespace {
const auto max{std::numeric_limits<int>::max()};
const auto min{std::numeric_limits<int>::min()};

TEST(mul_overflow_test, misc) {
    EXPECT_FALSE(ul::math::mul_overflow(max, 1));
    EXPECT_TRUE(ul::math::mul_overflow(max, 2));
    EXPECT_FALSE(ul::math::mul_overflow(max / 2, 2));
    EXPECT_TRUE(ul::math::mul_overflow(max / 2 + 1, 2));
    EXPECT_TRUE(ul::math::mul_overflow(min, -1)); // also min / -1 would overflow
    EXPECT_FALSE(ul::math::mul_overflow(min / 2, 2));
    EXPECT_TRUE(ul::math::mul_overflow(min / 2 - 1, 2));
}
} // namespace
