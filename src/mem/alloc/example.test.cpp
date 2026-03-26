#include "mb/ul/mem/alloc/example.hpp"
#include "mb/ul/basiccodesupport/finally.hpp"
#include "mb/ul/basiccodesupport/ignore-unused.hpp"
#include "mb/ul/mem/types.hpp"
#include "gtest/gtest.h"

namespace ul = mb::ul;
using ul::mem::Bytes;

// NOLINTBEGIN
TEST(alloc_Example, basics) {
    auto* mem = reinterpret_cast<int*>(ul::mem::alloc::Example::allocate(Bytes{42 * sizeof(int)}));
    const auto auto_deallocate = ul::finally([mem]() {
        ul::mem::alloc::Example::deallocate(reinterpret_cast<uint8_t*>(mem));
    });
    ul::ignore_unused(auto_deallocate);

    mem[0] = 1;
    mem[41] = 1;
    EXPECT_EQ(mem[0], 1);
    EXPECT_EQ(mem[41], 1);
}

// NOLINTEND
