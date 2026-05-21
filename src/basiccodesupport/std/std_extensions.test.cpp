#include "mb/ul/basiccodesupport/std/std_extensions.hpp"
#include "gtest/gtest.h"
#include <functional>
#include <memory>
#include <numeric>
#include <vector>

namespace ul = mb::ul;

TEST(ul_make_uniqueTest, test) {
    const std::unique_ptr<int> pi = ul::make_unique<int>(1);
    EXPECT_TRUE(pi != nullptr);
}

TEST(ul_accumulateTest, test) {
    std::vector<int> v{1, 2, 3, 4, 5}; // NOLINT
    const int sum_expected = std::accumulate(v.begin(), v.end(), 0);
    const int sum = ul::accumulate(v.begin(), v.end(), 0, std::plus<>());
    EXPECT_TRUE(sum == sum_expected);
}
