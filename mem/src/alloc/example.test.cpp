#include "toolib/mem/alloc/example.h"
#include "toolib/finally.h"
#include "toolib/ignore_arg.h"
#include "toolib/mem/types.h"
#include "gtest/gtest.h"

using too::mem::Bytes;


TEST(alloc_Example, basics)
{
    too::mem::alloc::Example a;
    auto mem = reinterpret_cast<int*>(a.allocate(Bytes{42 * sizeof(int)}));
    const auto autoDeallocate = too::finally([mem, &a]() {
        a.deallocate(reinterpret_cast<uint8_t*>(mem));
    });
    too::ignore_arg(autoDeallocate);

    mem[0] = 1;
    mem[41] = 1;
    EXPECT_EQ(mem[0], 1);
    EXPECT_EQ(mem[41], 1);
}
