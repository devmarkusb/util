#include "toolib/sizeof.h"
#include "gtest/gtest.h"
#include <cstdint>


namespace
{
struct Empty
{
};

struct S
{
    int8_t a[13];
};
} // namespace

// uncomment for experimentation
//TOO_CT_SIZEOF(Empty)
//TOO_CT_SIZEOF(S)
//TOO_CT_SIZEOF(int)

TEST(ct_sizeof, basics)
{
    // uncomment for experimentation
    //TOO_CT_SIZEOF_OPT2(S);
}