#include "ul/string/locale.h"
#include "ul/string/ulstring.h"
#include "gtest/gtest.h"
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <locale>
#include <string>

namespace ul = mb::ul;

namespace {
void save_to_text_file(const std::string& file_path_name_ext, const std::string& content) {
    std::ofstream file(file_path_name_ext);
    if (!file)
        throw std::runtime_error("file open");
    file << content;
}

void load_from_text_file(const std::string& file_path_name_ext, std::string& content) {
    std::ifstream file(file_path_name_ext);
    if (!file)
        throw std::runtime_error("file open");
    file.seekg(0, std::ios::end);
    if (!file)
        throw std::runtime_error("file seek");
    const auto size = file.tellg();
    if (!file || size == static_cast<decltype(size)>(-1))
        throw std::runtime_error("get file size");
    content.resize(static_cast<size_t>(size)); // need the precise size for the string, I guess
    file.seekg(0);
    if (!file)
        throw std::runtime_error("file seek");
    file.read(content.data(), size);
}
} // namespace

struct CharEncodingFileTest : public ::testing::Test {
    CharEncodingFileTest() = default;

    void SetUp() override {
        file_path_name_ext_ = "i_can_be_deleted__temp_txt_file_";
        file_path_name_ext_ += CharEncodingFileTest::counter++;
        file_path_name_ext_ += ".txt";
    }

    void write_file(const std::string& text) const {
        save_to_text_file(file_path_name_ext_, text);
    }

    [[nodiscard]] std::string read_file() const {
        std::string ret;
        load_from_text_file(file_path_name_ext_, ret);
        return ret;
    }

    static bool is_valid_utf8(const std::string& s) {
        return utf8::is_valid(s.begin(), s.end());
    }

    void TearDown() override {
        const int ret = std::remove(file_path_name_ext_.c_str());
        ASSERT_EQ(0, ret);
    }

    ~CharEncodingFileTest() override = default;

private:
    std::string file_path_name_ext_;
    static inline char counter{'a'};
};

TEST_F(CharEncodingFileTest, ASCII) {
    const std::string s = "!!AA~~";
    write_file(s);
    EXPECT_TRUE(is_valid_utf8(s));
    const std::string read = read_file();
    EXPECT_EQ(read, s);
}

TEST_F(CharEncodingFileTest, ASCIIhex) {
    const std::string s = "\x21\x21"
                          "AA~\x7e";
    EXPECT_TRUE(s == "!!AA~~");
    EXPECT_TRUE(is_valid_utf8(s));
    write_file(s);
    const std::string read = read_file();
    EXPECT_EQ(read, s);
}

TEST_F(CharEncodingFileTest, UTF8) {
    const std::string s = "\xc3\xa4"
                          "hnlich";
    EXPECT_TRUE(is_valid_utf8(s));
    write_file(s);
    const std::string read = read_file();
    EXPECT_EQ(read, s);
}

TEST_F(CharEncodingFileTest, UTF8_cyrillic) {
    const std::string s = "schtscha:\xd0\xa9";
    EXPECT_TRUE(is_valid_utf8(s));
    write_file(s);
    const std::string read = read_file();
    EXPECT_EQ(read, s);
}

TEST_F(CharEncodingFileTest, latin1) {
    const std::string s = "\xe4\xf6\xfc"; // latin1 (aeoeue)
    EXPECT_FALSE(is_valid_utf8(s));
    write_file(s);
    const std::string read = read_file();
    EXPECT_EQ(read, s);
}

TEST(consoleTest, utf8_to_utf16) {
    const std::string s = "\xc3\xa4"
                          "hnlich";
    std::cout << s << "\n";

    std::u16string utf16to;
    utf8::utf8to16(s.begin(), s.end(), std::back_inserter(utf16to));

    const std::wstring ws(utf16to.begin(), utf16to.end());

    // Cf. locale.test.cpp #locale_facet-exception for explanation.
#if !(UL_COMP_MINGW && UL_COMP_MINGW_VER <= 50300)
    const ul::SetGlobalLocaleScoped loc{ul::GlobalLocale::user_preferred};
#endif
    std::wcout << ws << L"\n";
    std::cout << s << "\n";
}
