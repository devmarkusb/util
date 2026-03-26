#include "mb/ul/string/ulstring.hpp" // NOLINT
#include "gtest/gtest.h"
#include <string>

namespace ul = mb::ul;

TEST(ulStringTest, get_length_ascii) {
    // Test with ASCII strings
    EXPECT_EQ(ul::str::get_length(""), 0);
    EXPECT_EQ(ul::str::get_length("a"), 1);
    EXPECT_EQ(ul::str::get_length("abc"), 3);
    EXPECT_EQ(ul::str::get_length("Hello, World!"), 13);
}

TEST(ulStringTest, get_length_utf8) {
    // Test with UTF-8 strings
    // German "ähnlich" (similar)
    EXPECT_EQ(ul::str::get_length("\xc3\xa4hnlich"), 7);
    // Japanese "こんにちは" (hello)
    EXPECT_EQ(ul::str::get_length("\xe3\x81\x93\xe3\x82\x93\xe3\x81\xab\xe3\x81\xa1\xe3\x81\xaf"), 5);
    // Chinese "你好" (hello)
    EXPECT_EQ(ul::str::get_length("\xe4\xbd\xa0\xe5\xa5\xbd"), 2);
    // Korean "안녕하세요" (hello)
    EXPECT_EQ(ul::str::get_length("\xec\x95\x88\xeb\x85\x95\xed\x95\x98\xec\x84\xb8\xec\x9a\x94"), 5);
}

TEST(ulStringTest, get_length_mixed) {
    // Test with mixed ASCII and UTF-8 strings
    EXPECT_EQ(ul::str::get_length("Hello \xc3\xa4hnlich"), 13);
    EXPECT_EQ(ul::str::get_length("Hello こんにちは"), 11);
    EXPECT_EQ(ul::str::get_length("Hello 你好"), 8);
    EXPECT_EQ(ul::str::get_length("Hello 안녕하세요"), 11);
}

TEST(ulStringTest, get_length_special) {
    // Test with special characters
    EXPECT_EQ(ul::str::get_length("\n\t\r"), 3); // Control characters
    EXPECT_EQ(ul::str::get_length(" "), 1); // Space
    EXPECT_EQ(ul::str::get_length("  "), 2); // Multiple spaces
    EXPECT_EQ(ul::str::get_length("!@#$%^&*()"), 10); // Special ASCII characters
}

TEST(ulStringTest, get_length_emoji) {
    // Test with emoji (which are UTF-8 characters)
    EXPECT_EQ(ul::str::get_length("😀"), 1); // Basic emoji
    EXPECT_EQ(ul::str::get_length("👋🌍"), 2); // Multiple emoji
    EXPECT_EQ(ul::str::get_length("Hello 😀"), 7); // Mixed with ASCII
}

TEST(ulStringTest, get_length_edge_cases) {
    // Test edge cases
    std::string empty;
    EXPECT_EQ(ul::str::get_length(empty), 0);

    // Test with string containing only null bytes
    std::string nulls(3, '\0');
    EXPECT_EQ(ul::str::get_length(nulls), 3);

    // Test with string containing invalid UTF-8
    std::string invalid_utf8 = "\xc3"; // Incomplete UTF-8 sequence
    EXPECT_ANY_THROW(ul::str::get_length(invalid_utf8));

    // Test with very long string
    std::string long_string(1000, 'a');
    EXPECT_EQ(ul::str::get_length(long_string), 1000);
}
