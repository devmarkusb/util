#include "ul/comp_bwds.h"
#include "gtest/gtest.h"
#include <string>

TEST(CompilerBackwardsTest, StringConversions) {
    // Test string conversion functions if available
#ifdef UL_HAS_STOI
    EXPECT_EQ(42, std::stoi("42"));
#endif
#ifdef UL_HAS_STOLD
    EXPECT_EQ(42.0L, std::stold("42.0"));
#endif
#ifdef UL_HAS_TO_STRING
    EXPECT_EQ("42", std::to_string(42));
#endif
}

TEST(CompilerBackwardsTest, MathFunctions) {
    // Test math functions if available
#ifdef UL_HAS_LLROUND
    EXPECT_EQ(42, std::llround(42.3));
    EXPECT_EQ(43, std::llround(42.7));
    EXPECT_EQ(-42, std::llround(-42.3));
    EXPECT_EQ(-43, std::llround(-42.7));
#endif
}
