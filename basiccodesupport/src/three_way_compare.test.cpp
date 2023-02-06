#include "ul/three_way_compare.h"
#include "gtest/gtest.h"
#include <string>

namespace ul = mb::ul;

TEST(three_way_compare, common_cases)
{
    EXPECT_EQ(ul::threeWayCompare(42, 41), 1);
    EXPECT_EQ(ul::threeWayCompare(42, 42), 0);
    EXPECT_EQ(ul::threeWayCompare(42, 43), -1);
}

TEST(three_way_compare, common_cases_neg)
{
    EXPECT_EQ(ul::threeWayCompare(-42, -41), -1);
    EXPECT_EQ(ul::threeWayCompare(-42, -42), 0);
    EXPECT_EQ(ul::threeWayCompare(-42, -43), 1);
    EXPECT_EQ(ul::threeWayCompare(-2, 1), -1);
    EXPECT_EQ(ul::threeWayCompare(1, -2), 1);
}

TEST(three_way_compare, huge_signed)
{
    EXPECT_EQ(ul::threeWayCompare(std::numeric_limits<intmax_t>::max(), intmax_t(1)), 1);
    EXPECT_EQ(ul::threeWayCompare(intmax_t(1), std::numeric_limits<intmax_t>::max()), -1);

    EXPECT_EQ(ul::threeWayCompare(-std::numeric_limits<intmax_t>::max(), -intmax_t(1)), -1);
    EXPECT_EQ(ul::threeWayCompare(-intmax_t(1), -std::numeric_limits<intmax_t>::max()), 1);

    EXPECT_EQ(ul::threeWayCompare(-std::numeric_limits<intmax_t>::max(), intmax_t(1)), -1);
    EXPECT_EQ(ul::threeWayCompare(intmax_t(1), -std::numeric_limits<intmax_t>::max()), 1);
}

TEST(three_way_compare, huge_unsigned)
{
    EXPECT_EQ(ul::threeWayCompare(std::numeric_limits<uintmax_t>::max(), uintmax_t(1)), 1);
    EXPECT_EQ(ul::threeWayCompare(uintmax_t(1), std::numeric_limits<uintmax_t>::max()), -1);
}

TEST(three_way_compare, strings)
{
    const std::string xb{"xb"};
    const std::string xa{"xa"};
    EXPECT_EQ(ul::threeWayCompare(xb, xa), 1);
    EXPECT_EQ(ul::threeWayCompare(xb, xb), 0);
    EXPECT_EQ(ul::threeWayCompare(xa, xb), -1);
}
