#include "ul/std/std_extensions.h"
#include "gtest/gtest.h"
#include <functional>
#include <vector>

namespace ul = mb::ul;

TEST(ul_make_uniqueTest, test)
{
    const std::unique_ptr<int> pi = ul::make_unique<int>(1);
    EXPECT_TRUE(pi != nullptr);
}

TEST(ul_accumulateTest, test)
{
    std::vector<int> v{1, 2, 3, 4, 5}; // NOLINT
    int sumSelfmade = 0;
    for (const auto& elem : v)
        sumSelfmade += elem;
    const int sum = ul::accumulate(v.begin(), v.end(), 0, std::plus<>());
    EXPECT_TRUE(sum == sumSelfmade);
}
