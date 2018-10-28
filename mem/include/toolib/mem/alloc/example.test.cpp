#include "example.h"
#include "toolib/finally.h"
#include "toolib/ignore_arg.h"
#include "gtest/gtest.h"


TEST(alloc_example, basics)
{
    too::mem::alloc::Example e;
    auto mem = e.allocate<int>(42);
    const auto autoDeallocate = [mem, &e]()
    { e.deallocate(mem); };
    too::ignore_arg(autoDeallocate);

    mem[0] = 1;
    mem[41] = 1;
    EXPECT_EQ(mem[0], 1);
    EXPECT_EQ(mem[41], 1);
    for (size_t i = 1; i <= 40; ++i)
        EXPECT_EQ(mem[i], 0);
}
