#include "linear.h"
#include "toolib/ignore_arg.h"
#include <cstddef>
#include "gtest/gtest.h"

using too::mem::Bytes;


TEST(alloc_Linear, constr)
{
    too::mem::alloc::Linear l;
    EXPECT_EQ(l.size(), Bytes{0});

    EXPECT_THROW(l.allocate(Bytes{1}), std::bad_alloc);

    EXPECT_DEBUG_DEATH(l.resize(Bytes{1}), "[Aa]ssert");
}

TEST(alloc_Linear, prealloc)
{
    too::mem::alloc::Linear l;
    using Type = int;
    l.preallocate(Bytes{10 * sizeof(Type)}, Bytes{alignof(Type)});

    EXPECT_EQ(l.size(), Bytes{0});

    EXPECT_THROW(l.allocate(Bytes{11 * sizeof(Type)}), std::bad_alloc);

    EXPECT_DEBUG_DEATH(l.resize(Bytes{11 * sizeof(Type)}), "[Aa]ssert");
}

TEST(alloc_Linear, alloc)
{
    too::mem::alloc::Linear l;
    using Type = int;
    l.preallocate(Bytes{10 * sizeof(Type)}, Bytes{alignof(Type)});

    auto p = reinterpret_cast<Type*>(l.allocate(Bytes{5 * sizeof(Type)}));
    EXPECT_EQ(l.size(), Bytes{5 * sizeof(Type)});
    p[0] = 1;
    p[4] = 1;
    EXPECT_EQ(p[0], 1);
    EXPECT_EQ(p[4], 1);

    p = reinterpret_cast<Type*>(l.allocate(Bytes{5 * sizeof(Type)}));
    too::ignore_arg(p);
    EXPECT_EQ(l.size(), Bytes{10 * sizeof(Type)});

    EXPECT_THROW(l.allocate(Bytes{1 * sizeof(Type)}), std::bad_alloc);
}

TEST(alloc_Linear, dealloc)
{
    too::mem::alloc::Linear l;
    using Type = int;
    l.preallocate(Bytes{10 * sizeof(Type)}, Bytes{alignof(Type)});

    auto p = reinterpret_cast<Type*>(l.allocate(Bytes{5 * sizeof(Type)}));
    too::ignore_arg(p);

    l.deallocate(reinterpret_cast<uint8_t*>(p));

    EXPECT_EQ(l.size(), Bytes{5 * sizeof(Type)});
}

TEST(alloc_Linear, resize)
{
    too::mem::alloc::Linear l;
    using Type = int;
    l.preallocate(Bytes{10 * sizeof(Type)}, Bytes{alignof(Type)});

    auto p = reinterpret_cast<Type*>(l.allocate(Bytes{5 * sizeof(Type)}));
    too::ignore_arg(p);

    l.resize(Bytes{0});
    EXPECT_EQ(l.size(), Bytes{0});

    p = reinterpret_cast<Type*>(l.allocate(Bytes{10 * sizeof(Type)}));
    too::ignore_arg(p);
    EXPECT_EQ(l.size(), Bytes{10 * sizeof(Type)});
}

TEST(alloc_Linear, free)
{
    too::mem::alloc::Linear l;
    using Type = int;
    l.preallocate(Bytes{10 * sizeof(Type)}, Bytes{alignof(Type)});
    l.free();

    EXPECT_THROW(l.allocate(Bytes{1 * sizeof(Type)}), std::bad_alloc);

    EXPECT_DEBUG_DEATH(l.resize(Bytes{1}), "[Aa]ssert");
}
