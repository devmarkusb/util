#include "ul/ptr.h"
#include "ul/ignore_arg.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(ownerTest, Test)
{
    // just a compile check
    ul::owner<int*> po = new int;
    int* non_owning = po;
    ul::ignore_arg(non_owning);
    delete po;
}

namespace
{
int* id_with_not_null_param(ul::not_null<int*> param)
{
    return param;
}
} // namespace

TEST(not_nullTest, Test)
{
    int x = 2;
    int* px = &x;
    EXPECT_EQ(px, id_with_not_null_param(px));
    int* nullp = nullptr;
    EXPECT_THROW(id_with_not_null_param(nullp), ul::fail_fast);
}
