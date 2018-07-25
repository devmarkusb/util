#include "Toolib/std/std_extensions.h"
#include <functional>
#include <vector>
#include "gtest/gtest.h"


TEST(too_make_uniqueTest, test)
{
    std::unique_ptr<int> pi = too::make_unique<int>(1);
    EXPECT_TRUE(pi != nullptr);
}

TEST(too_accumulateTest, test)
{
    std::vector<int> v{1, 2, 3, 4, 5};
    int sumSelfmade = 0;
    for (const auto& elem : v)
        sumSelfmade += elem;
    int sum = too::accumulate(v.begin(), v.end(), 0, std::plus<int>());
    EXPECT_TRUE(sum == sumSelfmade);
}
