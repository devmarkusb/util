#include "gtest/gtest.h"
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <iterator>
#include "utf8.h"
#include "toolib/PPDEFS.h"
#include "toolib/ignore_arg.h"


TEST(utf8cpp_is_validTest, test)
{
    std::string s = "schtscha:\xd0\xa9";
    EXPECT_TRUE(utf8::is_valid(s.begin(), s.end()));
    s = "\xe4\xf6\xfc"; // latin1 aeoeue
    EXPECT_FALSE(utf8::is_valid(s.begin(), s.end()));
}

TEST(utf8cpp_find_invalidTest, test)
{
    char utf_invalid[] = "\xe6\x97\xa5\xd1\x88\xfa";
    char* invalid = utf8::find_invalid(utf_invalid, utf_invalid + 6);
    EXPECT_TRUE(invalid == utf_invalid + 5);
}

TEST(utf8cpp_replace_invalidTest, test)
{
    std::string res;
    std::string s = "some invalid ones: \xe4,\xf6,\xfc"; // latin1 ae oe ue
    char32_t rc = 0x3f;
    utf8::replace_invalid(s.begin(), s.end(), std::back_inserter(res), rc);
    EXPECT_EQ("some invalid ones: ?,?,?", res);

    res.clear();
    s = "some invalid ones: \xe4,\xf6,\xfc"; // latin1 ae oe ue
    utf8::replace_invalid(s.begin(), s.end(), std::back_inserter(res), '?');
    EXPECT_EQ("some invalid ones: ?,?,?", res);

    char invalid_sequence[] = "a\x80\xe0\xa0\xc0\xaf\xed\xa0\x80z";
    std::vector<char> replace_invalid_result;
    utf8::replace_invalid(
        invalid_sequence, invalid_sequence + sizeof(invalid_sequence), std::back_inserter(replace_invalid_result), '?');
    bool bvalid = utf8::is_valid(replace_invalid_result.begin(), replace_invalid_result.end());
    EXPECT_TRUE(bvalid);
    const char* fixed_invalid_sequence = "a????z";
    EXPECT_TRUE(std::equal(replace_invalid_result.begin(), replace_invalid_result.end(), fixed_invalid_sequence));
}

TEST(utf8cpp_appendTest, test)
{
    unsigned char u[5] = {0, 0, 0, 0, 0};
    unsigned char* end = utf8::append(0x0448, u); // cyrillic scha
    too::ignore_arg(end);
    EXPECT_TRUE(u[0] == 0xd1 && u[1] == 0x88 && u[2] == 0 && u[3] == 0 && u[4] == 0);
}

TEST(utf8cpp_nextTest, test)
{
    char twochars[] = "\xe6\x97\xa5\xd1\x88";
    char* w         = twochars;
    int cp = utf8::next(w, twochars + 6);
    EXPECT_EQ(0x65e5, cp);
    EXPECT_EQ(w, twochars + 3);
}

TEST(utf8cpp_peek_nextTest, test)
{
    char twochars[] = "\xe6\x97\xa5\xd1\x88";
    char* w         = twochars;
    int cp = utf8::peek_next(w, twochars + 6);
    EXPECT_EQ(0x65e5, cp);
    EXPECT_EQ(w, twochars);
}

TEST(utf8cpp_priorTest, test)
{
    char twochars[] = "\xe6\x97\xa5\xd1\x88";
    char* w         = twochars + 3;
    int cp = utf8::prior(w, twochars);
    EXPECT_EQ(0x65e5, cp);
    EXPECT_EQ(w, twochars);
}

TEST(utf8cpp_advanceTest, test)
{
    char twochars[] = "\xe6\x97\xa5\xd1\x88";
    char* w = twochars;
    utf8::advance(w, 2, twochars + 6);
    EXPECT_EQ(w, twochars + 5);
}

TEST(utf8cpp_distanceTest, test)
{
    char twochars[] = "\xe6\x97\xa5\xd1\x88";
    ptrdiff_t dist = utf8::distance(twochars, twochars + 5);
    EXPECT_EQ(2, dist);
}

TEST(utf8cpp_is_bomTest, test)
{
    unsigned char byte_order_mark[] = {0xef, 0xbb, 0xbf};
    bool bbom = utf8::is_bom(byte_order_mark);
    EXPECT_TRUE(bbom);
}

