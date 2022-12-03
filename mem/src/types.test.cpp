#include "ul/mem/types.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;
using ul::mem::Bytes;


TEST(Bytes, equal)
{
    EXPECT_EQ(Bytes{4}, Bytes{4});
    EXPECT_NE(Bytes{8}, Bytes{4});
}
