// VLD detects lots of memory leaks for this file (unclear)
#include "ul/xor.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(xorTest, xor_bools) {
    bool a{false};
    bool b{false};
    EXPECT_FALSE(ul::xor_bool(a, b));

    a = true;
    b = true;
    EXPECT_FALSE(ul::xor_bool(a, b));

    a = false;
    b = true;
    EXPECT_TRUE(ul::xor_bool(a, b));

    a = true;
    b = false;
    EXPECT_TRUE(ul::xor_bool(a, b));
}

TEST(xorTest, xnor_bools) {
    bool a{false};
    bool b{false};
    EXPECT_TRUE(ul::xnor(a, b));

    a = true;
    b = true;
    EXPECT_TRUE(ul::xnor(a, b));

    a = false;
    b = true;
    EXPECT_FALSE(ul::xnor(a, b));

    a = true;
    b = false;
    EXPECT_FALSE(ul::xnor(a, b));
}

TEST(xorTest, xor_nonbool) {
    int a{0};
    int b{0};
    int* pa{nullptr};
    int* pb{nullptr};
    EXPECT_FALSE(ul::xor_bool(pa, pb));
    pa = &a;
    EXPECT_TRUE(ul::xor_bool(pa, pb));
    pb = &b;
    EXPECT_FALSE(ul::xor_bool(pa, pb)); // not equal, but both non-nullptr, so equivalent in the desired sense
    pb = &a;
    EXPECT_FALSE(ul::xor_bool(pa, pb));
    pb = &b;
    pa = nullptr;
    EXPECT_TRUE(ul::xor_bool(pa, pb));

    EXPECT_FALSE(ul::xor_bool(a, b));
    a = -1;
    EXPECT_TRUE(ul::xor_bool(a, b));
    b = -1;
    EXPECT_FALSE(ul::xor_bool(a, b));
    a = 0;
    EXPECT_TRUE(ul::xor_bool(a, b));
    a = 1;
    EXPECT_FALSE(ul::xor_bool(a, b)); // both non-0
    b = 0;
    EXPECT_TRUE(ul::xor_bool(a, b));
    b = 1;
    EXPECT_FALSE(ul::xor_bool(a, b));
    a = 0;
    EXPECT_TRUE(ul::xor_bool(a, b));
    a = -1;
    EXPECT_FALSE(ul::xor_bool(a, b)); // both non-0
}

TEST(xorTest, nxor_nonbool) {
    int a{0};
    int b{0};
    int* pa{nullptr};
    int* pb{nullptr};
    EXPECT_TRUE(ul::xnor(pa, pb));
    pa = &a;
    EXPECT_FALSE(ul::xnor(pa, pb));
    pb = &b;
    EXPECT_TRUE(ul::xnor(pa, pb)); // not equal, but both non-nullptr, so equivalent in the desired sense
    pb = &a;
    EXPECT_TRUE(ul::xnor(pa, pb));
    pb = &b;
    pa = nullptr;
    EXPECT_FALSE(ul::xnor(pa, pb));

    EXPECT_TRUE(ul::xnor(a, b));
    a = -1;
    EXPECT_FALSE(ul::xnor(a, b));
    b = -1;
    EXPECT_TRUE(ul::xnor(a, b));
    a = 0;
    EXPECT_FALSE(ul::xnor(a, b));
    a = 1;
    EXPECT_TRUE(ul::xnor(a, b)); // both non-0
    b = 0;
    EXPECT_FALSE(ul::xnor(a, b));
    b = 1;
    EXPECT_TRUE(ul::xnor(a, b));
    a = 0;
    EXPECT_FALSE(ul::xnor(a, b));
    a = -1;
    EXPECT_TRUE(ul::xnor(a, b)); // both non-0
}

TEST(xorTest, xor_mixedtypes) {
    const int a{0};
    EXPECT_FALSE(ul::xor_bool(0, nullptr));
    EXPECT_FALSE(ul::xor_bool(nullptr, 0));
    EXPECT_FALSE(ul::xor_bool(1, &a));
    EXPECT_FALSE(ul::xor_bool(&a, 1));
    EXPECT_FALSE(ul::xor_bool(-1, &a));
    EXPECT_FALSE(ul::xor_bool(&a, -1));

    EXPECT_TRUE(ul::xor_bool(0, &a));
    EXPECT_TRUE(ul::xor_bool(&a, 0));
    EXPECT_TRUE(ul::xor_bool(1, nullptr));
    EXPECT_TRUE(ul::xor_bool(nullptr, 1));
    EXPECT_TRUE(ul::xor_bool(-1, nullptr));
    EXPECT_TRUE(ul::xor_bool(nullptr, -1));
}