TEST(utf8cpp_iteratorTest, test)
{
    char threechars[] = "\xf0\x90\x8d\x86\xe6\x97\xa5\xd1\x88";
    utf8::iterator<char*> it(threechars, threechars, threechars + 9);
    utf8::iterator<char*> it2 = it;
    EXPECT_EQ(it2, it);
    EXPECT_EQ(0x10346u, *it);
    EXPECT_EQ(0x65e5u, *(++it));
    EXPECT_EQ(0x65e5u, (*it++));
    EXPECT_EQ(0x0448u, *it);
    EXPECT_NE(it, it2);
    utf8::iterator<char*> endit(threechars + 9, threechars, threechars + 9);
    EXPECT_EQ(++it, endit);
    EXPECT_EQ(0x0448u, *(--it));
    EXPECT_EQ(0x0448u, (*it--));
    EXPECT_EQ(0x65e5u, *it);
    EXPECT_EQ(--it, utf8::iterator<char*>(threechars, threechars, threechars + 9));
    EXPECT_EQ(0x10346u, *it);
}

TEST(utf8cpp_8to16to8Test, test)
{
    // utf8::utf8to16(it, eos, back_inserter(u16string));
    char utf8_with_surrogates[] = "\xe6\x97\xa5\xd1\x88\xf0\x9d\x84\x9e";
    std::vector<unsigned short> utf16result;
    utf8::utf8to16(utf8_with_surrogates, utf8_with_surrogates + 9, std::back_inserter(utf16result));
    EXPECT_EQ(4u, utf16result.size());
    EXPECT_EQ(0xd834, utf16result[2]);
    EXPECT_EQ(0xdd1e, utf16result[3]);

    unsigned short utf16string[] = {0x41, 0x0448, 0x65e5, 0xd834, 0xdd1e};
    std::vector<unsigned char> utf8result;
    utf8::utf16to8(utf16string, utf16string + 5, back_inserter(utf8result));
    EXPECT_EQ(10u, utf8result.size());
}

TEST(utf8cpp_8to32to8Test, test)
{
    // utf8::utf8to16(it, eos, back_inserter(u16string));
    char twochars[] = "\xe6\x97\xa5\xd1\x88";
    std::vector<int> utf32result;
    utf8::utf8to32(twochars, twochars + 5, std::back_inserter(utf32result));
    EXPECT_EQ(2u, utf32result.size());

    int utf32string[] = {0x448, 0x65E5, 0x10346, 0};
    std::vector<unsigned char> utf8result;
    utf8::utf32to8(utf32string, utf32string + 3, back_inserter(utf8result));
    EXPECT_EQ(9u, utf8result.size());
}

TEST(utf8cpp_unchecked_appendTest, test)
{
    unsigned char u[7] = {0, 0, 0, 0, 0, 0, 0};
    unsigned char* end = utf8::unchecked::append(0x0448, u + 1); // cyrillic scha
    too::ignore_arg(end);
    EXPECT_TRUE(u[0] == 0 && u[1] == 0xd1 && u[2] == 0x88 && u[3] == 0 && u[4] == 0 && u[5] == 0);
}

TEST(utf8cpp_unchecked_nextTest, test)
{
    char twochars[] = "\xe6\x97\xa5\xd1\x88"; // 'U+65E5' 3 bytes + cyrillic scha 2 bytes
    char* w         = twochars;
    int cp = utf8::unchecked::next(w);
    EXPECT_EQ(0x65e5, cp);
    EXPECT_EQ(w, twochars + 3);
}

TEST(utf8cpp_unchecked_peek_nextTest, test)
{
    char twochars[] = "\xe6\x97\xa5\xd1\x88";
    char* w         = twochars;
    int cp = utf8::unchecked::peek_next(w);
    EXPECT_EQ(0x65e5, cp);
    EXPECT_EQ(w, twochars);
}

TEST(utf8cpp_unchecked_priorTest, test)
{
    char twochars[] = "\xe6\x97\xa5\xd1\x88";
    char* w         = twochars + 3;
    int cp = utf8::unchecked::prior(w);
    EXPECT_EQ(0x65e5, cp);
    EXPECT_EQ(w, twochars);
}

TEST(utf8cpp_unchecked_advanceTest, test)
{
    char twochars[] = "\xe6\x97\xa5\xd1\x88";
    char* w = twochars;
    utf8::unchecked::advance(w, 2);
    EXPECT_EQ(w, twochars + 5);
}

