#include "toolib/mem/allocator.h"
#include "toolib/mem/alloc/default.h"
#include "toolib/mem/alloc/example.h"
#include "toolib/mem/alloc/linear.h"
#include "toolib/mem/alloc/onstack.h"
#include "toolib/mem/compiler_quirks.h"
#include "toolib/mem/types.h"
#include "gtest/gtest.h"
#include <map>
#include <string>
#include <vector>

using Bytes = too::mem::Bytes;


template <typename Vector>
void common_vector_test(Vector&& v)
{
    v.reserve(100'000);
    EXPECT_TRUE(v.empty());
    v.resize(50'000);
    EXPECT_EQ(v.size(), 50'000);
    v.resize(100'000);
    EXPECT_EQ(v.size(), 100'000);
    v[50'001] = 42;
    EXPECT_EQ(v[50'001], 42);
    v.pop_back();
    EXPECT_EQ(v.size(), 99'999);
    v.clear();
    EXPECT_TRUE(v.empty());
    v.shrink_to_fit();
    // implementation dependent according to doc., so can't test anything
    //EXPECT_EQ(v.capacity(), 0);
    v.push_back(42);
    EXPECT_EQ(v.size(), 1);
    EXPECT_GE(v.capacity(), 1);
    for (int i = 1; i <= 999; ++i)
    {
        v.push_back(42);
    }
    std::cout << "capacity: " << v.capacity() << "\n";
    EXPECT_EQ(v.size(), 1000);
}

template <typename Map>
void common_map_test(Map&& m)
{
    EXPECT_TRUE(m.empty());
    m[0] = "0";
    EXPECT_EQ(m.size(), 1);
    for (int i = 1; i <= 999; ++i)
    {
        m[i] = std::to_string(i);
    }
    EXPECT_EQ(m.size(), 1'000);
}

TEST(allocator_example, vector)
{
    using Arena = too::mem::alloc::Example;
    using Allocator = too::mem::Allocator<int, Arena>;
    Arena a;
    Allocator al{a};
    std::vector<int, Allocator> v{al};

    common_vector_test(v);
}

TEST(allocator_example, map)
{
    using Arena = too::mem::alloc::Example;
    using MapPair = std::pair<const int, std::string>;
    using Allocator = too::mem::Allocator<MapPair, Arena>;
    Arena a;
    Allocator al{a};
    std::map<int, std::string, std::less<>, Allocator> m{al};

    common_map_test(m);
}

TEST(allocator_default, vector)
{
    using Arena = too::mem::alloc::DefaultNewDelete<>;
    using Allocator = too::mem::Allocator<int, Arena>;
    Arena a;
    Allocator al{a};
    std::vector<int, Allocator> v{al};

    common_vector_test(v);
}

TEST(allocator_default, map)
{
    using Arena = too::mem::alloc::DefaultNewDelete<>;
    using MapPair = std::pair<const int, std::string>;
    using Allocator = too::mem::Allocator<MapPair, Arena>;
    Arena a;
    Allocator al{a};
    std::map<int, std::string, std::less<>, Allocator> m{al};

    common_map_test(m);
}

TEST(allocator_linear, vector)
{
    using Arena = too::mem::alloc::Linear<>;
    using Allocator = too::mem::Allocator<int, Arena>;
    try
    {
        Arena a{
            Bytes{100'000 * sizeof(int) + too::mem::quirk::vector::constr_heap_alloc_size.value}, Bytes{alignof(int)}};
        Allocator al{a};
        std::vector<int, Allocator> v{al};

        common_vector_test(v);
    }
    catch (const std::bad_alloc&)
    {
        std::cout << "low mem, omitted test";
    }
}

TEST(allocator_linear, map)
{
    using Arena = too::mem::alloc::Linear<>;
    using MapPair = std::pair<const int, std::string>;
    using Allocator = too::mem::Allocator<MapPair, Arena>;
    std::cout << "sizeof(MapPair): " << sizeof(MapPair) << ", alignof(MapPair): " << alignof(MapPair) << "\n";
    Arena a{Bytes{100'000}, Bytes{alignof(MapPair)}};
    Allocator al{a};
    std::map<int, std::string, std::less<>, Allocator> m{al};

    common_map_test(m);
}

TEST(allocator_onstack, vector)
{
    using Arena = too::mem::alloc::OnStack<
        100'000 * sizeof(int) + too::mem::quirk::vector::constr_heap_alloc_size.value, alignof(int)>;
    using Allocator = too::mem::Allocator<int, Arena>;
    try
    {
        Arena a;
        Allocator al{a};
        std::vector<int, Allocator> v{al};

        common_vector_test(v);
    }
    catch (const std::bad_alloc&)
    {
        std::cout << "low mem, omitted test";
    }
}

TEST(allocator_onstack, map)
{
    using MapPair = std::pair<const int, std::string>;
    using Arena = too::mem::alloc::OnStack<100'000, alignof(MapPair)>;
    using Allocator = too::mem::Allocator<MapPair, Arena>;
    Arena a;
    Allocator al{a};
    std::map<int, std::string, std::less<>, Allocator> m{al};

    common_map_test(m);
}
