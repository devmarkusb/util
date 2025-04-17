#include "ul/string/string_trim.h" // NOLINT
#include "gtest/gtest.h"
#include <string>

namespace ul = mb::ul;

TEST(StringTrimTest, ltrim_basic) {
    std::string s = "  hello  ";
    EXPECT_EQ(ul::str::ltrim(s), "hello  ");
    
    s = "\t\n\r hello";
    EXPECT_EQ(ul::str::ltrim(s), "hello");
    
    s = "hello";
    EXPECT_EQ(ul::str::ltrim(s), "hello");
}

TEST(StringTrimTest, ltrim_custom_chars) {
    std::string s = "xxxhello";
    EXPECT_EQ(ul::str::ltrim(s, "x"), "hello");
    
    s = ".-hello";
    EXPECT_EQ(ul::str::ltrim(s, ".-"), "hello");
    
    s = "abcabchello";
    EXPECT_EQ(ul::str::ltrim(s, "abc"), "hello");
}

TEST(StringTrimTest, rtrim_basic) {
    std::string s = "  hello  ";
    EXPECT_EQ(ul::str::rtrim(s), "  hello");
    
    s = "hello\t\n\r";
    EXPECT_EQ(ul::str::rtrim(s), "hello");
    
    s = "hello";
    EXPECT_EQ(ul::str::rtrim(s), "hello");
}

TEST(StringTrimTest, rtrim_custom_chars) {
    std::string s = "helloxxx";
    EXPECT_EQ(ul::str::rtrim(s, "x"), "hello");
    
    s = "hello.-";
    EXPECT_EQ(ul::str::rtrim(s, ".-"), "hello");
    
    s = "helloabcabc";
    EXPECT_EQ(ul::str::rtrim(s, "abc"), "hello");
}

TEST(StringTrimTest, trim_basic) {
    std::string s = "  hello  ";
    EXPECT_EQ(ul::str::trim(s), "hello");
    
    s = "\t\n\r hello\t\n\r";
    EXPECT_EQ(ul::str::trim(s), "hello");
    
    s = "hello";
    EXPECT_EQ(ul::str::trim(s), "hello");
}

TEST(StringTrimTest, trim_empty) {
    std::string s = "";
    EXPECT_EQ(ul::str::trim(s), "");
    
    s = "   ";
    EXPECT_EQ(ul::str::trim(s), "");
    
    s = "\t\n\r";
    EXPECT_EQ(ul::str::trim(s), "");
}

TEST(StringTrimTest, trim_special_chars) {
    std::string s = "\0hello\0";
    EXPECT_EQ(ul::str::trim(s), "\0hello\0");
    
    s = "  hello\xc3\xa4  ";  // UTF-8 character
    EXPECT_EQ(ul::str::trim(s), "hello\xc3\xa4");
}

TEST(StringTrimTest, trim_all_spaces) {
    std::string s = "   ";
    EXPECT_EQ(ul::str::trim(s), "");
    
    s = "\t\n\r \t\n\r";
    EXPECT_EQ(ul::str::trim(s), "");
    
    s = " \t\n\r \t\n\r ";
    EXPECT_EQ(ul::str::trim(s), "");
}

TEST(StringTrimTest, trim_mixed) {
    std::string s = "  hello\xc3\xa4  ";  // UTF-8 character with spaces
    EXPECT_EQ(ul::str::trim(s), "hello\xc3\xa4");
    
    s = "\t\n\r hello\xc3\xa4\t\n\r";  // UTF-8 character with control chars
    EXPECT_EQ(ul::str::trim(s), "hello\xc3\xa4");
    
    s = "  hello\xc3\xa4\t\n\r  ";  // UTF-8 character with mixed whitespace
    EXPECT_EQ(ul::str::trim(s), "hello\xc3\xa4");
}
