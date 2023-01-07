#include "ul/mem/alloc/example.h"
#include "ul/finally.h"
#include "ul/ignore_arg.h"
#include "ul/mem/types.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;
using ul::mem::Bytes;

TEST(alloc_Example, basics)
{
    auto mem = reinterpret_cast<int*>(ul::mem::alloc::Example::allocate(Bytes{42 * sizeof(int)}));
    const auto autoDeallocate = ul::finally(
        [mem]()
        {
            ul::mem::alloc::Example::deallocate(reinterpret_cast<uint8_t*>(mem));
        });
    ul::ignore_arg(autoDeallocate);

    mem[0] = 1;
    mem[41] = 1;
    EXPECT_EQ(mem[0], 1);
    EXPECT_EQ(mem[41], 1);
}
