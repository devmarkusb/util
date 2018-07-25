#include "Toolib/types.h"
#include "gtest/gtest.h"


TEST(tooTypesDefinitionTest, correctness)
{
    EXPECT_TRUE(typeid(too::uchar) == typeid(unsigned char));
    EXPECT_TRUE(typeid(too::byte) == typeid(unsigned char));
    EXPECT_TRUE(typeid(too::schar) == typeid(signed char));
    EXPECT_TRUE(typeid(too::ushort) == typeid(unsigned short));
    EXPECT_TRUE(typeid(too::sshort) == typeid(signed short));
    EXPECT_TRUE(typeid(too::uint) == typeid(unsigned int));
    EXPECT_TRUE(typeid(too::sint) == typeid(signed int));
    EXPECT_TRUE(typeid(too::ulong) == typeid(unsigned long));
    EXPECT_TRUE(typeid(too::slong) == typeid(signed long));
}
