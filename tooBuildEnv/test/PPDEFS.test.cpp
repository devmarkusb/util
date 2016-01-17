#include "gtest/gtest.h"
#include "Toolib/PPDEFS.h"

TEST(TOO_DEBUGTest, test)
{
#ifdef NDEBUG
    EXPECT_EQ(0, TOO_DEBUG);
#else
    EXPECT_EQ(1, TOO_DEBUG);
#endif
}

TEST(TOO_NONAMETest, test)
{
    // clang-format off
    const int TOO_NONAME = 1; const int x = TOO_NONAME; // in the same line, TOO_NONAME is still defined...
    // clang-format on
    // int x = TOO_NONAME; // ... but here, no longer defined (doesn't compile, which is ok)
    EXPECT_EQ(1, x);
#if TOO_NONAME != TOO_ANONYMOUS_VARIABLE
#error "TOO_NONAME and TOO_ANONYMOUS_VARIABLE should be defined identically."
#endif
}

TEST(TOO_STRINGIFYTest, test)
{
    std::string s = TOO_STRINGIFY(this is a test string);
    EXPECT_EQ("this is a test string", s);
    std::string s2 = TOO_STRINGIFY("this is a test string"); // not the proper usage
    EXPECT_EQ("\"this is a test string\"", s2);
}
