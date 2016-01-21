#include "gtest/gtest.h"
#include "Toolib/optional.h"
#include <string>
#include <vector>


TEST(optTest, empty_constr)
{
    too::opt<bool> b;
    if (b)
        EXPECT_TRUE(false);
    else
        EXPECT_TRUE(true);
    const too::opt<bool> cb;
    if (cb)
        EXPECT_TRUE(false);
    else
        EXPECT_TRUE(true);
    too::opt<const bool> bc;
    if (bc)
        EXPECT_TRUE(false);
    else
        EXPECT_TRUE(true);
}

TEST(optTest, not)
{
    too::opt<bool> b;
    if (!b)
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
    EXPECT_TRUE(!b);
    EXPECT_FALSE(b);
    EXPECT_EQ(too::none, b);
    EXPECT_DEATH([&](){ *b = false; }(), "");
}

TEST(optTest, constr_assign_moves)
{
    too::opt<bool> b(true);
    if (b)
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
    EXPECT_TRUE(*b);
    b = false;
    EXPECT_FALSE(*b);
    b = too::none;
    EXPECT_FALSE(b);
    b = false;
    EXPECT_TRUE(b);
    EXPECT_FALSE(*b);

    too::opt<bool> copy(b);
    EXPECT_TRUE(copy);
    EXPECT_FALSE(*copy);

    b = true;
    copy = b;
    EXPECT_TRUE(copy);
    EXPECT_TRUE(*copy);

    b = too::opt<bool>(false);
    EXPECT_TRUE(b);
    EXPECT_TRUE(!*b);

    bool boo = true;
    too::opt<bool> c(boo);
    EXPECT_TRUE(c);
    EXPECT_TRUE(*c);
}

namespace
{
void f_with_opt_string_arg(const too::opt<std::string>&)
{
}
}

TEST(optTest, types)
{
    too::opt<double> d = 5.2;
    EXPECT_TRUE(d);
    EXPECT_DOUBLE_EQ(5.2, *d);

    too::opt<std::string> s("Hi!");
    EXPECT_TRUE(s);
    EXPECT_EQ("Hi!", *s);
    f_with_opt_string_arg(std::string("Hi!"));

    too::opt<std::vector<int>> v({1,2,3});
    EXPECT_TRUE(v);
    std::vector<int> vtest{1,2,3};
    EXPECT_EQ(vtest, *v);

    too::opt<double*> pd = nullptr;
    EXPECT_TRUE(pd);
    EXPECT_EQ(nullptr, *pd);
}

TEST(optTest, comparisons)
{
    //todo
//    too::opt<int> a = 1;
//    too::opt<int> b = 1;
//    EXPECT_EQ(a, b);
}
