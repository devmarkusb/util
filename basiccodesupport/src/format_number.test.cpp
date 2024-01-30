#include "ul/format_number.h" // NOLINT
#include "ul/macros.h"
#include "gtest/gtest.h"

#if UL_HAS_NOCRASH_IMBUE_LOCALE
namespace ul = mb::ul;
#endif

TEST(fmt_groupThousands, basics) {
#if UL_HAS_NOCRASH_IMBUE_LOCALE
    EXPECT_STREQ(ul::fmt::group_thousands(1000000, '.').c_str(), "1.000.000");
    EXPECT_STREQ(ul::fmt::group_thousands(1'000'000, '.').c_str(), "1.000.000");
    EXPECT_STREQ(ul::fmt::group_thousands(100'000, '.').c_str(), "100.000");
    EXPECT_STREQ(ul::fmt::group_thousands(10'000, '.').c_str(), "10.000");
    EXPECT_STREQ(ul::fmt::group_thousands(1'000, '.').c_str(), "1.000");
    EXPECT_STREQ(ul::fmt::group_thousands(100, '.').c_str(), "100");
    EXPECT_STREQ(ul::fmt::group_thousands(1, '.').c_str(), "1");

    EXPECT_STREQ(ul::fmt::group_thousands(1'000'000, '\'').c_str(), "1\'000\'000");
    EXPECT_STREQ(ul::fmt::group_thousands(1'000'000, ',').c_str(), "1,000,000");
#endif
}
