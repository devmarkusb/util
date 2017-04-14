// VLD detects lots of memory leaks for this file (unclear)
#include "Toolib/xor.h"
#include "gtest/gtest.h"


TEST(xorTest, xor)
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

TEST(xorTest, xnor)
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
    int* b{nullptr};
    EXPECT_FALSE(too::XOR(a, b));

    a = -1;
    b = &a;
    EXPECT_FALSE(too::XOR(a, b));

    a = 0;
    b = &a;
    EXPECT_TRUE(too::XOR(a, b));

    a = -1;
    b = nullptr;
    EXPECT_TRUE(too::XOR(a, b));
}
