// Markus Borris, 2011
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file

#pragma once
#ifndef TOOTYPES_H_INCL_com3iur982zxr920z427
#define TOOTYPES_H_INCL_com3iur982zxr920z427

#include <string>
#include <sstream>
#include <cstdint>
#include "Toolib/PPDEFS.H"

namespace too
{
//! String type. Just use too::string and too::stringstream and _TOOSTR("bla") for literals and too::charType
/** Be prepared, that the string behaves exactly like a std::string.
    Detailed explanation how the type evolved. Two choices were to be considered
    1. which type out of char==char8_t, wchar_t, char16_t, char32_t
        (Here you have to notice, that wchar_t is the worst choice, because it doesn't have platform independent
        fixed size, whereas char is always 8 Bit, and the other are 16, 32 Bit respectively.
    2. which encoding out of local (whatever that may be, perhaps ISO-8859-1 Latin 1 on german systems),
        wide character (whatever that superimposes on the local whatever-thing, but usually UTF-16 or UTF-32),
        UTF-8, UTF-16, UTF-32
        (Here you should notice that UTF-8 seems to be to most common choice on the internet.)

    So, what's your choice for the time being?
    Decision:
        char and UTF-8
    Downside (always inevitable, but looking for the lesser of two evils):
        i) Convert to UTF16/32 encoding and suitable type whenever appropriate for languages,
            or frameworks/libraries (e.g. Windows API would require conversions to/from UTF-16 + wchar_t)
        ii) Convert to UTF16 + char for std::fstream & Co. as long as necessary
        iii) Exclusively write english/ASCII source code files if your compiler doesn't support the u8 string literal
            declaration prefix yet*/

#define _ENCODING_LOCAL(x) x
#define _ENCODING_LOCAL_WIDE(x) L##x
#if TOO_HAS_NO_CPP11_U8
#define _ENCODING_UTF8(x) _ENCODING_LOCAL(x)
#else
#define _ENCODING_UTF8(x) u8##x
#endif
#define _ENCODING_UTF16(x) u##x
#define _ENCODING_UTF32(x) U##x

template <typename CHAR_TYPE>
using stdbasicstring_chartype = std::basic_string<CHAR_TYPE>;
#ifndef char8_t
typedef char char8_t;
#endif

//! Number suffixes below denote bits.
/** One note on std::stringstream, here only the w and non-w (for string8) variants seem to be supported.*/
using string8  = stdbasicstring_chartype<char8_t>;
using string16 = stdbasicstring_chartype<char16_t>;
using string32 = stdbasicstring_chartype<char32_t>;
using stringw  = stdbasicstring_chartype<wchar_t>;

//! Number suffixes below denote specific UTF encoding..
#define _TOOSTR_LOCAL(x) _ENCODING_LOCAL(x)
#define _TOOSTR_LOCALWIDE(x) _ENCODING_LOCAL_WIDE(x)
#define _TOOSTR_UTF8(x) _ENCODING_UTF8(x)
#define _TOOSTR_UTF16(x) _ENCODING_UTF16(x)
#define _TOOSTR_UTF32(x) _ENCODING_UTF32(x)

#if TOO_OS_WINDOWS
//    using string = string16;
//    #define _TOOSTR(x)     _TOOSTR_UTF16(x)
// try not using wstring anymore... but is u16string above a worthy alternative!?
//    using string = stringw;
//    #define _TOOSTR(x)     _TOOSTR_LOCALWIDE(x)
// finally opt for a decision candidate
using string       = string8;
using stringstream = std::stringstream;
#define _TOOSTR(x) _TOOSTR_UTF8(x)
#if !defined(UNICODE) && !defined(_UNICODE)
#error \
    "Under Windows your code has to be compiled with Unicode setting - according to a desired choice of encoding >= UTF8"
#endif
#else
using string       = string8;
using stringstream = std::stringstream;
#define _TOOSTR(x) _TOOSTR_UTF8(x)
#endif
using charType = string::value_type;

//! This function's name promises a bit too much.
/** It is still a todo to switch string literal encoding (prefixes u8, u16, u32, L) without preprocessor defines
    and then make a proper check here. But for practical purposes you probably start with UTF8 and want to make a
    switch to UTF16 later. Then it is very likely, that you also switch from 8 Bits to 16 Bits representation,
    in which case the assertion fires easily.*/
template <typename StringType, size_t BitsPerCharacter>
inline
#ifdef constexpr
    constexpr
#endif
    void
    static_assert_string_BitsPerChar_and_Encoding()
{
    static_assert(sizeof(typename StringType::value_type) == BitsPerCharacter / 8,
        "Change this function if you change the byte count per character in your string. You probably also want to "
        "have a look at your encoding (UTF-?).");
}
}

#endif
