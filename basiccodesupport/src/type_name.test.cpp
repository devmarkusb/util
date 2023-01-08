#include "ul/type_name.h"
#include "ul/ignore_unused.h"
#include "ul/macros.h"
#include "gtest/gtest.h"
#include <tuple>

namespace ul = mb::ul;

TEST(type_nameTest, basic)
{
    struct SomeTypeName
    {
    };
#if UL_COMP_CLANG
    EXPECT_STREQ(ul::typeName<SomeTypeName>().data(), ") [T = SomeTypeName]");
#endif
}

TEST(type_nameTest, special)
{
    auto whatType = std::make_tuple("bla", 42, 1.33);
#if UL_COMP_CLANG
    EXPECT_STREQ(ul::typeName<decltype(whatType)>().data(), ") [T = std::tuple<const char *, int, double>]");
#else
    ul::ignore_unused(whatType);
#endif
}

TEST(UL_COMPILETIME_TYPE_NAMETest, special)
{
    auto whatType = std::make_tuple("bla", 42, 1.33);
    ul::ignore_unused(whatType);
    // uncomment for experimentation
    //UL_COMPILETIME_TYPE_NAME(whatType);
}
