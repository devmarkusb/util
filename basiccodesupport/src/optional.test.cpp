#include "ul/optional.h"
#include "ul/warnings.h"
#include "gtest/gtest.h"
#include <string>
#include <vector>

namespace ul = mb::ul;

TEST(optTest, empty_constr)
{
    const ul::opt<bool> b;
    if (b)
        EXPECT_TRUE(false);
    else
        EXPECT_TRUE(true);
    const ul::opt<bool> cb;
    if (cb)
        EXPECT_TRUE(false);
    else
        EXPECT_TRUE(true);
    const ul::opt<const bool> bc;
    if (bc)
        EXPECT_TRUE(false);
    else
        EXPECT_TRUE(true);
}

TEST(optTest, none_constr)
{
    const ul::opt<bool> b{ul::none};
    if (b)
        EXPECT_TRUE(false);
    else
        EXPECT_TRUE(true);
    const ul::opt<bool> cb{ul::none};
    if (cb)
        EXPECT_TRUE(false);
    else
        EXPECT_TRUE(true);
    const ul::opt<const bool> bc{ul::none};
    if (bc)
        EXPECT_TRUE(false);
    else
        EXPECT_TRUE(true);
}

// VLD detects 3 memory leaks (unclear)
TEST(optTest, not_)
{
    ul::opt<bool> b;
    if (!b)
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
    // EXPECT_TRUE(!b);
    // EXPECT_FALSE(b);
    EXPECT_EQ(ul::none, b);
#if GTEST_HAS_DEATH_TEST
    UL_PRAGMA_WARNINGS_PUSH
    // clang-format off
    UL_WARNING_DISABLE_CLANG(used-but-marked-unused)
    // clang-format on
    EXPECT_DEATH(
        [&]()
        {
            *b = false;
        }(),
        "");
    UL_PRAGMA_WARNINGS_POP
#endif
}

TEST(optTest, constr_assign_moves)
{
    ul::opt<bool> b(true);
    if (b)
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
    EXPECT_TRUE(*b);
    b = false;
    EXPECT_FALSE(*b);
    b = ul::none;
    EXPECT_FALSE(b);
    b = false;
    if (b)
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
    EXPECT_FALSE(*b);

    ul::opt<bool> copy(b);
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

    b = ul::opt<bool>(false);
    if (b)
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
    EXPECT_TRUE(!*b);

    const bool boo = true;
    const ul::opt<bool> c(boo);
    if (c)
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
    EXPECT_TRUE(*c);
}

namespace
{
void f_with_opt_string_arg(const ul::opt<std::string>& /*unused*/)
{
}
} // namespace

TEST(optTest, types)
{
    const ul::opt<double> d = 5.2;
    if (d)
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
    EXPECT_DOUBLE_EQ(5.2, *d);

    const ul::opt<std::string> s("Hi!");
    if (s)
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
    EXPECT_EQ("Hi!", *s);
    f_with_opt_string_arg(std::string("Hi!"));

    const ul::opt<std::vector<int>> v({1, 2, 3});
    if (v)
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
    const std::vector<int> vtest{1, 2, 3};
    EXPECT_EQ(vtest, *v);

    const ul::opt<double*> pd = nullptr;
    if (pd)
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
    EXPECT_EQ(nullptr, *pd);
}

TEST(optTest, comparisons)
{
    // untested
    //    ul::opt<int> a = 1;
    //    ul::opt<int> b = 1;
    //    EXPECT_EQ(a, b);
}
