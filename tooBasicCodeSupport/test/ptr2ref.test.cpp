#include "Toolib/ptr2ref.h"
#include "gtest/gtest.h"


TEST(ptr2refTest, test)
{
    int i     = 5;
    int* pi   = &i;
    int& refi = too::Ptr2Ref(pi);
    EXPECT_TRUE(&refi == pi);
    const int& crefi = too::Ptr2RefC(pi);
    EXPECT_TRUE(&crefi == pi);
}
