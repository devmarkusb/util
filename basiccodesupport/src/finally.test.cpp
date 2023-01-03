#include "ul/finally.h"
#include "ul/macros.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(FinallyTest, Test)
{
    bool isChanged = false;
    {
        const auto UL_ANONYMOUS_VARIABLE = ul::finally(
            [&]()
            {
                isChanged = true;
            });
        isChanged = false;
    }
    EXPECT_TRUE(isChanged);
}
