#include "ul/enum_cast.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(EnumCast_as_numberTest, test) {
    enum class EC {
        zeroth,
        first,
        second,
    };
    auto number = ul::as_number(EC::first);
    EXPECT_EQ(1, number);
    enum class E {
        zeroth,
        first,
        second,
    };
    number = ul::as_number(E::first);
    EXPECT_EQ(1, number);
}

TEST(EnumCast_as_numberTest, TypeSpecific) {
    enum class EC : uint64_t {
        zeroth,
        first,
        second,
    };
    auto number = ul::as_number(EC::first);
    EXPECT_EQ(1ul, number);
    EXPECT_TRUE(typeid(number) == typeid(uint64_t));
}
