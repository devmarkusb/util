#include "type_name.h"
#include "ignore_arg.h"
#include <tuple>
#include "gtest/gtest.h"


TEST(type_nameTest, basic)
{
    struct SomeTypeName
    {
    };

    EXPECT_STREQ(too::type_name<SomeTypeName>().data(), "[T = SomeTypeName]");
}

TEST(type_nameTest, special)
{
    auto whatType = std::make_tuple("bla", 42, 1.33);
    EXPECT_STREQ(too::type_name<decltype(whatType)>().data(), "[T = std::tuple<const char *, int, double>]");
}

TEST(TOO_COMPILETIME_TYPE_NAMETest, special)
{
    auto whatType = std::make_tuple("bla", 42, 1.33);
    too::ignore_arg(whatType);
    // uncomment for experimentation
    //TOO_COMPILETIME_TYPE_NAME(whatType);
}
