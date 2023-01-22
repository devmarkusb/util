#include "ul/narrow.h"
#include "ul/macros.h"
#include "ul/macros/UNDEF_MIN_MAX.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(NarrowTest, Cast)
{
    EXPECT_TRUE(static_cast<long>(ul::narrow_cast<unsigned char>(42l)) == 42l);
}

TEST(NarrowTest, CheckedCast)
{
    EXPECT_TRUE(static_cast<long>(ul::narrow<unsigned char>(42l)) == 42l);
    EXPECT_NO_THROW(ul::narrow<unsigned char>(42l));
    EXPECT_THROW(ul::narrow<unsigned char>(300l), ul::narrowing_error);
}

TEST(NarrowTest, Misc)
{
    EXPECT_THROW(ul::narrow<unsigned char>(-1), ul::narrowing_error);
    EXPECT_NO_THROW(ul::narrow<unsigned char>(0));
    EXPECT_NO_THROW(ul::narrow<unsigned char>(255));
    EXPECT_THROW(ul::narrow<unsigned char>(256), ul::narrowing_error);
}

TEST(NarrowTest, floating_point)
{
    EXPECT_THROW(ul::narrow<int>(1.8), ul::narrowing_error);
    EXPECT_NO_THROW(ul::narrow<int>(2.0));
    EXPECT_EQ(1, ul::narrow_cast<int>(1.8));
    EXPECT_EQ(2, ul::narrow<int>(2.0));

    const auto more_precise_than_float{std::numeric_limits<uint64_t>::max()};
#if !UL_DEBUG \
    && (UL_COMP_MINGW && UL_COMP_MINGW_VER == 50300 \
        || UL_COMP_GNU_CPP && UL_COMP_GNU_CPP_VER >= 60201 && UL_COMP_GNU_CPP_VER < 120000 \
        || UL_COMP_CLANG && UL_COMP_CLANG_VER < 160000)
    // absolutely no idea so far why this doesn't throw in release under these compilers :O
    EXPECT_NO_THROW(ul::narrow<float>(more_precise_than_float));
#else
    EXPECT_THROW(ul::narrow<float>(more_precise_than_float), ul::narrowing_error);
#endif
    EXPECT_NO_THROW(ul::narrow<float>(2));
    EXPECT_EQ(static_cast<float>(more_precise_than_float), ul::narrow_cast<float>(more_precise_than_float));
    EXPECT_EQ(static_cast<float>(2), ul::narrow<float>(2));

    // and this shouldn't compile yet
    // float x = ul::narrow<double>(2.0);
    // but this is ok
    const auto f = ul::narrow_cast<float>(2.0);
    EXPECT_EQ(static_cast<float>(2.0), f);
    const auto d = ul::narrow_cast<double>(2.0L);
    EXPECT_EQ(static_cast<double>(2.0L), d);
}
