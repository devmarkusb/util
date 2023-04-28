// VLD detects lots of memory leaks for this file (unclear)
#include "ul/xor.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(xorTest, xor_bools) {
    bool a{false};
    bool b{false};
    EXPECT_FALSE(ul::XOR(a, b));

    a = true;
    b = true;
    EXPECT_FALSE(ul::XOR(a, b));

    a = false;
    b = true;
    EXPECT_TRUE(ul::XOR(a, b));

    a = true;
    b = false;
    EXPECT_TRUE(ul::XOR(a, b));
}

TEST(xorTest, xnor_bools) {
    bool a{false};
    bool b{false};
    EXPECT_TRUE(ul::XNOR(a, b));

    a = true;
    b = true;
    EXPECT_TRUE(ul::XNOR(a, b));

    a = false;
    b = true;
    EXPECT_FALSE(ul::XNOR(a, b));

    a = true;
    b = false;
    EXPECT_FALSE(ul::XNOR(a, b));
}

TEST(xorTest, xor_nonbool) {
    int a{0};
    int b{0};
    int* pa{nullptr};
    int* pb{nullptr};
    EXPECT_FALSE(ul::XOR(pa, pb));
    pa = &a;
    EXPECT_TRUE(ul::XOR(pa, pb));
    pb = &b;
    EXPECT_FALSE(ul::XOR(pa, pb)); // not equal, but both non-nullptr, so equivalent in the desired sense
    pb = &a;
    EXPECT_FALSE(ul::XOR(pa, pb));
    pb = &b;
    pa = nullptr;
    EXPECT_TRUE(ul::XOR(pa, pb));

    EXPECT_FALSE(ul::XOR(a, b));
    a = -1;
    EXPECT_TRUE(ul::XOR(a, b));
    b = -1;
    EXPECT_FALSE(ul::XOR(a, b));
    a = 0;
    EXPECT_TRUE(ul::XOR(a, b));
    a = 1;
    EXPECT_FALSE(ul::XOR(a, b)); // both non-0
    b = 0;
    EXPECT_TRUE(ul::XOR(a, b));
    b = 1;
    EXPECT_FALSE(ul::XOR(a, b));
    a = 0;
    EXPECT_TRUE(ul::XOR(a, b));
    a = -1;
    EXPECT_FALSE(ul::XOR(a, b)); // both non-0
}

TEST(xorTest, nxor_nonbool) {
    int a{0};
    int b{0};
    int* pa{nullptr};
    int* pb{nullptr};
    EXPECT_TRUE(ul::XNOR(pa, pb));
    pa = &a;
    EXPECT_FALSE(ul::XNOR(pa, pb));
    pb = &b;
    EXPECT_TRUE(ul::XNOR(pa, pb)); // not equal, but both non-nullptr, so equivalent in the desired sense
    pb = &a;
    EXPECT_TRUE(ul::XNOR(pa, pb));
    pb = &b;
    pa = nullptr;
    EXPECT_FALSE(ul::XNOR(pa, pb));

    EXPECT_TRUE(ul::XNOR(a, b));
    a = -1;
    EXPECT_FALSE(ul::XNOR(a, b));
    b = -1;
    EXPECT_TRUE(ul::XNOR(a, b));
    a = 0;
    EXPECT_FALSE(ul::XNOR(a, b));
    a = 1;
    EXPECT_TRUE(ul::XNOR(a, b)); // both non-0
    b = 0;
    EXPECT_FALSE(ul::XNOR(a, b));
    b = 1;
    EXPECT_TRUE(ul::XNOR(a, b));
    a = 0;
    EXPECT_FALSE(ul::XNOR(a, b));
    a = -1;
    EXPECT_TRUE(ul::XNOR(a, b)); // both non-0
}

TEST(xorTest, xor_mixedtypes) {
    const int a{0};
    EXPECT_FALSE(ul::XOR(0, nullptr));
    EXPECT_FALSE(ul::XOR(nullptr, 0));
    EXPECT_FALSE(ul::XOR(1, &a));
    EXPECT_FALSE(ul::XOR(&a, 1));
    EXPECT_FALSE(ul::XOR(-1, &a));
    EXPECT_FALSE(ul::XOR(&a, -1));

    EXPECT_TRUE(ul::XOR(0, &a));
    EXPECT_TRUE(ul::XOR(&a, 0));
    EXPECT_TRUE(ul::XOR(1, nullptr));
    EXPECT_TRUE(ul::XOR(nullptr, 1));
    EXPECT_TRUE(ul::XOR(-1, nullptr));
    EXPECT_TRUE(ul::XOR(nullptr, -1));
}
