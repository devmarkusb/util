#include "linear.h"
#include "toolib/ignore_arg.h"
#include "toolib/PPDEFS.h"
#include <cstddef>
#include "gtest/gtest.h"

using too::mem::Bytes;


TEST(alloc_Linear, constr)
{
    too::mem::alloc::Linear<> a{{}, {}};
    EXPECT_EQ(a.size(), Bytes{0});

    EXPECT_THROW(a.allocate(Bytes{1}), std::bad_alloc);

#if !TOO_COMP_MS_VISUAL_STUDIO_CPP
    EXPECT_DEBUG_DEATH(a.resize(Bytes{1}), "[Aa]ssert");
#endif
}

TEST(alloc_Linear, prealloc)
{
    using Type = int;
    too::mem::alloc::Linear<> a{Bytes{10 * sizeof(Type)}, Bytes{alignof(Type)}};

    EXPECT_EQ(a.size(), Bytes{0});

    EXPECT_THROW(a.allocate(Bytes{11 * sizeof(Type)}), std::bad_alloc);

#if !TOO_COMP_MS_VISUAL_STUDIO_CPP
    EXPECT_DEBUG_DEATH(a.resize(Bytes{11 * sizeof(Type)}), "[Aa]ssert");
#endif
}

TEST(alloc_Linear, alloc)
{
    using Type = int;
    too::mem::alloc::Linear<> a{Bytes{10 * sizeof(Type)}, Bytes{alignof(Type)}};

    auto p = reinterpret_cast<Type*>(a.allocate(Bytes{5 * sizeof(Type)}));
    EXPECT_EQ(a.size(), Bytes{5 * sizeof(Type)});
    p[0] = 1;
    p[4] = 1;
    EXPECT_EQ(p[0], 1);
    EXPECT_EQ(p[4], 1);

    p = reinterpret_cast<Type*>(a.allocate(Bytes{5 * sizeof(Type)}));
    too::ignore_arg(p);
    EXPECT_EQ(a.size(), Bytes{10 * sizeof(Type)});

    EXPECT_THROW(a.allocate(Bytes{1 * sizeof(Type)}), std::bad_alloc);
}

TEST(alloc_Linear, dealloc)
{
    using Type = int;
    too::mem::alloc::Linear<> a{Bytes{10 * sizeof(Type)}, Bytes{alignof(Type)}};

    auto p = reinterpret_cast<Type*>(a.allocate(Bytes{5 * sizeof(Type)}));
    too::ignore_arg(p);

    a.deallocate(reinterpret_cast<uint8_t*>(p), Bytes{5 * sizeof(Type)});

    EXPECT_EQ(a.size(), Bytes{0});
}

TEST(alloc_Linear, dealloc_noop)
{
    using Type = int;
    too::mem::alloc::Linear<> a{Bytes{10 * sizeof(Type)}, Bytes{alignof(Type)}};

    auto p = reinterpret_cast<Type*>(a.allocate(Bytes{5 * sizeof(Type)}));
    too::ignore_arg(p);

    auto q = reinterpret_cast<Type*>(a.allocate(Bytes{5 * sizeof(Type)}));
    too::ignore_arg(q);

    a.deallocate(reinterpret_cast<uint8_t*>(p), Bytes{5 * sizeof(Type)});

    EXPECT_EQ(a.size(), Bytes{10 * sizeof(Type)});
}

TEST(alloc_Linear, resize)
{
    using Type = int;
    too::mem::alloc::Linear<> a{Bytes{10 * sizeof(Type)}, Bytes{alignof(Type)}};

    auto p = reinterpret_cast<Type*>(a.allocate(Bytes{5 * sizeof(Type)}));
    too::ignore_arg(p);

    a.resize(Bytes{0});
    EXPECT_EQ(a.size(), Bytes{0});

    p = reinterpret_cast<Type*>(a.allocate(Bytes{10 * sizeof(Type)}));
    too::ignore_arg(p);
    EXPECT_EQ(a.size(), Bytes{10 * sizeof(Type)});
}

TEST(alloc_Linear, with_stats)
{
    using Type = int;
    too::mem::alloc::Linear<too::mem::alloc::Statistics> a{Bytes{10 * sizeof(Type)}, Bytes{alignof(Type)}};

    auto p = reinterpret_cast<Type*>(a.allocate(Bytes{5 * sizeof(Type)}));
    too::ignore_arg(p);

    a.deallocate(reinterpret_cast<uint8_t*>(p), Bytes{5 * sizeof(Type)});

    p = reinterpret_cast<Type*>(a.allocate(Bytes{4 * sizeof(Type)}));
    too::ignore_arg(p);

    ASSERT_TRUE(a.peak());
    EXPECT_EQ(*a.peak(), Bytes{5 * sizeof(Type)});
}
