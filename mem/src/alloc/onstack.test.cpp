#include "ul/mem/alloc/onstack.h"
#include "ul/ignore_unused.h"
#include "gtest/gtest.h"
#include <cstddef>

namespace ul = mb::ul;
using ul::mem::Bytes;

TEST(alloc_OnStack, constr)
{
    ul::mem::alloc::OnStack<1024, 1> a;
    EXPECT_EQ(a.size(), Bytes{0});
    EXPECT_EQ(a.capacity(), Bytes{1024});

    EXPECT_THROW(a.allocate(Bytes{1025}), std::bad_alloc);
}

TEST(alloc_OnStack, alloc)
{
    ul::mem::alloc::OnStack<1024, 1> a;

    auto p = reinterpret_cast<char*>(a.allocate(Bytes{1000}));

    EXPECT_EQ(a.size(), Bytes{1000});
    p[0] = 'a';
    p[999] = 'a';
    EXPECT_EQ(p[0], 'a');
    EXPECT_EQ(p[999], 'a');

    p = reinterpret_cast<char*>(a.allocate(Bytes{24}));
    ul::ignore_unused(p);
    EXPECT_EQ(a.size(), Bytes{1024});

    EXPECT_THROW(a.allocate(Bytes{1}), std::bad_alloc);
}

TEST(alloc_OnStack, dealloc)
{
    using Type = int;
    static_assert(alignof(Type) == sizeof(Type));
    ul::mem::alloc::OnStack<1024, alignof(Type)> a;

    auto p = reinterpret_cast<Type*>(a.allocate(Bytes{100 * sizeof(Type)}));
    ul::ignore_unused(p);

    a.deallocate(reinterpret_cast<uint8_t*>(p), Bytes{100 * sizeof(Type)});
    EXPECT_EQ(a.size(), Bytes{0});

    p = reinterpret_cast<Type*>(a.allocate(Bytes{100 * sizeof(Type)}));
    ul::ignore_unused(p);
    p = reinterpret_cast<Type*>(a.allocate(Bytes{100 * sizeof(Type)}));
    ul::ignore_unused(p);
    EXPECT_EQ(a.size(), Bytes{200 * sizeof(Type)});

    a.reset();

    EXPECT_EQ(a.size(), Bytes{0});
}

TEST(alloc_OnStack, padding)
{
    ul::mem::alloc::OnStack<16, 4> a;

    auto p = reinterpret_cast<char*>(a.allocate(Bytes{1}));
    ul::ignore_unused(p);
    EXPECT_EQ(a.size(), Bytes{4});
    p = reinterpret_cast<char*>(a.allocate(Bytes{1}));
    ul::ignore_unused(p);
    p = reinterpret_cast<char*>(a.allocate(Bytes{1}));
    ul::ignore_unused(p);
    p = reinterpret_cast<char*>(a.allocate(Bytes{1}));
    ul::ignore_unused(p);

    EXPECT_EQ(a.size(), Bytes{16});

    ul::mem::alloc::OnStack<16, 4> a2;

    p = reinterpret_cast<char*>(a2.allocate(Bytes{3}));
    ul::ignore_unused(p);
    EXPECT_EQ(a2.size(), Bytes{4});
}

TEST(alloc_OnStack, with_stats)
{
    using Type = int;
    static_assert(alignof(Type) == sizeof(Type));
    ul::mem::alloc::OnStack<1024, alignof(Type), ul::mem::alloc::Statistics> a;

    auto p = reinterpret_cast<Type*>(a.allocate(Bytes{20 * sizeof(Type)}));
    ul::ignore_unused(p);
    p = reinterpret_cast<Type*>(a.allocate(Bytes{100 * sizeof(Type)}));
    a.deallocate(reinterpret_cast<uint8_t*>(p), Bytes{100 * sizeof(Type)});
    p = reinterpret_cast<Type*>(a.allocate(Bytes{90 * sizeof(Type)}));
    ul::ignore_unused(p);

    ASSERT_TRUE(a.peak());
    EXPECT_EQ(*a.peak(), Bytes{120 * sizeof(Type)});
}
