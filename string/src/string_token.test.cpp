#include "ul/string/string_token.h" // NOLINT
#include "gtest/gtest.h"
#include <string>
#include <vector>

namespace ul = mb::ul;

TEST(string_token, basic) {
    std::string input = "Hello World";
    std::string delimiters = " ";
    std::vector<std::string> tokens;
    
    ul::str::tokenize_string(input, delimiters, tokens);
    
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ("Hello", tokens[0]);
    EXPECT_EQ("World", tokens[1]);
}

TEST(string_token, multiple_delimiters) {
    std::string input = "Hello,World;Test";
    std::string delimiters = ",;";
    std::vector<std::string> tokens;
    
    ul::str::tokenize_string(input, delimiters, tokens);
    
    EXPECT_EQ(3, tokens.size());
    EXPECT_EQ("Hello", tokens[0]);
    EXPECT_EQ("World", tokens[1]);
    EXPECT_EQ("Test", tokens[2]);
}

TEST(string_token, empty_string) {
    std::string input = "";
    std::string delimiters = " ";
    std::vector<std::string> tokens;
    
    ul::str::tokenize_string(input, delimiters, tokens);
    
    EXPECT_EQ(0, tokens.size());
}

TEST(string_token, string_with_only_delimiters) {
    std::string input = "   ";
    std::string delimiters = " ";
    std::vector<std::string> tokens;
    
    ul::str::tokenize_string(input, delimiters, tokens);
    
    EXPECT_EQ(0, tokens.size());
}

TEST(string_token, string_with_leading_delimiters) {
    std::string input = "  Hello World";
    std::string delimiters = " ";
    std::vector<std::string> tokens;
    
    ul::str::tokenize_string(input, delimiters, tokens);
    
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ("Hello", tokens[0]);
    EXPECT_EQ("World", tokens[1]);
}

TEST(string_token, string_with_trailing_delimiters) {
    std::string input = "Hello World  ";
    std::string delimiters = " ";
    std::vector<std::string> tokens;
    
    ul::str::tokenize_string(input, delimiters, tokens);
    
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ("Hello", tokens[0]);
    EXPECT_EQ("World", tokens[1]);
}

TEST(string_token, string_with_consecutive_delimiters) {
    std::string input = "Hello  World";
    std::string delimiters = " ";
    std::vector<std::string> tokens;
    
    ul::str::tokenize_string(input, delimiters, tokens);
    
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ("Hello", tokens[0]);
    EXPECT_EQ("World", tokens[1]);
}

TEST(string_token, empty_delimiters) {
    std::string input = "Hello World";
    std::string delimiters = "";
    std::vector<std::string> tokens;
    
    ul::str::tokenize_string(input, delimiters, tokens);
    
    // With empty delimiters, the entire string should be treated as a single token
    EXPECT_EQ(1, tokens.size());
    EXPECT_EQ("Hello World", tokens[0]);
}

TEST(string_token, reuse_vector) {
    std::string input1 = "Hello World";
    std::string input2 = "Test,Case";
    std::string delimiters1 = " ";
    std::string delimiters2 = ",";
    std::vector<std::string> tokens;
    
    ul::str::tokenize_string(input1, delimiters1, tokens);
    
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ("Hello", tokens[0]);
    EXPECT_EQ("World", tokens[1]);

    tokens.clear();
    
    // Reuse the same vector
    ul::str::tokenize_string(input2, delimiters2, tokens);
    
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ("Test", tokens[0]);
    EXPECT_EQ("Case", tokens[1]);
}

TEST(string_token, special_characters) {
    std::string input = "Hello\nWorld\tTest";
    std::string delimiters = "\n\t";
    std::vector<std::string> tokens;
    
    ul::str::tokenize_string(input, delimiters, tokens);
    
    EXPECT_EQ(3, tokens.size());
    EXPECT_EQ("Hello", tokens[0]);
    EXPECT_EQ("World", tokens[1]);
    EXPECT_EQ("Test", tokens[2]);
}

TEST(string_token, utf8_characters) {
    std::string input = "Hello\xc3\xa4World";
    std::string delimiters = "\xc3\xa4";
    std::vector<std::string> tokens;
    
    ul::str::tokenize_string(input, delimiters, tokens);
    
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ("Hello", tokens[0]);
    EXPECT_EQ("World", tokens[1]);
}
