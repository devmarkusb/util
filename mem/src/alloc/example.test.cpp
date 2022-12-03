#include "ul/mem/alloc/example.h"
#include "ul/finally.h"
#include "ul/ignore_arg.h"
#include "ul/mem/types.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;
using ul::mem::Bytes;


TEST(alloc_Example, basics)
{
    ul::mem::alloc::Example a;
    auto mem = reinterpret_cast<int*>(a.allocate(Bytes{42 * sizeof(int)}));
    const auto autoDeallocate = ul::finally([mem, &a]() {
        a.deallocate(reinterpret_cast<uint8_t*>(mem));
    });
    ul::ignore_arg(autoDeallocate);

    mem[0] = 1;
    mem[41] = 1;
    EXPECT_EQ(mem[0], 1);
    EXPECT_EQ(mem[41], 1);
}
