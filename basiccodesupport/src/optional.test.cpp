#include "toolib/optional.h"
#include "toolib/warnings.h"
#include "gtest/gtest.h"
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

TEST(optTest, none_constr)
{
    too::opt<bool> b{too::none};
    if (b)
        EXPECT_TRUE(false);
    else
        EXPECT_TRUE(true);
    const too::opt<bool> cb{too::none};
    if (cb)
        EXPECT_TRUE(false);
    else
        EXPECT_TRUE(true);
    too::opt<const bool> bc{too::none};
    if (bc)
        EXPECT_TRUE(false);
    else
        EXPECT_TRUE(true);
}

// VLD detects 3 memory leaks (unclear)
TEST(optTest, not_)
{
    too::opt<bool> b;
    if (!b)
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
    // EXPECT_TRUE(!b);
    // EXPECT_FALSE(b);
    EXPECT_EQ(too::none, b);
#if GTEST_HAS_DEATH_TEST
    TOO_PRAGMA_WARNINGS_PUSH
    // clang-format off
    TOO_WARNING_DISABLE_CLANG(used-but-marked-unused)
    // clang-format on
    EXPECT_DEATH(
        [&]() {
            *b = false;
        }(),
        "");
    TOO_PRAGMA_WARNINGS_POP
#endif
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
    if (b)
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
    EXPECT_FALSE(*b);

    too::opt<bool> copy(b);
    if (copy)
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
    EXPECT_FALSE(*copy);

    b = true;
    copy = b;
    if (copy)
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
    EXPECT_TRUE(*copy);

    b = too::opt<bool>(false);
    if (b)
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
    EXPECT_TRUE(!*b);

    bool boo = true;
    too::opt<bool> c(boo);
    if (c)
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
    EXPECT_TRUE(*c);
}

namespace
{
void f_with_opt_string_arg(const too::opt<std::string>&)
{
}
} // namespace

TEST(optTest, types)
{
    too::opt<double> d = 5.2;
    if (d)
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
    EXPECT_DOUBLE_EQ(5.2, *d);

    too::opt<std::string> s("Hi!");
    if (s)
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
    EXPECT_EQ("Hi!", *s);
    f_with_opt_string_arg(std::string("Hi!"));

    too::opt<std::vector<int>> v({1, 2, 3});
    if (v)
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
    std::vector<int> vtest{1, 2, 3};
    EXPECT_EQ(vtest, *v);

    too::opt<double*> pd = nullptr;
    if (pd)
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
    EXPECT_EQ(nullptr, *pd);
}

TEST(optTest, comparisons)
{
    // untested
    //    too::opt<int> a = 1;
    //    too::opt<int> b = 1;
    //    EXPECT_EQ(a, b);
}