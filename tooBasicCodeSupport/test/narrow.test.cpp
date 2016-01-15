#include "gtest/gtest.h"
#include "Toolib/narrow.h"


TEST(NarrowTest, Cast)
{
    EXPECT_TRUE(static_cast<long>(too::narrow_cast<unsigned char>(42l)) == 42l);
}

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
