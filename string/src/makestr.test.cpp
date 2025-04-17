#include "ul/string/makestr.h"
#include "gtest/gtest.h"
#include <iomanip>
#include <ios>
#include <string>

namespace ul = mb::ul;

TEST(makestrTest, basic) {
    const std::string s = ul::str::Makestr() << "Hey, here is number one: " << 1 << ", and other stuff: " << 5.4 << 'X'
                                             << "\xc3\xa4 " << std::boolalpha << true;
    EXPECT_EQ("Hey, here is number one: 1, and other stuff: 5.4X\xc3\xa4 true", s);
}

TEST(makestrTest, empty) {
    const std::string s = ul::str::Makestr();
    EXPECT_EQ("", s);
}

TEST(makestrTest, single_value) {
    const std::string s1 = ul::str::Makestr() << "Hello";
    EXPECT_EQ("Hello", s1);
    
    const std::string s2 = ul::str::Makestr() << 42;
    EXPECT_EQ("42", s2);
    
    const std::string s3 = ul::str::Makestr() << 3.14;
    EXPECT_EQ("3.14", s3);
    
    const std::string s4 = ul::str::Makestr() << true;
    EXPECT_EQ("1", s4);
    
    const std::string s5 = ul::str::Makestr() << std::boolalpha << true;
    EXPECT_EQ("true", s5);
}

TEST(makestrTest, multiple_values) {
    const std::string s = ul::str::Makestr() << "Hello" << " " << "World" << " " << 42;
    EXPECT_EQ("Hello World 42", s);
}

TEST(makestrTest, special_characters) {
    const std::string s = ul::str::Makestr() << "Hello" << "\n" << "World" << "\t" << "42";
    EXPECT_EQ("Hello\nWorld\t42", s);
}

TEST(makestrTest, utf8_characters) {
    const std::string s = ul::str::Makestr() << "Hello" << " " << "\xc3\xa4" << " " << "\xe2\x82\xac";
    EXPECT_EQ("Hello \xc3\xa4 \xe2\x82\xac", s);
}

TEST(makestrTest, stream_manipulators) {
    const std::string s1 = ul::str::Makestr() << std::hex << 42;
    EXPECT_EQ("2a", s1);
    
    const std::string s2 = ul::str::Makestr() << std::dec << 42;
    EXPECT_EQ("42", s2);
    
    const std::string s3 = ul::str::Makestr() << std::setw(5) << std::setfill('0') << 42;
    EXPECT_EQ("00042", s3);
    
    const std::string s4 = ul::str::Makestr() << std::fixed << std::setprecision(2) << 3.14159;
    EXPECT_EQ("3.14", s4);
}

TEST(makestrTest, implicit_conversion) {
    std::string s = ul::str::Makestr() << "Hello" << " " << "World";
    EXPECT_EQ("Hello World", s);
    
    // Test that the implicit conversion works
    void test_function(const std::string& s);
    test_function(ul::str::Makestr() << "Hello" << " " << "World");
}

// Helper function for the implicit conversion test
void test_function(const std::string& s) {
    EXPECT_EQ("Hello World", s);
}
