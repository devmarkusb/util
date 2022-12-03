#include "ul/string/locale.h"
#include "ul/string/ulstring.h"
#include "gtest/gtest.h"
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <locale>
#include <string>


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
} // namespace

struct char_encoding_fileTest : public ::testing::Test
{
    char_encoding_fileTest()
    {
    }

    void SetUp()
    {
        FilePathNameExt = "i_can_be_deleted__temp_txt_file_";
        FilePathNameExt += this->counter++;
        FilePathNameExt += ".txt";
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
        ASSERT_EQ(0, ret);
    }

    ~char_encoding_fileTest()
    {
    }

private:
    std::string FilePathNameExt;
    std::string content;
    static char counter;
};

char char_encoding_fileTest::counter{'a'};

TEST_F(char_encoding_fileTest, ASCII)
{
    const std::string s = "!!AA~~";
    write_file(s);
    EXPECT_TRUE(is_valid_utf8(s));
    const std::string read = read_file();
    EXPECT_EQ(read, s);
}

TEST_F(char_encoding_fileTest, ASCIIhex)
{
    const std::string s = "\x21\x21"
                          "AA~\x7e";
    EXPECT_TRUE(s == "!!AA~~");
    EXPECT_TRUE(is_valid_utf8(s));
    write_file(s);
    const std::string read = read_file();
    EXPECT_EQ(read, s);
}

TEST_F(char_encoding_fileTest, UTF8)
{
    const std::string s = "\xc3\xa4"
                          "hnlich";
    EXPECT_TRUE(is_valid_utf8(s));
    write_file(s);
    const std::string read = read_file();
    EXPECT_EQ(read, s);
}

TEST_F(char_encoding_fileTest, UTF8_cyrillic)
{
    const std::string s = "schtscha:\xd0\xa9";
    EXPECT_TRUE(is_valid_utf8(s));
    write_file(s);
    const std::string read = read_file();
    EXPECT_EQ(read, s);
}

TEST_F(char_encoding_fileTest, latin1)
{
    const std::string s = "\xe4\xf6\xfc"; // latin1 (aeoeue)
    EXPECT_FALSE(is_valid_utf8(s));
    write_file(s);
    const std::string read = read_file();
    EXPECT_EQ(read, s);
}

TEST(consoleTest, utf8_to_utf16)
{
    const std::string s = "\xc3\xa4"
                          "hnlich";
    std::cout << s << "\n";

    std::u16string utf16to;
    utf8::utf8to16(s.begin(), s.end(), std::back_inserter(utf16to));

    std::wstring ws(utf16to.begin(), utf16to.end());

    // Cf. locale.test.cpp #locale_facet-exception for explanation.
#if !(UL_COMP_MINGW && UL_COMP_MINGW_VER <= 50300)
    ul::set_global_locale_scoped loc{ul::Global_locale::user_preferred};
#endif
    std::wcout << ws << L"\n";
    std::cout << s << "\n";
}
