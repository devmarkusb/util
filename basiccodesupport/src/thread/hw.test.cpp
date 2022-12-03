#include "toolib/thread/hw.h"
#include "gtest/gtest.h"
#include <thread>


TEST(thread_numAvailLogicalCores, consistency)
{
    EXPECT_EQ(std::thread::hardware_concurrency(), too::thread::numAvailLogicalCores());
}
