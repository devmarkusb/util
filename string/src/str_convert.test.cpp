#include "ul/string/str_convert.h"
#include "utf8/unchecked.h"
#include "gtest/gtest.h"
#include <iterator>
#include <string>

namespace ul = mb::ul;

TEST(utf16to8_ws2sTest, test) {
    // use the perfectly tested utf8cpp lib to construct an utf16 wstring
    std::string utf8("\xc3\xa4"
                     "hnlich\x21");
    std::u16string utf16;
    utf8::unchecked::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(utf16));
    const std::wstring ws_from_utf16(utf16.begin(), utf16.end());

    const std::string s = ul::str::utf16to8_ws2s(ws_from_utf16);
    EXPECT_EQ(s, utf8);
}

TEST(utf16to8_ws2s_portableTest, test) {
    // use the perfectly tested utf8cpp lib to construct an utf16 wstring
    std::string utf8("\xc3\xa4"
                     "hnlich\x21");
    std::u16string utf16;
    utf8::unchecked::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(utf16));
    const std::wstring ws_from_utf16(utf16.begin(), utf16.end());

    const std::string s = ul::str::utf16or32to8_ws2s_portable(ws_from_utf16);
    EXPECT_EQ(s, utf8);
}

TEST(utf8to16_s2wsTest, test) {
    // use the perfectly tested utf8cpp lib to yield a comparison utf16 wstring
    std::string utf8("\xc3\xa4"
                     "hnlich\x21");
    std::u16string utf16;
    utf8::unchecked::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(utf16));
    const std::wstring ws_from_utf16(utf16.begin(), utf16.end());

    const std::wstring ws_other = ul::str::utf8to16_s2ws(utf8);
    EXPECT_EQ(ws_from_utf16, ws_other);
}

TEST(utf8to16_s2ws_portableTest, test) {
    // use the perfectly tested utf8cpp lib to yield a comparison utf16 wstring
    std::string utf8("\xc3\xa4"
                     "hnlich\x21");
    std::u16string utf16;
    utf8::unchecked::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(utf16));
    const std::wstring ws_from_utf16(utf16.begin(), utf16.end());

    const std::wstring ws_other = ul::str::utf8to16or32_s2ws_portable(utf8);
    EXPECT_EQ(ws_from_utf16, ws_other);
}

TEST(utf16to8_ws2s_codecvtTest, test) {
#if !UL_HAS_NO_CODECVT
    // use the perfectly tested utf8cpp lib to construct an utf16 wstring
    std::string utf8("\xc3\xa4"
                     "hnlich\x21");
    std::u16string utf16;
    utf8::unchecked::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(utf16));
    const std::wstring ws_from_utf16(utf16.begin(), utf16.end());

    const std::string s = ul::str::utf16to8_ws2s_codecvt(ws_from_utf16);
    EXPECT_EQ(s, utf8);
#endif
}

TEST(utf8to16_s2ws_codecvtTest, test) {
#if !UL_HAS_NO_CODECVT
    // use the perfectly tested utf8cpp lib to yield a comparison utf16 wstring
    std::string utf8("\xc3\xa4"
                     "hnlich\x21");
    std::u16string utf16;
    utf8::unchecked::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(utf16));
    const std::wstring ws_from_utf16(utf16.begin(), utf16.end());

    const std::wstring ws_other = ul::str::utf8to16_s2ws_codecvt(utf8);
    EXPECT_EQ(ws_from_utf16, ws_other);
#endif
}

TEST(locenc_s2wsTest, test) {
    const std::string loc("local stuff: \xe4"); // latin1 ae
    const std::wstring ws(ul::str::locenc_s2ws(loc));
#if UL_OS_WINDOWS
    EXPECT_EQ(L"local stuff: \xe4", ws);
#else
    EXPECT_EQ(L"local stuff: \xffffffe4", ws);
#endif
}

TEST(utf8toHTML, test) {
    const std::string s("\xc3\xa4"
                        "hnlich\x21 Und nochmal:\xc3\xa4");
    const std::string ret = ul::str::utf8_to_html(s);
    EXPECT_EQ("&#228;hnlich! Und nochmal:&#228;", ret);
}

TEST(utf8_to_latin1, test) {
    const std::string utf8{"\xc3\xa4"};
    std::string latin1{ul::str::utf8_to_latin1(utf8)};
    EXPECT_EQ('\xe4', latin1[0]);
}

TEST(utf8_to_latin1, roundtrip) {
    const std::string utf8{"\xc3\xa4"};
    const std::string latin1{ul::str::utf8_to_latin1(utf8)};
    const std::string utf8_roundtrip{ul::str::latin1_to_utf8(latin1)};
    EXPECT_STREQ(utf8.c_str(), utf8_roundtrip.c_str());
}

TEST(utf8_to_latin1, failtest) {
    const std::string utf8{"\xD1\x88"}; // russian scha
    std::string latin1{ul::str::utf8_to_latin1(utf8)};
    EXPECT_EQ('?', latin1[0]);
}

TEST(utf8_to_printable_ascii, test) {
    const std::string utf8{" abc123xyz~"};
    const std::string ascii{ul::str::utf8_to_printable_ascii(utf8)};
    EXPECT_STREQ(" abc123xyz~", ascii.c_str());
}

TEST(utf8_to_printable_ascii, roundtrip) {
    const std::string utf8{" abc123xyz~"};
    const std::string ascii{ul::str::utf8_to_printable_ascii(utf8)};
    const std::string utf8_roundtrip{ul::str::printable_ascii_to_utf8(ascii)};
    EXPECT_STREQ(utf8.c_str(), utf8_roundtrip.c_str());
}

TEST(utf8_to_printable_ascii, failtest) {
    std::string utf8{"\x1f"}; // 31
    std::string ascii{ul::str::utf8_to_printable_ascii(utf8)};
    EXPECT_EQ('?', ascii[0]);

    utf8 = "\x7f"; // 127
    ascii = ul::str::utf8_to_printable_ascii(utf8);
    EXPECT_EQ('?', ascii[0]);
}

TEST(toHexString, test) {
    EXPECT_STREQ("c3a4", ul::str::to_hex_string("\xc3\xa4").c_str());
    EXPECT_STREQ("\\xc3\\xa4", ul::str::to_hex_string("\xc3\xa4", "\\x").c_str());
}
