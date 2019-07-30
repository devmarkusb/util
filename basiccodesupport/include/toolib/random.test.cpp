#include "random.h"
#include "gtest/gtest.h"


TEST(random_fracTest, basics)
{
    EXPECT_GE(too::math::random_frac(), 0.0);
    EXPECT_LT(too::math::random_frac(), 1.0);
}
