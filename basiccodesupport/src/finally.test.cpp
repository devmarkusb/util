#include "ul/finally.h"
#include "gtest/gtest.h"

#include "ul/macros.h"

namespace ul = mb::ul;

TEST(FinallyTest, Test)
{
    bool isChanged = false;
    {
        const auto UL_ANONYMOUS_VARIABLE = ul::finally([&]() {
            isChanged = true;
        });
        isChanged = false;
    }
    EXPECT_TRUE(isChanged);
}
