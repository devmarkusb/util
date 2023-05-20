#include "ul/type_name.h"
#include "ul/ignore_unused.h"
#include "ul/macros.h"
#include "gtest/gtest.h"
#include <tuple>

namespace ul = mb::ul;

TEST(type_nameTest, DISABLED_basic) {
    struct SomeTypeName {};
#if UL_COMP_CLANG
    EXPECT_STREQ(ul::type_name<SomeTypeName>().data(), ") [T = SomeTypeName]");
#endif
}

TEST(type_nameTest, DISABLED_special) {
    auto what_type = std::make_tuple("bla", 42, 1.33); // NOLINT
#if UL_COMP_CLANG
    EXPECT_STREQ(ul::type_name<decltype(what_type)>().data(), ") [T = std::tuple<const char *, int, double>]");
#else
    ul::ignore_unused(whatType);
#endif
}

TEST(UL_COMPILETIME_TYPE_NAMETest, special) {
    [[maybe_unused]] auto what_type = std::make_tuple("bla", 42, 1.33); // NOLINT
    // uncomment for experimentation
    //UL_COMPILETIME_TYPE_NAME(what_type);
}
