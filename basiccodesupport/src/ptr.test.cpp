#include "ul/ptr.h"
#include "ul/assert.h"
#include "ul/ignore_unused.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(ownerTest, Test) {
    // just a compile check
    ul::Owner<int*> po = new int; // NOLINT
    int* non_owning = po;
    ul::ignore_unused(non_owning);
    delete po;
}

namespace {
int* id_with_not_null_param(ul::NotNull<int*> param) {
    return param;
}
} // namespace

TEST(not_nullTest, Test) {
    int x = 2;
    int* px = &x;
    EXPECT_EQ(px, id_with_not_null_param(px));
    int* nullp = nullptr;
    EXPECT_THROW(id_with_not_null_param(nullp), ul::FailFast);
}
