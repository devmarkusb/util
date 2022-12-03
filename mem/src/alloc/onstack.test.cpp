#include "toolib/mem/alloc/onstack.h"
#include "toolib/ignore_arg.h"
#include "gtest/gtest.h"
#include <cstddef>

using too::mem::Bytes;


TEST(alloc_OnStack, constr)
{
    too::mem::alloc::OnStack<1024, 1> a;
    EXPECT_EQ(a.size(), Bytes{0});
    EXPECT_EQ(a.capacity(), Bytes{1024});

    EXPECT_THROW(a.allocate(Bytes{1025}), std::bad_alloc);
}

TEST(alloc_OnStack, alloc)
{
    too::mem::alloc::OnStack<1024, 1> a;

    auto p = reinterpret_cast<char*>(a.allocate(Bytes{1000}));

    EXPECT_EQ(a.size(), Bytes{1000});
    p[0] = 'a';
    p[999] = 'a';
    EXPECT_EQ(p[0], 'a');
    EXPECT_EQ(p[999], 'a');

    p = reinterpret_cast<char*>(a.allocate(Bytes{24}));
    too::ignore_arg(p);
    EXPECT_EQ(a.size(), Bytes{1024});

    EXPECT_THROW(a.allocate(Bytes{1}), std::bad_alloc);
}

TEST(alloc_OnStack, dealloc)
{
    using Type = int;
    static_assert(alignof(Type) == sizeof(Type));
    too::mem::alloc::OnStack<1024, alignof(Type)> a;

    auto p = reinterpret_cast<Type*>(a.allocate(Bytes{100 * sizeof(Type)}));
    too::ignore_arg(p);

    a.deallocate(reinterpret_cast<uint8_t*>(p), Bytes{100 * sizeof(Type)});
    EXPECT_EQ(a.size(), Bytes{0});

    p = reinterpret_cast<Type*>(a.allocate(Bytes{100 * sizeof(Type)}));
    too::ignore_arg(p);
    p = reinterpret_cast<Type*>(a.allocate(Bytes{100 * sizeof(Type)}));
    too::ignore_arg(p);
    EXPECT_EQ(a.size(), Bytes{200 * sizeof(Type)});

    a.reset();

    EXPECT_EQ(a.size(), Bytes{0});
}

TEST(alloc_OnStack, padding)
{
    too::mem::alloc::OnStack<16, 4> a;

    auto p = reinterpret_cast<char*>(a.allocate(Bytes{1}));
    too::ignore_arg(p);
    EXPECT_EQ(a.size(), Bytes{4});
    p = reinterpret_cast<char*>(a.allocate(Bytes{1}));
    too::ignore_arg(p);
    p = reinterpret_cast<char*>(a.allocate(Bytes{1}));
    too::ignore_arg(p);
    p = reinterpret_cast<char*>(a.allocate(Bytes{1}));

    EXPECT_EQ(a.size(), Bytes{16});

    too::mem::alloc::OnStack<16, 4> a2;

    p = reinterpret_cast<char*>(a2.allocate(Bytes{3}));
    too::ignore_arg(p);
    EXPECT_EQ(a2.size(), Bytes{4});
}

TEST(alloc_OnStack, with_stats)
{
    using Type = int;
    static_assert(alignof(Type) == sizeof(Type));
    too::mem::alloc::OnStack<1024, alignof(Type), too::mem::alloc::Statistics> a;

    auto p = reinterpret_cast<Type*>(a.allocate(Bytes{20 * sizeof(Type)}));
    too::ignore_arg(p);
    p = reinterpret_cast<Type*>(a.allocate(Bytes{100 * sizeof(Type)}));
    a.deallocate(reinterpret_cast<uint8_t*>(p), Bytes{100 * sizeof(Type)});
    p = reinterpret_cast<Type*>(a.allocate(Bytes{90 * sizeof(Type)}));
    too::ignore_arg(p);

    ASSERT_TRUE(a.peak());
    EXPECT_EQ(*a.peak(), Bytes{120 * sizeof(Type)});
}
