// Markus Borris, 2011-2016
// This file is part of Toolib library.

//!
/**
*/
//! \file


#ifndef STR_CONVERT_DETAIL_H_sdhixergh3q78q473gnyt3yg
#define STR_CONVERT_DETAIL_H_sdhixergh3q78q473gnyt3yg

#include "Toolib/error.h"
#include "Toolib/ignore_arg.h"
#include "Toolib/PPDEFS.h"
#include "../../../../sdks/utf8cpp/source/utf8.h"
#include <algorithm>
#include <iomanip>
#include <iterator>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#if !TOO_HAS_NO_CODECVT
#include <codecvt>
#endif
#if TOO_OS_WINDOWS
#include <windows.h>
#endif


namespace too
{
namespace str
{
inline std::string utf16or32to8_ws2s_portable(const std::wstring& wstr);

inline std::string utf16to8_ws2s(const std::wstring& wstr)
{
#if TOO_OS_WINDOWS
    std::string convertedString;
    int requiredSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, 0, 0, 0, 0);
    if (requiredSize > 0)
    {
        std::vector<char> buffer(requiredSize);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &buffer[0], requiredSize, 0, 0);
        convertedString.assign(buffer.begin(), buffer.end() - 1);
    }
    return convertedString;
#else
    return utf16or32to8_ws2s_portable(wstr);
#endif
}

inline std::string utf16or32to8_ws2s_portable(const std::wstring& wstr)
{
#if TOO_SIZEOF_WCHAR_T == 2
    std::string ret;
    utf8::utf16to8(wstr.begin(), wstr.end(), std::back_inserter(ret));
    return ret;
#elif TOO_SIZEOF_WCHAR_T == 4
    // not tested!
    std::u32string tmp(wstr.begin(), wstr.end());
    std::string ret;
    utf8::utf32to8(tmp.begin(), tmp.end(), std::back_inserter(ret));
    return ret;
#else
    too::ignore_arg(wstr);
    throw too::not_implemented{"utf16or32to8_ws2s_portable"};
#endif
}

inline std::wstring utf8to16or32_s2ws_portable(const std::string& str);

inline std::wstring utf8to16_s2ws(const std::string& str)
{
#if TOO_OS_WINDOWS
    std::wstring convertedString;
    int requiredSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, 0, 0);
    if (requiredSize > 0)
    {
        std::vector<wchar_t> buffer(requiredSize);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &buffer[0], requiredSize);
        convertedString.assign(buffer.begin(), buffer.end() - 1);
    }
    return convertedString;
#else
    return utf8to16or32_s2ws_portable(str);
#endif
}

inline std::wstring utf8to16or32_s2ws_portable(const std::string& str)
{
#if TOO_SIZEOF_WCHAR_T == 2
    std::wstring ret;
    utf8::utf8to16(str.begin(), str.end(), std::back_inserter(ret));
    return ret;
#elif TOO_SIZEOF_WCHAR_T == 4
    // not tested!
    std::u32string utf32;
    utf8::utf8to32(str.begin(), str.end(), std::back_inserter(utf32));
    std::wstring ret(utf32.begin(), utf32.end());
    return ret;
#else
    too::ignore_arg(str);
    throw too::not_implemented{"utf8to16or32_s2ws_portable"};
#endif
}

#if !TOO_HAS_NO_CODECVT
inline std::string utf16to8_ws2s_codecvt(const std::wstring& wstr)
{
    using cc = std::codecvt_utf8_utf16<wchar_t>;
    std::wstring_convert<cc, wchar_t> converter;

    return converter.to_bytes(wstr);
}

inline std::wstring utf8to16_s2ws_codecvt(const std::string& str)
{
    // Note that this isn't understood very well.
    // The test under Windows and mingw succeeds for the little_endian choice.
    // It could very well be that this doesn't hold on every platform.
    using cc = std::codecvt_utf8_utf16<wchar_t, 0x10ffff, std::little_endian>;
    std::wstring_convert<cc, wchar_t> converter;

    return converter.from_bytes(str);
}
#endif

/*inline std::string utf8_ws2s(const std::wstring& wstr)
{
    // of course this could be implemented better, but since this shouldn't be used anyway, a quick version was welcome
    std::string ret;
    std::u16string interm;
    // compiler warning, utf8cpp expects 8bit type for utf8
    utf8::utf8to16(wstr.begin(), wstr.end(), std::back_inserter(interm));
    utf8::utf16to8(interm.begin(), interm.end(), std::back_inserter(ret));
    return ret;
}*/

