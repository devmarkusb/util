#include "ul/type_name.h"
#include "ul/ignore_arg.h"
#include "gtest/gtest.h"
#include <tuple>

#include "ul/macros.h"

namespace ul = mb::ul;

TEST(type_nameTest, basic)
{
    struct SomeTypeName
    {
    };
#if !TOO_COMP_GNU_CPP
    EXPECT_STREQ(ul::type_name<SomeTypeName>().data(), "() [T = SomeTypeName]");
#endif
}

TEST(type_nameTest, special)
{
    auto whatType = std::make_tuple("bla", 42, 1.33);
#if !TOO_COMP_GNU_CPP
    EXPECT_STREQ(ul::type_name<decltype(whatType)>().data(), "() [T = std::tuple<const char *, int, double>]");
#else
    ul::ignore_arg(whatType);
#endif
}

TEST(TOO_COMPILETIME_TYPE_NAMETest, special)
{
    auto whatType = std::make_tuple("bla", 42, 1.33);
    ul::ignore_arg(whatType);
    // uncomment for experimentation
    //TOO_COMPILETIME_TYPE_NAME(whatType);
}
