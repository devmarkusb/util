#include "toolib/format_number.h"
#include "gtest/gtest.h"


TEST(fmt_groupThousands, basics)
{
    EXPECT_STREQ(too::fmt::groupThousands(1000000, '.').c_str(), "1.000.000");
    EXPECT_STREQ(too::fmt::groupThousands(1'000'000, '.').c_str(), "1.000.000");
    EXPECT_STREQ(too::fmt::groupThousands(100'000, '.').c_str(), "100.000");
    EXPECT_STREQ(too::fmt::groupThousands(10'000, '.').c_str(), "10.000");
    EXPECT_STREQ(too::fmt::groupThousands(1'000, '.').c_str(), "1.000");
    EXPECT_STREQ(too::fmt::groupThousands(100, '.').c_str(), "100");
    EXPECT_STREQ(too::fmt::groupThousands(1, '.').c_str(), "1");

    EXPECT_STREQ(too::fmt::groupThousands(1'000'000, '\'').c_str(), "1\'000\'000");
    EXPECT_STREQ(too::fmt::groupThousands(1'000'000, ',').c_str(), "1,000,000");
}
