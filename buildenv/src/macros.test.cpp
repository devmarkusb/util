#include "ul/macros.h"
#include "gtest/gtest.h"

TEST(UL_DEBUGTest, test)
{
#ifdef NDEBUG
    EXPECT_EQ(0, UL_DEBUG);
#else
    EXPECT_EQ(1, UL_DEBUG);
#endif
}

TEST(UL_DUMMYTest, test)
{
    // clang-format off
    const int UL_DUMMY = 1; const int x = UL_DUMMY; // in the same line, UL_DUMMY is still defined...
    const int UL_DUMMY = 2; const int y = UL_DUMMY; // a different line, other unique UL_DUMMY
    // clang-format on
    // int x = UL_DUMMY; // ... but here, no longer defined (doesn't compile, which is ok)
    EXPECT_EQ(1, x);
    EXPECT_EQ(2, y);
#if UL_DUMMY != UL_ANONYMOUS_IDENTIFIER
#error "UL_DUMMY and UL_ANONYMOUS_IDENTIFIER should be defined identically."
#endif
}

TEST(UL_STRINGIFYTest, plain)
{
    const std::string s = UL_STRINGIFY(this is a test string);
    EXPECT_EQ("this is a test string", s);
    const std::string s2 = UL_STRINGIFY("this is a test string"); // not the proper usage
    EXPECT_EQ("\"this is a test string\"", s2);
}

TEST(UL_STRINGIFYTest, macro)
{
#define SOMETHING 42
    const std::string s = UL_STRINGIFY(SOMETHING);
    EXPECT_EQ("SOMETHING", s);
    const std::string s2 = UL_STRINGIFY_VALUE(SOMETHING);
    EXPECT_EQ("42", s2);
#undef SOMETHING
}

namespace ppdefs::test::impl::treat_this_as_anonymous_namespace
{
UL_DEPRECATED void deprecated_function()
{
}
} // namespace ppdefs::test::impl::treat_this_as_anonymous_namespace

using ppdefs::test::impl::treat_this_as_anonymous_namespace::deprecated_function;

TEST(UL_DEPRECATEDTest, test)
{
    //uncomment for experimentation
    //deprecated_function();
}
