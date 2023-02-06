#include "ul/ptr2ref.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(ptr2refTest, test)
{
    constexpr auto someNr{5};
    int i = someNr;
    int* pi = &i;
    int& refi = ul::ptr2Ref(pi);
    EXPECT_TRUE(&refi == pi);
    const int& crefi = ul::ptr2RefC(pi);
    EXPECT_TRUE(&crefi == pi);
}
