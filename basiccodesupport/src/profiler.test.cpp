#include "toolib/profiler.h"
#include "gtest/gtest.h"
#include <thread>


TEST(profilerTest, basics)
{
    const auto before{too::profiler_now()};
    std::this_thread::sleep_for(std::chrono::milliseconds(42));
    const auto after{too::profiler_now()};
    EXPECT_GT(after, before);
    const auto diff{too::profiler_diff_s(before, after)};
    EXPECT_GT(0.045, diff);
    EXPECT_LT(0.039, diff);
}
