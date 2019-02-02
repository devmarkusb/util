#include "shared_instance.h"
#include "gtest/gtest.h"


TEST(getSharedInstance, basics)
{
    auto i1 = too::getSharedInstance<int>();
    // yes you can assume the implementation to call a value initializing make_shared (for POD types)
    EXPECT_EQ(*i1, 0);
    *i1 = 42;
    auto i2 = i1;
    i1 = nullptr;
    EXPECT_EQ(*i2, 42);
    i2 = nullptr;

    i1 = too::getSharedInstance<int>();
    *i1 = 42;
    i2 = too::getSharedInstance<int>();
    EXPECT_EQ(*i2, 42);
    i1 = nullptr;
    i2 = too::getSharedInstance<int>();
    EXPECT_EQ(*i2, 42);
    i2 = nullptr;
    i2 = too::getSharedInstance<int>();
    EXPECT_EQ(*i2, 0);
}