TEST(utf8cpp_unchecked_distanceTest, test)
{
    char twochars[] = "\xe6\x97\xa5\xd1\x88";
    size_t dist = utf8::unchecked::distance(twochars, twochars + 5);
    EXPECT_EQ(2u, dist);
}

TEST(utf8cpp_unchecked_iteratorTest, test)
{
    // U+10346 GOTHIC LETTER FAIHU 4 bytes + U+65E5 3 bytes + U+0448 cyrillic scha 2 bytes
    char threechars[] = "\xf0\x90\x8d\x86\xe6\x97\xa5\xd1\x88";
    utf8::unchecked::iterator<char*> it(threechars);
    utf8::unchecked::iterator<char*> it2 = it;
    EXPECT_EQ(it2, it);
    EXPECT_EQ(0x10346u, *it);
    EXPECT_EQ(0x65e5u, *++it);
    EXPECT_EQ(0x65e5u, *it++);
    EXPECT_EQ(0x0448u, *it);
    EXPECT_NE(it, it2);
    utf8::unchecked::iterator<char*> endit(threechars + 9);
    EXPECT_EQ(++it, endit);
    EXPECT_EQ(0x0448u, *(--it));
    EXPECT_EQ(0x0448u, (*it--));
    EXPECT_EQ(0x65e5u, *it);
    EXPECT_EQ(--it, utf8::unchecked::iterator<char*>(threechars));
    EXPECT_EQ(0x10346u, *it);
}

TEST(utf8cpp_unchecked_8to16to8Test, test)
{
    // U+65E5 3 bytes + U+0448 cyrillic scha 2 bytes + U+1D11E MUSICAL SYMBOL G CLEF 4 bytes
    char utf8_with_surrogates[] = "\xe6\x97\xa5\xd1\x88\xf0\x9d\x84\x9e";
    /*std::vector <char16_t> */ std::u16string utf16result;
    utf8::unchecked::utf8to16(utf8_with_surrogates, utf8_with_surrogates + 9, std::back_inserter(utf16result));
    EXPECT_EQ(4u, utf16result.size());
    EXPECT_EQ(0xd834, utf16result[2]);
    EXPECT_EQ(0xdd1e, utf16result[3]);
    // full result: 0x65e5 0x0448 0xd834 0xdd1e
    std::string backto8;
    utf8::unchecked::utf16to8(utf16result.begin(), utf16result.end(), std::back_inserter(backto8));
    std::string orig(utf8_with_surrogates);
    EXPECT_EQ(backto8, orig);

    std::u16string utf16from = {0x65e5, 0x0448, 0xd834, 0xdd1e};
    std::string utf8to;
    utf8::unchecked::utf16to8(utf16from.begin(), utf16from.end(), std::back_inserter(utf8to));
    EXPECT_EQ(utf8to, orig);

    unsigned short utf16string[] = {0x41, 0x0448, 0x65e5, 0xd834, 0xdd1e};
    std::vector<unsigned char> utf8result;
    utf8::unchecked::utf16to8(utf16string, utf16string + 5, std::back_inserter(utf8result));
    EXPECT_EQ(10u, utf8result.size());
}

TEST(utf8cpp_unchecked_8to16to8Test, test2)
{
    std::string utf8("\xc3\xa4"
                     "hnlich\x21");
    std::u16string utf16;
    utf8::unchecked::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(utf16));
    std::string back;
    utf8::unchecked::utf16to8(utf16.begin(), utf16.end(), std::back_inserter(back));
    EXPECT_EQ(back, utf8);

#if TOO_OS_WINDOWS && TOO_SIZEOF_WCHAR_T == 2 // && Windows uses UTF16, which we can assume, I guess
    std::wstring ws = {0xe4};
    ws += L"hnlich!";
    std::wstring ws_from_utf16(utf16.begin(), utf16.end());
    EXPECT_EQ(ws, ws_from_utf16);
// mingw doesn't compile: 'converting to execution character set: Illegal byte sequence'
#if TOO_COMP_MS_VISUAL_STUDIO_CPP
    ws = L"\xe4hnlich!";
    EXPECT_EQ(ws, ws_from_utf16);
#endif

    std::string utf8from = "\xd1\x88\xd1\x88";
    std::u16string utf16to;
    utf8::unchecked::utf8to16(utf8from.begin(), utf8from.end(), std::back_inserter(utf16to));
    std::wstring wsto;
    utf8::unchecked::utf8to16(utf8from.begin(), utf8from.end(), std::back_inserter(wsto));
    std::u16string utf16_from_ws_to(wsto.begin(), wsto.end());
    EXPECT_EQ(utf16to, utf16_from_ws_to);
