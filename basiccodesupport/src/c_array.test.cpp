#include "ul/c_array.h"
#include "gtest/gtest.h"
#include <cstddef>

namespace ul = mb::ul;

TEST(c_array_lengthTest, test) {
    const size_t size = 5;
    int x[size] = {}; // NOLINT
    EXPECT_EQ(size, ul::c_array_length(x));
}
