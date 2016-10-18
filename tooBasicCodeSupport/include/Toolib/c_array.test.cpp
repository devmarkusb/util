#include "Toolib/c_array.h"
#include "gtest/gtest.h"


TEST(c_array_lengthTest, test)
{
    const size_t size = 5;
    int x[size] = {};
    EXPECT_EQ(size, too::c_array_length(x));
}
