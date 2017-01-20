#include "Toolib/string/str_convert.h"
#include "gtest/gtest.h"
#include <string>


TEST(utf16to8_ws2sTest, test)
{
    // use the perfectly tested utf8cpp lib to construct an utf16 wstring
    std::string utf8("\xc3\xa4"
                     "hnlich\x21");
    std::u16string utf16;
    utf8::unchecked::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(utf16));
    std::wstring ws_from_utf16(utf16.begin(), utf16.end());

    std::string s = too::str::utf16to8_ws2s(ws_from_utf16);
    EXPECT_EQ(s, utf8);
}

TEST(utf16to8_ws2s_portableTest, test)
{
    // use the perfectly tested utf8cpp lib to construct an utf16 wstring
    std::string utf8("\xc3\xa4"
                     "hnlich\x21");
    std::u16string utf16;
    utf8::unchecked::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(utf16));
    std::wstring ws_from_utf16(utf16.begin(), utf16.end());

    std::string s = too::str::utf16or32to8_ws2s_portable(ws_from_utf16);
    EXPECT_EQ(s, utf8);
}

TEST(utf8to16_s2wsTest, test)
{
    // use the perfectly tested utf8cpp lib to yield a comparison utf16 wstring
    std::string utf8("\xc3\xa4"
                     "hnlich\x21");
    std::u16string utf16;
    utf8::unchecked::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(utf16));
    std::wstring ws_from_utf16(utf16.begin(), utf16.end());

    std::wstring ws_other = too::str::utf8to16_s2ws(utf8);
    EXPECT_EQ(ws_from_utf16, ws_other);
}

TEST(utf8to16_s2ws_portableTest, test)
{
    // use the perfectly tested utf8cpp lib to yield a comparison utf16 wstring
    std::string utf8("\xc3\xa4"
                     "hnlich\x21");
    std::u16string utf16;
    utf8::unchecked::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(utf16));
    std::wstring ws_from_utf16(utf16.begin(), utf16.end());

    std::wstring ws_other = too::str::utf8to16or32_s2ws_portable(utf8);
    EXPECT_EQ(ws_from_utf16, ws_other);
}

TEST(utf16to8_ws2s_codecvtTest, test)
{
#if !TOO_HAS_NO_CODECVT
    // use the perfectly tested utf8cpp lib to construct an utf16 wstring
    std::string utf8("\xc3\xa4"
                     "hnlich\x21");
    std::u16string utf16;
    utf8::unchecked::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(utf16));
    std::wstring ws_from_utf16(utf16.begin(), utf16.end());

    std::string s = too::str::utf16to8_ws2s_codecvt(ws_from_utf16);
    EXPECT_EQ(s, utf8);
#endif
}

TEST(utf8to16_s2ws_codecvtTest, test)
{
#if !TOO_HAS_NO_CODECVT
    // use the perfectly tested utf8cpp lib to yield a comparison utf16 wstring
    std::string utf8("\xc3\xa4"
                     "hnlich\x21");
    std::u16string utf16;
    utf8::unchecked::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(utf16));
    std::wstring ws_from_utf16(utf16.begin(), utf16.end());

    std::wstring ws_other = too::str::utf8to16_s2ws_codecvt(utf8);
    EXPECT_EQ(ws_from_utf16, ws_other);
#endif
}

TEST(locenc_s2wsTest, test)
{
    std::string loc("local stuff: \xe4"); // latin1 ae
    std::wstring ws(too::str::locenc_s2ws(loc));
    EXPECT_EQ(L"local stuff: \xe4", ws);
}

TEST(utf8toHTML, test)
{
    std::string s("\xc3\xa4"
                  "hnlich\x21 Und nochmal:\xc3\xa4");
    std::string ret = too::str::utf8toHTML(s);
    EXPECT_EQ("&#228;hnlich! Und nochmal:&#228;", ret);
}

TEST(utf8_to_latin1, test)
{
    std::string utf8{"\xc3\xa4"};
    std::string latin1{too::str::utf8_to_latin1(utf8)};
    EXPECT_EQ('\xe4', latin1[0]);
}

TEST(utf8_to_latin1, roundtrip)
{
    std::string utf8{"\xc3\xa4"};
    std::string latin1{too::str::utf8_to_latin1(utf8)};
    std::string utf8_roundtrip{too::str::latin1_to_utf8(latin1)};
    EXPECT_STREQ(utf8.c_str(), utf8_roundtrip.c_str());
}

TEST(utf8_to_latin1, failtest)
{
    std::string utf8{"\xD1\x88"}; // russian scha
    std::string latin1{too::str::utf8_to_latin1(utf8)};
    EXPECT_EQ('?', latin1[0]);
}

TEST(utf8_to_printableASCII, test)
{
    std::string utf8{" abc123xyz~"};
    std::string ascii{too::str::utf8_to_printableASCII(utf8)};
    EXPECT_STREQ(" abc123xyz~", ascii.c_str());
}

TEST(utf8_to_printableASCII, roundtrip)
{
    std::string utf8{" abc123xyz~"};
    std::string ascii{too::str::utf8_to_printableASCII(utf8)};
    std::string utf8_roundtrip{too::str::printableASCII_to_utf8(ascii)};
    EXPECT_STREQ(utf8.c_str(), utf8_roundtrip.c_str());
}

TEST(utf8_to_printableASCII, failtest)
{
    std::string utf8{"\x1f"}; // 31
    std::string ascii{too::str::utf8_to_printableASCII(utf8)};
    EXPECT_EQ('?', ascii[0]);

    utf8 = "\x7f"; // 127
    ascii = too::str::utf8_to_printableASCII(utf8);
    EXPECT_EQ('?', ascii[0]);
}

TEST(toHexString, test)
{
    EXPECT_STREQ("c3a4", too::str::toHexString("\xc3\xa4").c_str());
    EXPECT_STREQ("\\xc3\\xa4", too::str::toHexString("\xc3\xa4", "\\x").c_str());
}
