#include "ul/string/string_misc.h"
#include "gtest/gtest.h"
#include <string>
#include <vector>

namespace ul = mb::ul;

TEST(str_apply_ellipse, cornercases) {
    std::string s;
    const auto ret = ul::str::apply_ellipse(s, 0);
    EXPECT_STREQ("", ret.c_str());
    const auto ret2 = ul::str::apply_ellipse(s, 1);
    EXPECT_STREQ("", ret2.c_str());

    s = "x";
    const auto ret3 = ul::str::apply_ellipse(s, 0);
    EXPECT_STREQ("x", ret3.c_str());
    const auto ret4 = ul::str::apply_ellipse(s, 1);
    EXPECT_STREQ("x", ret4.c_str());
}

TEST(str_apply_ellipse, test_german) {
    const std::string s{"xyz"};
    const auto ret = ul::str::apply_ellipse(s, 2);
    EXPECT_STREQ("xy...", ret.c_str());

    const auto ret2 = ul::str::apply_ellipse(s, 1);
    EXPECT_STREQ("x...", ret2.c_str());

    const auto ret3 = ul::str::apply_ellipse(s, 3);
    EXPECT_STREQ("xyz", ret3.c_str());

    const auto ret4 = ul::str::apply_ellipse(s, 4);
    EXPECT_STREQ("xyz", ret4.c_str());
}

TEST(str_apply_ellipse, test_wider_chars) {
    // U+10346 GOTHIC LETTER FAIHU 4 bytes + U+65E5 3 bytes + U+0448 cyrillic scha 2 bytes.
    // So, a nice example with 4+3+2 bytes but actually only 3 chars.
    const std::string threechars{"\xf0\x90\x8d\x86\xe6\x97\xa5\xd1\x88"};
    const std::string& s{threechars};

    const auto ret = ul::str::apply_ellipse(s, 2);
    EXPECT_STREQ("\xf0\x90\x8d\x86\xe6\x97\xa5...", ret.c_str());

    const auto ret2 = ul::str::apply_ellipse(s, 1);
    EXPECT_STREQ("\xf0\x90\x8d\x86...", ret2.c_str());

    const auto ret3 = ul::str::apply_ellipse(s, 3);
    EXPECT_STREQ("\xf0\x90\x8d\x86\xe6\x97\xa5\xd1\x88", ret3.c_str());

    const auto ret4 = ul::str::apply_ellipse(s, 4);
    EXPECT_STREQ("\xf0\x90\x8d\x86\xe6\x97\xa5\xd1\x88", ret4.c_str());
}

TEST(str_make_upper, basic) {
    std::string s = "hello";
    EXPECT_EQ(ul::str::make_upper(s), "HELLO");
}

TEST(str_make_upper, special_chars) {
    std::string s = "hello!@#$%^&*()";  // Only ASCII special characters
    EXPECT_EQ(ul::str::make_upper(s), "HELLO!@#$%^&*()");
}

TEST(str_make_lower, basic) {
    std::string s = "HELLO";
    EXPECT_EQ(ul::str::make_lower(s), "hello");
}

TEST(str_make_lower, special_chars) {
    std::string s = "HELLO!@#$%^&*()";  // Only ASCII special characters
    EXPECT_EQ(ul::str::make_lower(s), "hello!@#$%^&*()");
}

TEST(str_replace_all, basic) {
    std::string s = "hello world";
    ul::str::replace_all(s, "world", "there");
    EXPECT_EQ(s, "hello there");
    
    s = "hello hello hello";
    ul::str::replace_all(s, "hello", "hi");
    EXPECT_EQ(s, "hi hi hi");
}

TEST(str_replace_all, empty_strings) {
    std::string s = "";
    ul::str::replace_all(s, "hello", "hi");  // Empty source string is OK
    EXPECT_EQ(s, "");
    
    // Note: Empty from_sub is not supported by design
}

TEST(str_replace_all, with_vector) {
    std::string s = "hello {1} world {2}";
    std::vector<std::string> replacements = {"beautiful", "today"};
    ul::str::replace_all(s, "{1}", replacements);
    EXPECT_EQ(s, "hello beautiful world {2}");
    
    s = "hello {} world {}";
    ul::str::replace_all(s, "{}", replacements);
    EXPECT_EQ(s, "hello beautiful world today");
}

TEST(str_bool_conversion, bool2str) {
    EXPECT_EQ(ul::str::bool2str(true), "1");
    EXPECT_EQ(ul::str::bool2str(false), "0");
    
    EXPECT_EQ(ul::str::bool2str<ul::str::BoolStrBoolalpha>(true), "true");
    EXPECT_EQ(ul::str::bool2str<ul::str::BoolStrBoolalpha>(false), "false");
}

TEST(str_bool_conversion, str2bool) {
    EXPECT_TRUE(ul::str::str2bool("1"));
    EXPECT_FALSE(ul::str::str2bool("0"));
    
    EXPECT_TRUE(ul::str::str2bool<ul::str::BoolStrBoolalpha>("true"));
    EXPECT_FALSE(ul::str::str2bool<ul::str::BoolStrBoolalpha>("false"));
}

TEST(str_ends_with, basic) {
    EXPECT_TRUE(ul::str::ends_with("hello world", "world"));
    EXPECT_FALSE(ul::str::ends_with("hello world", "hello"));
    EXPECT_TRUE(ul::str::ends_with("hello", "hello"));
}

TEST(str_ends_with, empty_strings) {
    EXPECT_TRUE(ul::str::ends_with("", ""));
    EXPECT_FALSE(ul::str::ends_with("", "hello"));
    EXPECT_TRUE(ul::str::ends_with("hello", ""));
}

TEST(str_ends_with, special_chars) {
    EXPECT_TRUE(ul::str::ends_with("hello\xc3\xa4", "\xc3\xa4"));  // UTF-8 character
    EXPECT_TRUE(ul::str::ends_with("hello.txt", ".txt"));
    EXPECT_TRUE(ul::str::ends_with("hello\n", "\n"));
}
