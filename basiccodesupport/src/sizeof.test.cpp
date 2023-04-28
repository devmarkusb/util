#include "ul/sizeof.h"
#include "gtest/gtest.h"
#include <cstdint>

namespace {
struct Empty {};

struct S {
    int8_t a[13]; // NOLINT
};
} // namespace

// uncomment for experimentation
//UL_CT_SIZEOF(Empty)
//UL_CT_SIZEOF(S)
//UL_CT_SIZEOF(int)

TEST(ct_sizeof, basics) {
    // uncomment for experimentation
    //UL_CT_SIZEOF_OPT2(S);
}
