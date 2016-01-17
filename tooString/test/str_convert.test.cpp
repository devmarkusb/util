#include "gtest/gtest.h"
#include <string>
#include "Toolib/string/str_convert.h"


TEST(utf16to8_ws2sTest, test)
{
    // use the perfectly tested utf8cpp lib to construct an utf16 wstring
    std::string utf8("\xc3\xa4""hnlich\x21");
    std::u16string utf16;
    utf8::unchecked::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(utf16));
    std::wstring ws_from_utf16(utf16.begin(), utf16.end());

    std::string s = too::str::utf16to8_ws2s(ws_from_utf16);
    EXPECT_EQ(s, utf8);
}

TEST(utf16to8_ws2s_portableTest, test)
{
    // use the perfectly tested utf8cpp lib to construct an utf16 wstring
    std::string utf8("\xc3\xa4""hnlich\x21");
    std::u16string utf16;
    utf8::unchecked::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(utf16));
    std::wstring ws_from_utf16(utf16.begin(), utf16.end());

    std::string s = too::str::utf16to8_ws2s_portable(ws_from_utf16);
    EXPECT_EQ(s, utf8);
}

TEST(utf8to16_s2wsTest, test)
{
    // use the perfectly tested utf8cpp lib to yield a comparison utf16 wstring
    std::string utf8("\xc3\xa4""hnlich\x21");
    std::u16string utf16;
    utf8::unchecked::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(utf16));
    std::wstring ws_from_utf16(utf16.begin(), utf16.end());

    std::wstring ws_other = too::str::utf8to16_s2ws(utf8);
    EXPECT_EQ(ws_from_utf16, ws_other);
}

TEST(utf8to16_s2ws_portableTest, test)
{
    // use the perfectly tested utf8cpp lib to yield a comparison utf16 wstring
    std::string utf8("\xc3\xa4""hnlich\x21");
    std::u16string utf16;
    utf8::unchecked::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(utf16));
    std::wstring ws_from_utf16(utf16.begin(), utf16.end());

    std::wstring ws_other = too::str::utf8to16_s2ws_portable(utf8);
    EXPECT_EQ(ws_from_utf16, ws_other);
}

TEST(utf16to8_ws2s_codecvtTest, test)
{
#if !TOO_HAS_NO_CODECVT
    // use the perfectly tested utf8cpp lib to construct an utf16 wstring
    std::string utf8("\xc3\xa4""hnlich\x21");
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
    std::string utf8("\xc3\xa4""hnlich\x21");
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
