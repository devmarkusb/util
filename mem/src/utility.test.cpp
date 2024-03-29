#include "ul/mem/utility.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(padUp, alignment4) {
    EXPECT_EQ(ul::mem::pad_up(1, 4), 4);
    EXPECT_EQ(ul::mem::pad_up(2, 4), 4);
    EXPECT_EQ(ul::mem::pad_up(3, 4), 4);
    EXPECT_EQ(ul::mem::pad_up(4, 4), 4);
    EXPECT_EQ(ul::mem::pad_up(5, 4), 8);
    EXPECT_EQ(ul::mem::pad_up(6, 4), 8);
    EXPECT_EQ(ul::mem::pad_up(7, 4), 8);
    EXPECT_EQ(ul::mem::pad_up(8, 4), 8);
    EXPECT_EQ(ul::mem::pad_up(9, 4), 12);
}

TEST(padUp, pathological) {
    EXPECT_EQ(ul::mem::pad_up(0, 0), 0);
    EXPECT_EQ(ul::mem::pad_up(0, 1), 0);
    EXPECT_EQ(ul::mem::pad_up(0, 4), 0);
    EXPECT_EQ(ul::mem::pad_up(0, 42), 0);

    EXPECT_EQ(ul::mem::pad_up(1, 0), 0);
    EXPECT_EQ(ul::mem::pad_up(42, 0), 0);
}

TEST(padUp, alignment1) {
    EXPECT_EQ(ul::mem::pad_up(1, 1), 1);
    EXPECT_EQ(ul::mem::pad_up(2, 1), 2);
    EXPECT_EQ(ul::mem::pad_up(3, 1), 3);
    EXPECT_EQ(ul::mem::pad_up(4, 1), 4);
    EXPECT_EQ(ul::mem::pad_up(5, 1), 5);
    EXPECT_EQ(ul::mem::pad_up(6, 1), 6);
    EXPECT_EQ(ul::mem::pad_up(7, 1), 7);
    EXPECT_EQ(ul::mem::pad_up(8, 1), 8);
    EXPECT_EQ(ul::mem::pad_up(9, 1), 9);
}
