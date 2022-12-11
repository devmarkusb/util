#include "ul/mem/utility.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(pad_up, alignment4)
{
    EXPECT_EQ(ul::mem::padUp(1, 4), 4);
    EXPECT_EQ(ul::mem::padUp(2, 4), 4);
    EXPECT_EQ(ul::mem::padUp(3, 4), 4);
    EXPECT_EQ(ul::mem::padUp(4, 4), 4);
    EXPECT_EQ(ul::mem::padUp(5, 4), 8);
    EXPECT_EQ(ul::mem::padUp(6, 4), 8);
    EXPECT_EQ(ul::mem::padUp(7, 4), 8);
    EXPECT_EQ(ul::mem::padUp(8, 4), 8);
    EXPECT_EQ(ul::mem::padUp(9, 4), 12);
}

TEST(pad_up, pathological)
{
    EXPECT_EQ(ul::mem::padUp(0, 0), 0);
    EXPECT_EQ(ul::mem::padUp(0, 1), 0);
    EXPECT_EQ(ul::mem::padUp(0, 4), 0);
    EXPECT_EQ(ul::mem::padUp(0, 42), 0);

    EXPECT_EQ(ul::mem::padUp(1, 0), 0);
    EXPECT_EQ(ul::mem::padUp(42, 0), 0);
}

TEST(pad_up, alignment1)
{
    EXPECT_EQ(ul::mem::padUp(1, 1), 1);
    EXPECT_EQ(ul::mem::padUp(2, 1), 2);
    EXPECT_EQ(ul::mem::padUp(3, 1), 3);
    EXPECT_EQ(ul::mem::padUp(4, 1), 4);
    EXPECT_EQ(ul::mem::padUp(5, 1), 5);
    EXPECT_EQ(ul::mem::padUp(6, 1), 6);
    EXPECT_EQ(ul::mem::padUp(7, 1), 7);
    EXPECT_EQ(ul::mem::padUp(8, 1), 8);
    EXPECT_EQ(ul::mem::padUp(9, 1), 9);
}
