#include "gtest/gtest.h"
#include <string>
#include <fstream>
#include <iostream>
#include <locale>
#include "utf8.h"


namespace
{
void SaveToTextFile(const std::string& FilePathNameExt, const std::string& Content)
{
    std::ofstream file(FilePathNameExt);
    if (!file)
        throw std::runtime_error("file open");
    file << Content;
}

void LoadFromTextFile(const std::string& FilePathNameExt, std::string& Content)
{
    std::ifstream file(FilePathNameExt);
    if (!file)
        throw std::runtime_error("file open");
    file.seekg(0, std::ios::end);
    if (!file)
        throw std::runtime_error("file seek");
    const auto size = file.tellg();
    if (!file || size == static_cast<decltype(size)>(-1))
        throw std::runtime_error("get file size");
    Content.resize(static_cast<size_t>(size)); // need the precise size for the string, I guess
    file.seekg(0);
    if (!file)
        throw std::runtime_error("file seek");
    file.read(&Content[0], size);
}
}

struct char_encoding_fileTest : public ::testing::Test
{
    char_encoding_fileTest()
    {
    }

    void SetUp()
    {
        FilePathNameExt = "i_can_be_deleted__temp_txt_file.txt";
    }

    void write_file(const std::string& text)
    {
        this->content = text;
        SaveToTextFile(FilePathNameExt, text);
    }

    std::string read_file()
    {
        std::string ret;
        LoadFromTextFile(FilePathNameExt, ret);
        return ret;
    }

    bool is_valid_utf8(const std::string& s)
    {
        return utf8::is_valid(s.begin(), s.end());
    }

    void TearDown()
    {
        int ret = std::remove(FilePathNameExt.c_str());
        ASSERT_TRUE(ret == 0);
    }

    ~char_encoding_fileTest()
    {
    }

private:
    std::string FilePathNameExt;
    std::string content;
};

TEST_F(char_encoding_fileTest, ASCII)
{
    const std::string s = "!!AA~~";
    write_file(s);
    EXPECT_TRUE(is_valid_utf8(s));
    const std::string read = read_file();
    EXPECT_TRUE(read == s);
}

TEST_F(char_encoding_fileTest, ASCIIhex)
{
    const std::string s = "\x21\x21""AA~\x7e";
    EXPECT_TRUE(s == "!!AA~~");
    EXPECT_TRUE(is_valid_utf8(s));
    write_file(s);
    const std::string read = read_file();
    EXPECT_TRUE(read == s);
}

TEST_F(char_encoding_fileTest, UTF8)
{
    const std::string s = "\xc3\xa4""hnlich";
    EXPECT_TRUE(is_valid_utf8(s));
    //const std::string locs = "ähnlich";
    //std::cout << locs << "\n";
    //std::locale::global(std::locale(""));
    //std::cout << locs << "\n";
    write_file(s);
    const std::string read = read_file();
    EXPECT_TRUE(read == s);
}

TEST_F(char_encoding_fileTest, UTF8_cyrillic)
{
    const std::string s = "schtscha:\xd0\xa9";
    EXPECT_TRUE(is_valid_utf8(s));
    write_file(s);
    const std::string read = read_file();
    EXPECT_TRUE(read == s);
}

TEST_F(char_encoding_fileTest, latin1)
{
    const std::string s = "äöü";
    EXPECT_FALSE(is_valid_utf8(s));
    write_file(s);
    const std::string read = read_file();
    EXPECT_TRUE(read == s);
}
