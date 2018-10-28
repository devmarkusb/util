#include "allocator.h"
#include "alloc/example.h"
#include "alloc/linear.h"
#include <map>
#include <string>
#include <vector>
#include "gtest/gtest.h"


TEST(allocator_example, vector)
{
    std::vector<int, too::mem::Allocator<int, too::mem::alloc::Example>> v;
    v.reserve(1'000'000);
    EXPECT_TRUE(v.empty());
    v.resize(500'000);
    EXPECT_EQ(v.size(), 500'000);
    v.resize(1'000'000);
    EXPECT_EQ(v.size(), 1'000'000);
    v[500'001] = 42;
    EXPECT_EQ(v[500'001], 42);
    v.pop_back();
    EXPECT_EQ(v.size(), 999'999);
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

TEST(allocator_example, map)
{
    using MapPair = std::pair<const int, std::string>;
    using AllocStrategy = too::mem::alloc::Example;
    using Allocator = too::mem::Allocator<MapPair, AllocStrategy>;
    std::map<int, std::string, std::less<>, Allocator> m;
    EXPECT_TRUE(m.empty());
    m[0] = "0";
    EXPECT_EQ(m.size(), 1);
    for (int i = 1; i <= 999; ++i)
    {
        m[i] = std::to_string(i);
    }
    EXPECT_EQ(m.size(), 1'000);
}

//TEST(allocator_linear, vector)
//{
//    using AllocStrategy = too::mem::alloc::Linear;
//    using Allocator = too::mem::Allocator<int, AllocStrategy>;
//    AllocStrategy as{1'000'000};
//    Allocator a{as};
//
//    std::vector<int, Allocator> v{a};
//    v.reserve(1'000'000);
//    EXPECT_TRUE(v.empty());
//    v.resize(500'000);
//    EXPECT_EQ(v.size(), 500'000);
//    v.resize(1'000'000);
//    EXPECT_EQ(v.size(), 1'000'000);
//    v[500'001] = 42;
//    EXPECT_EQ(v[500'001], 42);
//    v.pop_back();
//    EXPECT_EQ(v.size(), 999'999);
//    v.clear();
//    EXPECT_TRUE(v.empty());
//    v.shrink_to_fit();
//    // implementation dependent according to doc., so can't test anything
//    //EXPECT_EQ(v.capacity(), 0);
//    v.push_back(42);
//    EXPECT_EQ(v.size(), 1);
//    EXPECT_GE(v.capacity(), 1);
//    for (int i = 1; i <= 999; ++i)
//    {
//        v.push_back(42);
//    }
//    std::cout << "capacity: " << v.capacity() << "\n";
//    EXPECT_EQ(v.size(), 1000);
//}
//
//TEST(allocator_linear, map)
//{
//    std::map<int, std::string, std::less<>,
//            too::mem::Allocator<std::pair<const int, std::string>, too::mem::alloc::Example>> m;
//    EXPECT_TRUE(m.empty());
//    m[0] = "0";
//    EXPECT_EQ(m.size(), 1);
//    for (int i = 1; i <= 999; ++i)
//    {
//        m[i] = std::to_string(i);
//    }
//    EXPECT_EQ(m.size(), 1'000);
//}
