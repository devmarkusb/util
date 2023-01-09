#include "ul/string/lex_cast.h"
#include "gtest/gtest.h"

TEST(lex_cast, int2str)
{
    EXPECT_EQ("42", ul::lex_cast<std::string>(42));
}

TEST(lex_cast, str2int)
{
    EXPECT_EQ(42, ul::lex_cast<int>("42"));
}

TEST(lex_cast, float2int)
{
    EXPECT_EQ(42, ul::lex_cast<int>(42.0f));
}
