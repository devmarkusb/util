#include "ul/string/string_misc.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(str_apply_ellipse, cornercases)
{
    std::string s;
    const auto ret = ul::str::applyEllipse(s, 0);
    EXPECT_STREQ("", ret.c_str());
    const auto ret2 = ul::str::applyEllipse(s, 1);
    EXPECT_STREQ("", ret2.c_str());

    s = "x";
    const auto ret3 = ul::str::applyEllipse(s, 0);
    EXPECT_STREQ("x", ret3.c_str());
    const auto ret4 = ul::str::applyEllipse(s, 1);
    EXPECT_STREQ("x", ret4.c_str());
}

TEST(str_apply_ellipse, test_german)
{
    const std::string s{"xyz"};
    const auto ret = ul::str::applyEllipse(s, 2);
    EXPECT_STREQ("xy...", ret.c_str());

    const auto ret2 = ul::str::applyEllipse(s, 1);
    EXPECT_STREQ("x...", ret2.c_str());

    const auto ret3 = ul::str::applyEllipse(s, 3);
    EXPECT_STREQ("xyz", ret3.c_str());

    const auto ret4 = ul::str::applyEllipse(s, 4);
    EXPECT_STREQ("xyz", ret4.c_str());
}


TEST(str_apply_ellipse, test_wider_chars)
{
    // U+10346 GOTHIC LETTER FAIHU 4 bytes + U+65E5 3 bytes + U+0448 cyrillic scha 2 bytes.
    // So, a nice example with 4+3+2 bytes but actually only 3 chars.
    const std::string threechars{"\xf0\x90\x8d\x86\xe6\x97\xa5\xd1\x88"};
    const std::string& s{threechars};

    const auto ret = ul::str::applyEllipse(s, 2);
    EXPECT_STREQ("\xf0\x90\x8d\x86\xe6\x97\xa5...", ret.c_str());

    const auto ret2 = ul::str::applyEllipse(s, 1);
    EXPECT_STREQ("\xf0\x90\x8d\x86...", ret2.c_str());

    const auto ret3 = ul::str::applyEllipse(s, 3);
    EXPECT_STREQ("\xf0\x90\x8d\x86\xe6\x97\xa5\xd1\x88", ret3.c_str());

    const auto ret4 = ul::str::applyEllipse(s, 4);
    EXPECT_STREQ("\xf0\x90\x8d\x86\xe6\x97\xa5\xd1\x88", ret4.c_str());
}
