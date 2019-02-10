#include "default.h"
#include "toolib/finally.h"
#include "toolib/ignore_arg.h"
#include "toolib/mem/types.h"
#include "gtest/gtest.h"

using too::mem::Bytes;


TEST(alloc_DefaultNewDelete, basics)
{
    too::mem::alloc::DefaultNewDelete<> a;
    auto mem = reinterpret_cast<int*>(a.allocate(Bytes{42 * sizeof(int)}));
    const auto autoDeallocate = too::finally([mem, &a]()
    { a.deallocate(reinterpret_cast<uint8_t*>(mem), Bytes{42 * sizeof(int)}); });

    mem[0] = 1;
    mem[41] = 1;
    EXPECT_EQ(mem[0], 1);
    EXPECT_EQ(mem[41], 1);
}

TEST(alloc_DefaultNewDelete, with_stats)
{
    using Type = int;
    static_assert(alignof(Type) == sizeof(Type));
    too::mem::alloc::DefaultNewDelete<too::mem::alloc::Statistics> a;

    auto p = reinterpret_cast<Type*>(a.allocate(Bytes{20 * sizeof(Type)}));
    const auto autoDeallocate20 = too::finally([p, &a]()
    { a.deallocate(reinterpret_cast<uint8_t*>(p), Bytes{20 * sizeof(Type)}); });
    p = reinterpret_cast<Type*>(a.allocate(Bytes{100 * sizeof(Type)}));
    a.deallocate(reinterpret_cast<uint8_t*>(p), Bytes{100 * sizeof(Type)});
    p = reinterpret_cast<Type*>(a.allocate(Bytes{90 * sizeof(Type)}));
    const auto autoDeallocate90 = too::finally([p, &a]()
    { a.deallocate(reinterpret_cast<uint8_t*>(p), Bytes{90 * sizeof(Type)}); });

    ASSERT_TRUE(a.peak());
    EXPECT_EQ(*a.peak(), Bytes{120 * sizeof(Type)});
}
