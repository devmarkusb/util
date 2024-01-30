#include "ul/mem/alloc/linear.h"
#include "ul/assert.h"
#include "ul/ignore_unused.h"
#include "ul/mem/types.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;
using ul::mem::Bytes;

// NOLINTBEGIN
TEST(alloc_Linear, constr) {
    ul::mem::alloc::Linear<> a{{}};
    EXPECT_EQ(a.size(), Bytes{0});

    EXPECT_THROW(a.allocate(Bytes{1}), std::bad_alloc);

    EXPECT_DEBUG_DEATH(a.resize(Bytes{1}), ul::death_assert_regex);
}

TEST(alloc_Linear, prealloc) {
    using Type = int;
    ul::mem::alloc::Linear<> a{Bytes{10 * sizeof(Type)}, Bytes{alignof(Type)}};

    EXPECT_EQ(a.size(), Bytes{0});

    EXPECT_THROW(a.allocate(Bytes{11 * sizeof(Type)}), std::bad_alloc);

    EXPECT_DEBUG_DEATH(a.resize(Bytes{11 * sizeof(Type)}), ul::death_assert_regex);
}

TEST(alloc_Linear, alloc) {
    using Type = int;
    ul::mem::alloc::Linear<> a{Bytes{10 * sizeof(Type)}, Bytes{alignof(Type)}};

    auto* p = reinterpret_cast<Type*>(a.allocate(Bytes{5 * sizeof(Type)}));
    EXPECT_EQ(a.size(), Bytes{5 * sizeof(Type)});
    p[0] = 1;
    p[4] = 1;
    EXPECT_EQ(p[0], 1);
    EXPECT_EQ(p[4], 1);

    p = reinterpret_cast<Type*>(a.allocate(Bytes{5 * sizeof(Type)}));
    ul::ignore_unused(p);
    EXPECT_EQ(a.size(), Bytes{10 * sizeof(Type)});

    EXPECT_THROW(a.allocate(Bytes{1 * sizeof(Type)}), std::bad_alloc);
}

TEST(alloc_Linear, dealloc) {
    using Type = int;
    ul::mem::alloc::Linear<> a{Bytes{10 * sizeof(Type)}, Bytes{alignof(Type)}};

    auto* p = reinterpret_cast<Type*>(a.allocate(Bytes{5 * sizeof(Type)}));
    ul::ignore_unused(p);

    a.deallocate(reinterpret_cast<uint8_t*>(p), Bytes{5 * sizeof(Type)});

    EXPECT_EQ(a.size(), Bytes{0});
}

TEST(alloc_Linear, dealloc_noop) {
    using Type = int;
    ul::mem::alloc::Linear<> a{Bytes{10 * sizeof(Type)}, Bytes{alignof(Type)}};

    auto* p = reinterpret_cast<Type*>(a.allocate(Bytes{5 * sizeof(Type)}));
    ul::ignore_unused(p);

    auto* q = reinterpret_cast<Type*>(a.allocate(Bytes{5 * sizeof(Type)}));
    ul::ignore_unused(q);

    a.deallocate(reinterpret_cast<uint8_t*>(p), Bytes{5 * sizeof(Type)});

    EXPECT_EQ(a.size(), Bytes{10 * sizeof(Type)});
}

TEST(alloc_Linear, resize) {
    using Type = int;
    ul::mem::alloc::Linear<> a{Bytes{10 * sizeof(Type)}, Bytes{alignof(Type)}};

    auto* p = reinterpret_cast<Type*>(a.allocate(Bytes{5 * sizeof(Type)}));
    ul::ignore_unused(p);

    a.resize(Bytes{0});
    EXPECT_EQ(a.size(), Bytes{0});

    p = reinterpret_cast<Type*>(a.allocate(Bytes{10 * sizeof(Type)}));
    ul::ignore_unused(p);
    EXPECT_EQ(a.size(), Bytes{10 * sizeof(Type)});
}

TEST(alloc_Linear, with_stats) {
    using Type = int;
    ul::mem::alloc::Linear<ul::mem::alloc::Statistics> a{Bytes{10 * sizeof(Type)}, Bytes{alignof(Type)}};

    auto* p = reinterpret_cast<Type*>(a.allocate(Bytes{5 * sizeof(Type)}));
    ul::ignore_unused(p);

    a.deallocate(reinterpret_cast<uint8_t*>(p), Bytes{5 * sizeof(Type)});

    p = reinterpret_cast<Type*>(a.allocate(Bytes{4 * sizeof(Type)}));
    ul::ignore_unused(p);

    ASSERT_TRUE(a.peak());
    EXPECT_EQ(*a.peak(), Bytes{5 * sizeof(Type)});
}

// NOLINTEND
