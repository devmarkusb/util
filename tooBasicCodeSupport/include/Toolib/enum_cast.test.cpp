#include "Toolib/enum_cast.h"
#include "gtest/gtest.h"


TEST(EnumCast_as_numberTest, test)
{
    enum class EC
    {
        zeroth,
        first,
        second,
    };
    auto number = too::as_number(EC::first);
    EXPECT_EQ(1, number);
    enum class E
    {
        zeroth,
        first,
        second,
    };
    auto number_ = too::as_number(E::first);
    EXPECT_EQ(1, number_);
}

TEST(EnumCast_as_numberTest, TypeSpecific)
{
    enum class EC : unsigned long
    {
        zeroth,
        first,
        second,
    };
    auto number = too::as_number(EC::first);
    EXPECT_EQ(1ul, number);
    EXPECT_TRUE(typeid(number) == typeid(unsigned long));
}