#endif
}

TEST(utf8cpp_unchecked_8to16to8Test, type_experiments)
{
    {
        std::string utf8("\xc3\xa4"
                         "hnlich\x21");
        std::vector<unsigned short> utf16;
        utf8::unchecked::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(utf16));
        std::string back;
        utf8::unchecked::utf16to8(utf16.begin(), utf16.end(), std::back_inserter(back));
        EXPECT_EQ(back, utf8);
    }

    {
        std::string utf8("\xc3\xa4"
                         "hnlich\x21");
        std::vector<char16_t> utf16;
        utf8::unchecked::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(utf16));
        std::string back;
        utf8::unchecked::utf16to8(utf16.begin(), utf16.end(), std::back_inserter(back));
        EXPECT_EQ(back, utf8);
    }

    {
        std::string utf8("\xc3\xa4"
                         "hnlich\x21");
        std::vector<uint16_t> utf16;
        utf8::unchecked::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(utf16));
        std::string back;
        utf8::unchecked::utf16to8(utf16.begin(), utf16.end(), std::back_inserter(back));
        EXPECT_EQ(back, utf8);
    }

    {
        std::string utf8("\xc3\xa4"
                         "hnlich\x21");
        std::vector<int16_t> utf16;
        utf8::unchecked::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(utf16));
        std::string back;
        utf8::unchecked::utf16to8(utf16.begin(), utf16.end(), std::back_inserter(back));
        EXPECT_EQ(back, utf8);
    }

    {
        std::string utf8("\xc3\xa4"
                         "hnlich\x21");
        std::u32string utf16;
        utf8::unchecked::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(utf16));
        std::string back;
        utf8::unchecked::utf16to8(utf16.begin(), utf16.end(), std::back_inserter(back));
        EXPECT_EQ(back, utf8);
    }

    // in case you want to try a std::string as target (could work in principle) you get a possible data loss warning
    // from the compiler; so utf8:: can't do this, which is ok
}

TEST(utf8cpp_unchecked_8to16Test, spelling)
{
    std::string utf8s{"\xc3\xa4hnlich!"};
    std::u16string utf16_test{0xe4, 'h', 'n', 'l', 'i', 'c', 'h', '!'}; // can I write this? yes...
    std::u16string utf16;
    utf8::utf8to16(utf8s.begin(), utf8s.end(), std::back_inserter(utf16));
    EXPECT_EQ(utf16_test, utf16);
}

TEST(utf8cpp_unchecked_8to32to8Test, test)
{
    // U+65E5 3 bytes + U+0448 cyrillic scha 2 bytes
    char twochars[] = "\xe6\x97\xa5\xd1\x88";
    std::vector<int> utf32result;
    utf8::unchecked::utf8to32(twochars, twochars + 5, std::back_inserter(utf32result));
    EXPECT_EQ(2u, utf32result.size());
    std::string back;
    utf8::unchecked::utf32to8(utf32result.begin(), utf32result.end(), std::back_inserter(back));
    EXPECT_FALSE(std::strcmp(twochars, back.c_str()));

    int utf32string[] = {0x448, 0x65E5, 0x10346, 0};
    std::vector<unsigned char> utf8result;
    utf8::unchecked::utf32to8(utf32string, utf32string + 3, back_inserter(utf8result));
    EXPECT_EQ(9u, utf8result.size());
}

TEST(utf8cpp_unchecked_8to32to8Test, readable_test)
{
    const char* utf8 = "\xc3\xa4"
                       "hnlich\x21";
    std::vector<int> utf32result;
    utf8::unchecked::utf8to32(utf8, utf8 + 9, std::back_inserter(utf32result));
    std::string back;
    utf8::unchecked::utf32to8(utf32result.begin(), utf32result.end(), std::back_inserter(back));
    EXPECT_FALSE(std::strcmp(utf8, back.c_str()));

    std::u32string utf32from = {0xe4, 0x68, 0x6e};
    std::string utf8to;
    utf8::unchecked::utf32to8(utf32from.begin(), utf32from.end(), std::back_inserter(utf8to));
    std::string orig = "\xc3\xa4"
                       "hn";
    EXPECT_EQ(orig, utf8to);
}
