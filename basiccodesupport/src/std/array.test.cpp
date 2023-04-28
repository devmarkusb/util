#include "ul/std/array.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(array_make, basics) {
    const auto a = ul::array::make(42, 43, 44);
    ASSERT_EQ(a.size(), 3u);
    EXPECT_EQ(a[0], 42);
    EXPECT_EQ(a[1], 43);
    EXPECT_EQ(a[2], 44);
}
