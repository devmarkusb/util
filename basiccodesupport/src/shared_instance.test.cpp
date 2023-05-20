#include "ul/shared_instance.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

namespace {
constexpr auto some_number_choice{42};
} // namespace

TEST(getSharedInstance, basics) {
    auto i1 = ul::get_shared_instance<int>();
    // yes you can assume the implementation to call a value initializing make_shared (for POD types)
    EXPECT_EQ(*i1, 0);
    *i1 = some_number_choice;
    auto i2 = i1;
    i1 = {};
    EXPECT_EQ(*i2, some_number_choice);
    i2 = {};

    i1 = ul::get_shared_instance<int>();
    *i1 = some_number_choice;
    i2 = ul::get_shared_instance<int>();
    EXPECT_EQ(*i2, some_number_choice);
    i1 = {};
    i2 = ul::get_shared_instance<int>();
    EXPECT_EQ(*i2, some_number_choice);
    i2 = {};
    i2 = ul::get_shared_instance<int>();
    EXPECT_EQ(*i2, 0);
}
