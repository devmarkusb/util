#include "toolib/finally.h"
#include "gtest/gtest.h"

#include "toolib/macros.h"


TEST(FinallyTest, Test)
{
    bool isChanged = false;
    {
        const auto TOO_ANONYMOUS_VARIABLE = too::finally([&]() {
            isChanged = true;
        });
        isChanged = false;
    }
    EXPECT_TRUE(isChanged);
}