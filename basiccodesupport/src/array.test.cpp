#include "ul/array.h"
#include "ul/std/array.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(ct_accumulate, array)
{
    const auto arr = ul::array::make(1, 2, 3, 4);
    EXPECT_EQ(ul::ct_accumulate(arr, 0), 1 + 2 + 3 + 4);
}

TEST(ct_accumulate, c_array)
{
    const int arr[]{1, 2, 3, 4};
    EXPECT_EQ(ul::ct_accumulate(arr, 0), 1 + 2 + 3 + 4);
}