/*inline std::wstring utf8_s2ws(const std::string& str)
{
    // of course this could be implemented better, but since this shouldn't be used anyway, a quick version was welcome
    std::wstring ret;
    std::u16string interm;
    utf8::utf8to16(str.begin(), str.end(), std::back_inserter(interm));
    utf8::utf16to8(interm.begin(), interm.end(), std::back_inserter(ret));
    return ret;
}*/

namespace detail
{
#if TOO_OS_WINDOWS
inline std::wstring acp_s2ws(const std::string& s)
{
    int slength = static_cast<int>(s.length()) + 1;
    if (slength < 0)
    {
#pragma push_macro("max")
#undef max
        slength = std::numeric_limits<int>::max();
#pragma pop_macro("max")
    }
    int len      = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    try
    {
        MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
        std::wstring r(buf);
        delete[] buf;
        return r;
    }
    catch (...)
    {
        delete[] buf;
        return L"";
    }
}
#endif
inline std::wstring s2ws(const std::string& s)
{
    std::wstring ws(s.length(), L' ');
    std::copy(s.begin(), s.end(), ws.begin());
    return ws;
}
}

inline std::wstring locenc_s2ws(const std::string& s)
{
#if TOO_OS_WINDOWS
    return detail::acp_s2ws(s);
#else
    return detail::s2ws(s);
#endif
}

namespace detail_impl
{
template <class OnConversionErrorPolicy = ConversionErrorToQuestionMark>
inline std::string utf8_to_latin1_range(const std::string& s, unsigned int from, unsigned int to)
{
    std::string ret;

    const char* cptr = s.c_str();
    unsigned int ui_codepoint{};
    while (*cptr != 0)
    {
        unsigned char uc = static_cast<unsigned char>(*cptr);
        if (uc <= 0x7f)
            ui_codepoint = uc;
        else if (uc <= 0xbf)
            ui_codepoint = (ui_codepoint << 6) | (uc & 0x3f);
        else if (uc <= 0xdf)
            ui_codepoint = uc & 0x1f;
        else if (uc <= 0xef)
            ui_codepoint = uc & 0x0f;
        else
            ui_codepoint = uc & 0x07;
        ++cptr;
        if (((*cptr & 0xc0) != 0x80) && (ui_codepoint <= 0x10ffff))
        {
            if (from <= ui_codepoint && ui_codepoint <= to)
                ret.append(1, static_cast<char>(ui_codepoint));
            else
                ret.append(1, OnConversionErrorPolicy::onConversionError(ui_codepoint));
        }
    }
    return ret;
}
} // detail_impl

template <class OnConversionErrorPolicy>
inline std::string utf8_to_latin1(const std::string& s)
{
    return detail_impl::utf8_to_latin1_range<OnConversionErrorPolicy>(s, 0, 255);
}

inline std::string latin1_to_utf8(const std::string& s)
{
    std::string ret;

    for (const auto& c : s)
    {
        uint8_t c_ = c;

        if (c_ < 0x80)
            ret.append(1, c_);
        else
        {
            ret.append(1, 0xc0 | (c_ & 0xc0) >> 6);
            ret.append(1, 0x80 | (c_ & 0x3f));
        }
    }
    return ret;
}

template <class OnConversionErrorPolicy>
inline std::string utf8_to_printableASCII(const std::string& s)
{
    return detail_impl::utf8_to_latin1_range<OnConversionErrorPolicy>(s, 32, 126);
}

inline std::string printableASCII_to_utf8(const std::string& s) { return latin1_to_utf8(s); }

inline std::string toHexString(const std::string& s, const std::string& prefix)
{
    static const char* const lut{"0123456789abcdef"};
    size_t length = s.size();
    std::string ret;
    ret.reserve((2 + prefix.size()) * length);
    std::for_each(std::begin(s), std::end(s), [&ret, &prefix](char c)
        {
            const unsigned char uc = c;
            ret.append(prefix);
            ret.push_back(lut[uc >> 4]);
            ret.push_back(lut[uc & 15]);
        });
    return ret;
}
} // str
} // too
#endif
