#include "ul/mem/allocator.h"
#include "ul/mem/alloc/default.h"
#include "ul/mem/alloc/example.h"
#include "ul/mem/alloc/linear.h"
#include "ul/mem/alloc/onstack.h"
#include "ul/mem/compiler_quirks.h"
#include "ul/mem/types.h"
#include "gtest/gtest.h"
#include <functional>
#include <iostream>
#include <map>
#include <new>
#include <string>
#include <utility>
#include <vector>

namespace ul = mb::ul;
using Bytes = ul::mem::Bytes;

namespace {
constexpr auto mem100k{100'000};
} // namespace

template <typename Vector>
void common_vector_test(Vector& v) {
    // NOLINTBEGIN
    v.reserve(mem100k);
    EXPECT_TRUE(v.empty());
    v.resize(50'000);
    EXPECT_EQ(v.size(), 50'000);
    v.resize(mem100k);
    EXPECT_EQ(v.size(), mem100k);
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
    for (int i = 1; i <= 999; ++i) {
        v.push_back(42);
    }
    // NOLINTEND
    std::cout << "capacity: " << v.capacity() << "\n";
    EXPECT_EQ(v.size(), 1000);
}

template <typename Map>
void common_map_test(Map& m) {
    EXPECT_TRUE(m.empty());
    m[0] = "0";
    EXPECT_EQ(m.size(), 1);
    for (int i = 1; i <= 999; ++i) // NOLINT
    {
        m[i] = std::to_string(i);
    }
    EXPECT_EQ(m.size(), 1'000);
}

TEST(allocator_example, vector) {
    using Arena = ul::mem::alloc::Example;
    using Allocator = ul::mem::Allocator<int, Arena>;
    Arena a;
    const Allocator al{a};
    std::vector<int, Allocator> v{al};

    common_vector_test(v);
}

TEST(allocator_example, map) {
    using Arena = ul::mem::alloc::Example;
    using MapPair = std::pair<const int, std::string>;
    using Allocator = ul::mem::Allocator<MapPair, Arena>;
    Arena a;
    const Allocator al{a};
    std::map<int, std::string, std::less<>, Allocator> m{al};

    common_map_test(m);
}

TEST(allocator_default, vector) {
    using Arena = ul::mem::alloc::DefaultNewDelete<>;
    using Allocator = ul::mem::Allocator<int, Arena>;
    Arena a;
    const Allocator al{a};
    std::vector<int, Allocator> v{al};

    common_vector_test(v);
}

TEST(allocator_default, map) {
    using Arena = ul::mem::alloc::DefaultNewDelete<>;
    using MapPair = std::pair<const int, std::string>;
    using Allocator = ul::mem::Allocator<MapPair, Arena>;
    Arena a;
    const Allocator al{a};
    std::map<int, std::string, std::less<>, Allocator> m{al};

    common_map_test(m);
}

TEST(allocator_linear, vector) {
    using Arena = ul::mem::alloc::Linear<>;
    using Allocator = ul::mem::Allocator<int, Arena>;
    try {
        Arena a{
            Bytes{mem100k * sizeof(int) + ul::mem::quirk::vector::constr_heap_alloc_size.value}, Bytes{alignof(int)}};
        const Allocator al{a};
        std::vector<int, Allocator> v{al};

        common_vector_test(v);
    } catch (const std::bad_alloc&) {
        std::cout << "low mem, omitted test";
    }
}

TEST(allocator_linear, map) {
    using Arena = ul::mem::alloc::Linear<>;
    using MapPair = std::pair<const int, std::string>;
    using Allocator = ul::mem::Allocator<MapPair, Arena>;
    std::cout << "sizeof(MapPair): " << sizeof(MapPair) << ", alignof(MapPair): " << alignof(MapPair) << "\n";
    Arena a{Bytes{mem100k}, Bytes{alignof(MapPair)}};
    const Allocator al{a};
    std::map<int, std::string, std::less<>, Allocator> m{al};

    common_map_test(m);
}

TEST(allocator_onstack, vector) {
    using Arena = ul::mem::alloc::OnStack<
        mem100k * sizeof(int) + ul::mem::quirk::vector::constr_heap_alloc_size.value, alignof(int)>;
    using Allocator = ul::mem::Allocator<int, Arena>;
    try {
        Arena a;
        const Allocator al{a};
        std::vector<int, Allocator> v{al};

        common_vector_test(v);
    } catch (const std::bad_alloc&) {
        std::cout << "low mem, omitted test";
    }
}

TEST(allocator_onstack, map) {
    using MapPair = std::pair<const int, std::string>;
    using Arena = ul::mem::alloc::OnStack<mem100k, alignof(MapPair)>;
    using Allocator = ul::mem::Allocator<MapPair, Arena>;
    Arena a;
    const Allocator al{a};
    std::map<int, std::string, std::less<>, Allocator> m{al};

    common_map_test(m);
}
