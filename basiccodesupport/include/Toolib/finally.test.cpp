#include "Toolib/finally.h"
#include "Toolib/PPDEFS.h"
#include "gtest/gtest.h"


TEST(FinallyTest, Test)
{
    bool isChanged = false;
    {
        const auto TOO_ANONYMOUS_VARIABLE = too::finally([&]()
            {
                isChanged = true;
            });
        isChanged = false;
    }
    EXPECT_TRUE(isChanged);
}
