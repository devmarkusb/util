#include "gtest/gtest.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "utf8.h"


TEST(utf8FileReadTest, correct_utf8_file)
{
    const char* test_file_path = "utf8.test.txt";
    std::ifstream fs8(test_file_path);
    ASSERT_TRUE(fs8.is_open());

    unsigned line_count = 1;
    std::string line;
	//char* start_it = fs8.rdbuf();
	//EXPECT_FALSE(utf8::is_bom(start_it));
    // Play with all the lines in the file
    while (std::getline(fs8, line)) {
       // check for invalid utf-8 (for a simple yes/no check, there is also utf8::is_valid function)
        std::string::iterator end_it = utf8::find_invalid(line.begin(), line.end());
        const bool valid_utf8 = end_it == line.end();
        EXPECT_TRUE(valid_utf8);
        if (!valid_utf8) {
            std::cout << "Invalid UTF-8 encoding detected at line " << line_count << "\n";
            std::cout << "This part is fine: " << std::string(line.begin(), end_it) << "\n";
        }

        // Get the line length (at least for the valid part)
        int length = utf8::distance(line.begin(), end_it);
        std::cout << "Length of line " << line_count << " is " << length <<  "\n";

        // Convert it to utf-16
        std::vector<unsigned short> utf16line;
        utf8::utf8to16(line.begin(), end_it, std::back_inserter(utf16line));

        // And back to utf-8
        std::string utf8line;
        utf8::utf16to8(utf16line.begin(), utf16line.end(), back_inserter(utf8line));

        // Confirm that the conversion went OK:
        const bool conv_ok = utf8line == std::string(line.begin(), end_it);
        EXPECT_TRUE(conv_ok);
        if (!conv_ok)
            std::cout << "Error in UTF-16 conversion at line: " << line_count << "\n";

        line_count++;
    }
}

TEST(utf8FileReadTest, correct_utf8bom_file)
{
    const char* test_file_path = "utf8withBOM.test.txt";
    std::ifstream fs8(test_file_path);
    ASSERT_TRUE(fs8.is_open());

    unsigned line_count = 1;
    std::string line;
    // Play with all the lines in the file
    while (std::getline(fs8, line)) {
       // check for invalid utf-8 (for a simple yes/no check, there is also utf8::is_valid function)
        std::string::iterator end_it = utf8::find_invalid(line.begin(), line.end());
        const bool valid_utf8 = end_it == line.end();
        EXPECT_TRUE(valid_utf8);
        if (!valid_utf8) {
            std::cout << "Invalid UTF-8 encoding detected at line " << line_count << "\n";
            std::cout << "This part is fine: " << std::string(line.begin(), end_it) << "\n";
        }

        // Get the line length (at least for the valid part)
        int length = utf8::distance(line.begin(), end_it);
        std::cout << "Length of line " << line_count << " is " << length <<  "\n";

        // Convert it to utf-16
        std::vector<unsigned short> utf16line;
        utf8::utf8to16(line.begin(), end_it, std::back_inserter(utf16line));

        // And back to utf-8
        std::string utf8line;
        utf8::utf16to8(utf16line.begin(), utf16line.end(), back_inserter(utf8line));

        // Confirm that the conversion went OK:
        const bool conv_ok = utf8line == std::string(line.begin(), end_it);
        EXPECT_TRUE(conv_ok);
        if (!conv_ok)
            std::cout << "Error in UTF-16 conversion at line: " << line_count << "\n";

        line_count++;
    }
}

TEST(utf8FileReadTest, ansi_file)
{
    const char* test_file_path = "latin1.test.txt";
    std::ifstream fs8(test_file_path);
    ASSERT_TRUE(fs8.is_open());

    unsigned line_count = 1;
    std::string line;
    // Play with all the lines in the file
    while (std::getline(fs8, line)) {
       // check for invalid utf-8 (for a simple yes/no check, there is also utf8::is_valid function)
        std::string::iterator end_it = utf8::find_invalid(line.begin(), line.end());
        const bool valid_utf8 = end_it == line.end();
        EXPECT_TRUE(valid_utf8);
        if (!valid_utf8) {
            std::cout << "Invalid UTF-8 encoding detected at line " << line_count << "\n";
            std::cout << "This part is fine: " << std::string(line.begin(), end_it) << "\n";
        }

        // Get the line length (at least for the valid part)
        int length = utf8::distance(line.begin(), end_it);
        std::cout << "Length of line " << line_count << " is " << length <<  "\n";

        // Convert it to utf-16
        std::vector<unsigned short> utf16line;
        utf8::utf8to16(line.begin(), end_it, std::back_inserter(utf16line));

        // And back to utf-8
        std::string utf8line;
        utf8::utf16to8(utf16line.begin(), utf16line.end(), back_inserter(utf8line));

        // Confirm that the conversion went OK:
        const bool conv_ok = utf8line == std::string(line.begin(), end_it);
        EXPECT_TRUE(conv_ok);
        if (!conv_ok)
            std::cout << "Error in UTF-16 conversion at line: " << line_count << "\n";

        line_count++;
    }
}
