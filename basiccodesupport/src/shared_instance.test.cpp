#include "ul/shared_instance.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(getSharedInstance, basics)
{
    auto i1 = ul::getSharedInstance<int>();
    // yes you can assume the implementation to call a value initializing make_shared (for POD types)
    EXPECT_EQ(*i1, 0);
    *i1 = 42;
    auto i2 = i1;
    i1 = {};
    EXPECT_EQ(*i2, 42);
    i2 = {};

    i1 = ul::getSharedInstance<int>();
    *i1 = 42;
    i2 = ul::getSharedInstance<int>();
    EXPECT_EQ(*i2, 42);
    i1 = {};
    i2 = ul::getSharedInstance<int>();
    EXPECT_EQ(*i2, 42);
    i2 = {};
    i2 = ul::getSharedInstance<int>();
    EXPECT_EQ(*i2, 0);
}
