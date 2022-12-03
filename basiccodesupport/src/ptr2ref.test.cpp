#include "ul/ptr2ref.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(ptr2refTest, test)
{
    int i = 5;
    int* pi = &i;
    int& refi = ul::Ptr2Ref(pi);
    EXPECT_TRUE(&refi == pi);
    const int& crefi = ul::Ptr2RefC(pi);
    EXPECT_TRUE(&crefi == pi);
}
