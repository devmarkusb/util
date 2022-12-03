#include "ul/macros.h"
#include "gtest/gtest.h"

TEST(TOO_DEBUGTest, test)
{
#ifdef NDEBUG
    EXPECT_EQ(0, TOO_DEBUG);
#else
    EXPECT_EQ(1, TOO_DEBUG);
#endif
}

TEST(TOO_DUMMYTest, test)
{
    // clang-format off
    const int TOO_DUMMY = 1; const int x = TOO_DUMMY; // in the same line, TOO_DUMMY is still defined...
    const int TOO_DUMMY = 2; const int y = TOO_DUMMY; // a different line, other unique TOO_DUMMY
    // clang-format on
    // int x = TOO_DUMMY; // ... but here, no longer defined (doesn't compile, which is ok)
    EXPECT_EQ(1, x);
    EXPECT_EQ(2, y);
#if TOO_DUMMY != TOO_ANONYMOUS_IDENTIFIER
#error "TOO_DUMMY and TOO_ANONYMOUS_IDENTIFIER should be defined identically."
#endif
}

TEST(TOO_STRINGIFYTest, plain)
{
    const std::string s = TOO_STRINGIFY(this is a test string);
    EXPECT_EQ("this is a test string", s);
    const std::string s2 = TOO_STRINGIFY("this is a test string"); // not the proper usage
    EXPECT_EQ("\"this is a test string\"", s2);
}

TEST(TOO_STRINGIFYTest, macro)
{
#define SOMETHING 42
    const std::string s = TOO_STRINGIFY(SOMETHING);
    EXPECT_EQ("SOMETHING", s);
    const std::string s2 = TOO_STRINGIFY_VALUE(SOMETHING);
    EXPECT_EQ("42", s2);
#undef SOMETHING
}

namespace ppdefs::test::impl::treat_this_as_anonymous_namespace
{
TOO_DEPRECATED void deprecated_function()
{
}
} // namespace ppdefs::test::impl::treat_this_as_anonymous_namespace

using ppdefs::test::impl::treat_this_as_anonymous_namespace::deprecated_function;

TEST(TOO_DEPRECATEDTest, test)
{
    //uncomment for experimentation
    //deprecated_function();
}
