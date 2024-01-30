#include "ul/three_way_compare.h"
#include "gtest/gtest.h"
#include <cstdint>
#include <limits>
#include <string>

namespace ul = mb::ul;

TEST(three_way_compare, common_cases) {
    EXPECT_EQ(ul::three_way_compare(42, 41), 1);
    EXPECT_EQ(ul::three_way_compare(42, 42), 0);
    EXPECT_EQ(ul::three_way_compare(42, 43), -1);
}

TEST(three_way_compare, common_cases_neg) {
    EXPECT_EQ(ul::three_way_compare(-42, -41), -1);
    EXPECT_EQ(ul::three_way_compare(-42, -42), 0);
    EXPECT_EQ(ul::three_way_compare(-42, -43), 1);
    EXPECT_EQ(ul::three_way_compare(-2, 1), -1);
    EXPECT_EQ(ul::three_way_compare(1, -2), 1);
}

TEST(three_way_compare, huge_signed) {
    EXPECT_EQ(ul::three_way_compare(std::numeric_limits<intmax_t>::max(), intmax_t(1)), 1);
    EXPECT_EQ(ul::three_way_compare(intmax_t(1), std::numeric_limits<intmax_t>::max()), -1);

    EXPECT_EQ(ul::three_way_compare(-std::numeric_limits<intmax_t>::max(), -intmax_t(1)), -1);
    EXPECT_EQ(ul::three_way_compare(-intmax_t(1), -std::numeric_limits<intmax_t>::max()), 1);

    EXPECT_EQ(ul::three_way_compare(-std::numeric_limits<intmax_t>::max(), intmax_t(1)), -1);
    EXPECT_EQ(ul::three_way_compare(intmax_t(1), -std::numeric_limits<intmax_t>::max()), 1);
}

TEST(three_way_compare, huge_unsigned) {
    EXPECT_EQ(ul::three_way_compare(std::numeric_limits<uintmax_t>::max(), uintmax_t(1)), 1);
    EXPECT_EQ(ul::three_way_compare(uintmax_t(1), std::numeric_limits<uintmax_t>::max()), -1);
}

TEST(three_way_compare, strings) {
    const std::string xb{"xb"};
    const std::string xa{"xa"};
    EXPECT_EQ(ul::three_way_compare(xb, xa), 1);
    EXPECT_EQ(ul::three_way_compare(xb, xb), 0);
    EXPECT_EQ(ul::three_way_compare(xa, xb), -1);
}
