#include "toolib/array.h"
#include "toolib/std/array.h"
#include "gtest/gtest.h"


TEST(ct_accumulate, array)
{
    const auto arr = too::array::make(1, 2, 3, 4);
    EXPECT_EQ(too::ct_accumulate(arr, 0), 1 + 2 + 3 + 4);
}

TEST(ct_accumulate, c_array)
{
    const int arr[]{1, 2, 3, 4};
    EXPECT_EQ(too::ct_accumulate(arr, 0), 1 + 2 + 3 + 4);
}
