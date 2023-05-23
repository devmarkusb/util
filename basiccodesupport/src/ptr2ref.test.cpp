#include "ul/ptr2ref.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(ptr2refTest, test) {
    constexpr auto some_nr{5};
    int i = some_nr;
    int* pi = &i;
    int& refi = ul::ptr2ref(pi);
    EXPECT_TRUE(&refi == pi);
    const int& crefi = ul::ptr2ref(pi);
    EXPECT_TRUE(&crefi == pi);
}
