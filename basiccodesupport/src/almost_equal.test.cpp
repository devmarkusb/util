#include "ul/almost_equal.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(too_almost_equal_Test, test)
{
    EXPECT_TRUE(ul::almost_equal(1.0, 1.0));
    EXPECT_FALSE(ul::almost_equal(1.0, 1.0000001));
}

TEST(math_almost_equal_alltypes_Test, test)
{
    EXPECT_TRUE(ul::almost_equal_alltypes(1, 1));
    EXPECT_FALSE(ul::almost_equal_alltypes(1, 2));
}
