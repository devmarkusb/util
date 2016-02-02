#include "gtest/gtest.h"
#include "Toolib/ptr.h"
#include "Toolib/ignore_arg.h"


TEST(ownerTest, Test)
{
    // just a compile check
    too::owner<int*> po = new int;
    int* non_owning = po;
    too::ignore_arg(non_owning);
    delete po;
}

namespace
{
int* id_with_not_null_param(too::not_null<int*> param) { return param; }
}

TEST(not_nullTest, Test)
{
    int x   = 2;
    int* px = &x;
    EXPECT_EQ(px, id_with_not_null_param(px));
    int* nullp = nullptr;
    EXPECT_DEBUG_DEATH(id_with_not_null_param(nullp), "");
}
