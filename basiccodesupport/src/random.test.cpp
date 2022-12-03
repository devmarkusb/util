#include "ul/random.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(random_fracTest, basics)
{
    EXPECT_GE(ul::math::random_frac(), 0.0);
    EXPECT_LT(ul::math::random_frac(), 1.0);
}
