#include "ul/mem/alloc/onstack.h"
#include "ul/ignore_unused.h"
#include "ul/mem/alloc/statistics.h"
#include "ul/mem/types.h"
#include "gtest/gtest.h"
#include <new>

namespace ul = mb::ul;
using ul::mem::Bytes;

namespace {
constexpr auto kb{1024};
constexpr auto b1000{1000};
constexpr auto b100{100};
constexpr auto b24{24};
constexpr auto b20{20};
constexpr auto b16{16};
constexpr auto b10{10};
constexpr auto b4{4};
} // namespace

TEST(alloc_OnStack, constr) {
    ul::mem::alloc::OnStack<kb, 1> a;
    EXPECT_EQ(a.size(), Bytes{0});
    EXPECT_EQ(a.capacity(), Bytes{kb});

    EXPECT_THROW(a.allocate(Bytes{kb + 1}), std::bad_alloc);
}

TEST(alloc_OnStack, alloc) {
    ul::mem::alloc::OnStack<kb, 1> a;

    auto* p = reinterpret_cast<char*>(a.allocate(Bytes{b1000}));

    EXPECT_EQ(a.size(), Bytes{b1000});
    p[0] = 'a'; // NOLINT
    p[b1000 - 1] = 'a'; // NOLINT
    EXPECT_EQ(p[0], 'a'); // NOLINT
    EXPECT_EQ(p[b1000 - 1], 'a'); // NOLINT

    p = reinterpret_cast<char*>(a.allocate(Bytes{b24})); // NOLINT
    ul::ignore_unused(p);
    EXPECT_EQ(a.size(), Bytes{kb});

    EXPECT_THROW(a.allocate(Bytes{1}), std::bad_alloc);
}

TEST(alloc_OnStack, dealloc) {
    using Type = int;
    static_assert(alignof(Type) == sizeof(Type)); // NOLINT
    ul::mem::alloc::OnStack<kb, alignof(Type)> a;

    auto* p = reinterpret_cast<Type*>(a.allocate(Bytes{b100 * sizeof(Type)})); // NOLINT
    ul::ignore_unused(p);

    a.deallocate(reinterpret_cast<uint8_t*>(p), Bytes{b100 * sizeof(Type)}); // NOLINT
    EXPECT_EQ(a.size(), Bytes{0});

    p = reinterpret_cast<Type*>(a.allocate(Bytes{b100 * sizeof(Type)})); // NOLINT
    ul::ignore_unused(p);
    p = reinterpret_cast<Type*>(a.allocate(Bytes{b100 * sizeof(Type)})); // NOLINT
    ul::ignore_unused(p);
    EXPECT_EQ(a.size(), Bytes{static_cast<size_t>(2 * b100) * sizeof(Type)});

    a.reset();

    EXPECT_EQ(a.size(), Bytes{0});
}

TEST(alloc_OnStack, padding) {
    ul::mem::alloc::OnStack<b16, b4> a;

    auto* p = reinterpret_cast<char*>(a.allocate(Bytes{1})); // NOLINT
    ul::ignore_unused(p);
    EXPECT_EQ(a.size(), Bytes{b4});
    p = reinterpret_cast<char*>(a.allocate(Bytes{1})); // NOLINT
    ul::ignore_unused(p);
    p = reinterpret_cast<char*>(a.allocate(Bytes{1})); // NOLINT
    ul::ignore_unused(p);
    p = reinterpret_cast<char*>(a.allocate(Bytes{1})); // NOLINT
    ul::ignore_unused(p);

    EXPECT_EQ(a.size(), Bytes{b16});

    ul::mem::alloc::OnStack<b16, b4> a2;

    p = reinterpret_cast<char*>(a2.allocate(Bytes{b4 - 1})); // NOLINT
    ul::ignore_unused(p);
    EXPECT_EQ(a2.size(), Bytes{b4});
}

TEST(alloc_OnStack, with_stats) {
    using Type = int;
    static_assert(alignof(Type) == sizeof(Type)); // NOLINT
    ul::mem::alloc::OnStack<kb, alignof(Type), ul::mem::alloc::Statistics> a;

    auto* p = reinterpret_cast<Type*>(a.allocate(Bytes{b20 * sizeof(Type)})); // NOLINT
    ul::ignore_unused(p);
    p = reinterpret_cast<Type*>(a.allocate(Bytes{b100 * sizeof(Type)})); // NOLINT
    a.deallocate(reinterpret_cast<uint8_t*>(p), Bytes{b100 * sizeof(Type)}); // NOLINT
    p = reinterpret_cast<Type*>(a.allocate(Bytes{(b100 - b10) * sizeof(Type)})); // NOLINT
    ul::ignore_unused(p);

    ASSERT_TRUE(a.peak());
    EXPECT_EQ(*a.peak(), Bytes{(b100 + b20) * sizeof(Type)});
}
