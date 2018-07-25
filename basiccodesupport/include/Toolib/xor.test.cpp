// VLD detects lots of memory leaks for this file (unclear)
#include "Toolib/xor.h"
#include "gtest/gtest.h"


TEST(xorTest, xor_bools)
{
    bool a{false};
    bool b{false};
    EXPECT_FALSE(too::XOR(a, b));

    a = true;
    b = true;
    EXPECT_FALSE(too::XOR(a, b));

    a = false;
    b = true;
    EXPECT_TRUE(too::XOR(a, b));

    a = true;
    b = false;
    EXPECT_TRUE(too::XOR(a, b));
}

TEST(xorTest, xnor_bools)
{
    bool a{false};
    bool b{false};
    EXPECT_TRUE(too::XNOR(a, b));

    a = true;
    b = true;
    EXPECT_TRUE(too::XNOR(a, b));

    a = false;
    b = true;
    EXPECT_FALSE(too::XNOR(a, b));

    a = true;
    b = false;
    EXPECT_FALSE(too::XNOR(a, b));
}

TEST(xorTest, xor_nonbool)
{
    int a{0};
    int b{0};
    int* pa{nullptr};
    int* pb{nullptr};
    EXPECT_FALSE(too::XOR(pa, pb));
    pa = &a;
    EXPECT_TRUE(too::XOR(pa, pb));
    pb = &b;
    EXPECT_FALSE(too::XOR(pa, pb)); // not equal, but both non-nullptr, so equivalent in the desired sense
    pb = &a;
    EXPECT_FALSE(too::XOR(pa, pb));
    pb = &b;
    pa = nullptr;
    EXPECT_TRUE(too::XOR(pa, pb));

    EXPECT_FALSE(too::XOR(a, b));
    a = -1;
    EXPECT_TRUE(too::XOR(a, b));
    b = -1;
    EXPECT_FALSE(too::XOR(a, b));
    a = 0;
    EXPECT_TRUE(too::XOR(a, b));
    a = 1;
    EXPECT_FALSE(too::XOR(a, b)); // both non-0
    b = 0;
    EXPECT_TRUE(too::XOR(a, b));
    b = 1;
    EXPECT_FALSE(too::XOR(a, b));
    a = 0;
    EXPECT_TRUE(too::XOR(a, b));
    a = -1;
    EXPECT_FALSE(too::XOR(a, b)); // both non-0
}

TEST(xorTest, nxor_nonbool)
{
    int a{0};
    int b{0};
    int* pa{nullptr};
    int* pb{nullptr};
    EXPECT_TRUE(too::XNOR(pa, pb));
    pa = &a;
    EXPECT_FALSE(too::XNOR(pa, pb));
    pb = &b;
    EXPECT_TRUE(too::XNOR(pa, pb)); // not equal, but both non-nullptr, so equivalent in the desired sense
    pb = &a;
    EXPECT_TRUE(too::XNOR(pa, pb));
    pb = &b;
    pa = nullptr;
    EXPECT_FALSE(too::XNOR(pa, pb));

    EXPECT_TRUE(too::XNOR(a, b));
    a = -1;
    EXPECT_FALSE(too::XNOR(a, b));
    b = -1;
    EXPECT_TRUE(too::XNOR(a, b));
    a = 0;
    EXPECT_FALSE(too::XNOR(a, b));
    a = 1;
    EXPECT_TRUE(too::XNOR(a, b)); // both non-0
    b = 0;
    EXPECT_FALSE(too::XNOR(a, b));
    b = 1;
    EXPECT_TRUE(too::XNOR(a, b));
    a = 0;
    EXPECT_FALSE(too::XNOR(a, b));
    a = -1;
    EXPECT_TRUE(too::XNOR(a, b)); // both non-0
}

TEST(xorTest, xor_mixedtypes)
{
    int a{0};
    EXPECT_FALSE(too::XOR(0, nullptr));
    EXPECT_FALSE(too::XOR(nullptr, 0));
    EXPECT_FALSE(too::XOR(1, &a));
    EXPECT_FALSE(too::XOR(&a, 1));
    EXPECT_FALSE(too::XOR(-1, &a));
    EXPECT_FALSE(too::XOR(&a, -1));

    EXPECT_TRUE(too::XOR(0, &a));
    EXPECT_TRUE(too::XOR(&a, 0));
    EXPECT_TRUE(too::XOR(1, nullptr));
    EXPECT_TRUE(too::XOR(nullptr, 1));
    EXPECT_TRUE(too::XOR(-1, nullptr));
    EXPECT_TRUE(too::XOR(nullptr, -1));
}

