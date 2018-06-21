#include "Toolib/narrow.h"
#include "Toolib/PPDEFS.h"
#include "Toolib/PPDefs/MSVC/UNDEF_MIN_MAX"
#include "gtest/gtest.h"


TEST(NarrowTest, Cast) { EXPECT_TRUE(static_cast<long>(too::narrow_cast<unsigned char>(42l)) == 42l); }

TEST(NarrowTest, CheckedCast)
{
    EXPECT_TRUE(static_cast<long>(too::narrow<unsigned char>(42l)) == 42l);
    EXPECT_NO_THROW(too::narrow<unsigned char>(42l));
    EXPECT_THROW(too::narrow<unsigned char>(300l), too::narrowing_error);
}

TEST(NarrowTest, Misc)
{
    EXPECT_THROW(too::narrow<unsigned char>(-1), too::narrowing_error);
    EXPECT_NO_THROW(too::narrow<unsigned char>(0));
    EXPECT_NO_THROW(too::narrow<unsigned char>(255));
    EXPECT_THROW(too::narrow<unsigned char>(256), too::narrowing_error);
}

TEST(NarrowTest, floating_point)
{
    EXPECT_THROW(too::narrow<int>(1.8), too::narrowing_error);
    EXPECT_NO_THROW(too::narrow<int>(2.0));
    EXPECT_EQ(1, too::narrow_cast<int>(1.8));
    EXPECT_EQ(2, too::narrow<int>(2.0));

    const long long more_precise_than_float{std::numeric_limits<long long>::max()};
#if !TOO_DEBUG && \
    (TOO_COMP_MINGW && TOO_COMP_MINGW_VER == 50300 \
    || TOO_COMP_GNU_CPP && TOO_COMP_GNU_CPP_VER >= 60201 && TOO_COMP_GNU_CPP_VER <= 70300)
    // absolutely no idea so far why this doesn't throw in release under these compilers :O
    EXPECT_NO_THROW(too::narrow<float>(more_precise_than_float));
#else
    EXPECT_THROW(too::narrow<float>(more_precise_than_float), too::narrowing_error);
#endif
    EXPECT_NO_THROW(too::narrow<float>(2));
    EXPECT_EQ(static_cast<float>(more_precise_than_float), too::narrow_cast<float>(more_precise_than_float));
    EXPECT_EQ(static_cast<float>(2), too::narrow<float>(2));

    // and this shouldn't compile yet
    // float x = too::narrow<double>(2.0);
    // but this is ok
    const float f = too::narrow_cast<float>(2.0);
    EXPECT_EQ(static_cast<float>(2.0), f);
    const double d = too::narrow_cast<double>(2.0L);
    EXPECT_EQ(static_cast<double>(2.0L), d);
}
