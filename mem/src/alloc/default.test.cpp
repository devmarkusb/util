#include "ul/mem/alloc/default.h"
#include "ul/finally.h"
#include "ul/mem/types.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;
using ul::mem::Bytes;

// NOLINTBEGIN
TEST(alloc_DefaultNewDelete, basics) {
    ul::mem::alloc::DefaultNewDelete<> a;
    constexpr auto ex_nr{42};
    auto* mem = reinterpret_cast<int*>(a.allocate(Bytes{ex_nr * sizeof(int)}));
    const auto auto_deallocate = ul::finally([mem, &a]() {
        a.deallocate(reinterpret_cast<uint8_t*>(mem), Bytes{ex_nr * sizeof(int)});
    });

    mem[0] = 1;
    mem[ex_nr - 1] = 1;
    EXPECT_EQ(mem[0], 1);
    EXPECT_EQ(mem[ex_nr - 1], 1);
}

TEST(alloc_DefaultNewDelete, with_stats) {
    using Type = int;
    static_assert(alignof(Type) == sizeof(Type));
    ul::mem::alloc::DefaultNewDelete<ul::mem::alloc::Statistics> a;

    constexpr auto ex_nr20{20};
    auto* p = reinterpret_cast<Type*>(a.allocate(Bytes{ex_nr20 * sizeof(Type)}));
    const auto auto_deallocate20 = ul::finally([p, &a]() {
        a.deallocate(reinterpret_cast<uint8_t*>(p), Bytes{ex_nr20 * sizeof(Type)});
    });
    constexpr auto ex_nr100{100};
    constexpr auto ex_nr90{90};
    p = reinterpret_cast<Type*>(a.allocate(Bytes{ex_nr100 * sizeof(Type)}));
    a.deallocate(reinterpret_cast<uint8_t*>(p), Bytes{ex_nr100 * sizeof(Type)});
    p = reinterpret_cast<Type*>(a.allocate(Bytes{ex_nr90 * sizeof(Type)}));
    const auto auto_deallocate90 = ul::finally([p, &a]() {
        a.deallocate(reinterpret_cast<uint8_t*>(p), Bytes{ex_nr90 * sizeof(Type)});
    });

    ASSERT_TRUE(a.peak());
    EXPECT_EQ(*a.peak(), Bytes{(ex_nr20 + ex_nr100) * sizeof(Type)});
}

// NOLINTEND
