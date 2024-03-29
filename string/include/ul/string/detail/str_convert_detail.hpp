//! \file

#ifndef STR_CONVERT_DETAIL_H_SDHIXERGH3Q78Q473GNYT3YG
#define STR_CONVERT_DETAIL_H_SDHIXERGH3Q78Q473GNYT3YG

#include "../ulstring.h"
#include "ul/error.h"
#include "ul/ignore_unused.h"
#include "ul/macros.h"
#include <algorithm>
#include <iomanip>
#include <iterator>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#if !UL_HAS_NO_CODECVT
#include <codecvt>
#endif
#if UL_OS_WINDOWS
#include <windows.h>
#endif

namespace mb::ul::str {
inline std::string utf16to8_ws2s(const std::wstring& wstr) {
#if UL_OS_WINDOWS
    std::string convertedString;
    int requiredSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, 0, 0, 0, 0);
    if (requiredSize > 0) {
        std::vector<char> buffer(requiredSize);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &buffer[0], requiredSize, 0, 0);
        convertedString.assign(buffer.begin(), buffer.end() - 1);
    }
    return convertedString;
#else
    return utf16or32to8_ws2s_portable(wstr);
#endif
}

inline std::string utf16or32to8_ws2s_portable(const std::wstring& wstr) {
#if UL_SIZEOF_WCHAR_T == 2
    std::string ret;
    utf8::utf16to8(wstr.begin(), wstr.end(), std::back_inserter(ret));
    return ret;
#elif UL_SIZEOF_WCHAR_T == 4
    // not tested!
    std::u32string tmp(wstr.begin(), wstr.end());
    std::string ret;
    utf8::utf32to8(tmp.begin(), tmp.end(), std::back_inserter(ret));
    return ret;
#else
    ul::ignore_unused(wstr);
    throw ul::NotImplemented{"utf16or32to8_ws2s_portable"};
#endif
}

inline std::wstring utf8to16_s2ws(const std::string& str) {
#if UL_OS_WINDOWS
    std::wstring convertedString;
    int requiredSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, 0, 0);
    if (requiredSize > 0) {
        std::vector<wchar_t> buffer(requiredSize);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &buffer[0], requiredSize);
        convertedString.assign(buffer.begin(), buffer.end() - 1);
    }
    return convertedString;
#else
    return utf8to16or32_s2ws_portable(str);
#endif
}

inline std::wstring utf8to16or32_s2ws_portable(const std::string& str) {
#if UL_SIZEOF_WCHAR_T == 2
    std::wstring ret;
    utf8::utf8to16(str.begin(), str.end(), std::back_inserter(ret));
    return ret;
#elif UL_SIZEOF_WCHAR_T == 4
    // not tested!
    std::u32string utf32;
    utf8::utf8to32(str.begin(), str.end(), std::back_inserter(utf32));
    std::wstring ret(utf32.begin(), utf32.end());
    return ret;
#else
    ul::ignore_unused(str);
    throw ul::NotImplemented{"utf8to16or32_s2ws_portable"};
#endif
}

#if !UL_HAS_NO_CODECVT
UL_PRAGMA_WARNINGS_PUSH
// clang-format off
UL_WARNING_DISABLE_CLANG(deprecated-declarations)

// clang-format on
inline std::string utf16to8_ws2s_codecvt(const std::wstring& wstr) {
    using CC = std::codecvt_utf8_utf16<wchar_t>;
    std::wstring_convert<CC, wchar_t> converter;

    return converter.to_bytes(wstr);
}

inline std::wstring utf8to16_s2ws_codecvt(const std::string& str) {
    // Note that this isn't understood very well.
    // The test under Windows and mingw succeeds for the little_endian choice.
    // It could very well be that this doesn't hold on every platform.
    constexpr auto max_code{0x10ffff}; // the default
    using Cc = std::codecvt_utf8_utf16<wchar_t, max_code, std::little_endian>;
    std::wstring_convert<Cc, wchar_t> converter;

    return converter.from_bytes(str);
}

UL_PRAGMA_WARNINGS_POP
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

namespace detail {
#if UL_OS_WINDOWS
#undef max

inline std::wstring acp_s2ws(const std::string& s) {
    int slength = static_cast<int>(s.length()) + 1;
    if (slength < 0) {
        slength = std::numeric_limits<int>::max();
    }
    int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    try {
        MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
        std::wstring r(buf);
        delete[] buf;
        return r;
    } catch (...) {
        delete[] buf;
        return L"";
    }
}
#endif
inline std::wstring s2ws(const std::string& s) {
    std::wstring ws(s.length(), L' ');
    std::copy(s.begin(), s.end(), ws.begin());
    return ws;
}
} // namespace detail

inline std::wstring locenc_s2ws(const std::string& s) {
#if UL_OS_WINDOWS
    return detail::acp_s2ws(s);
#else
    return detail::s2ws(s);
#endif
}

namespace detail_impl {
#if UL_COMP_MS_VISUAL_STUDIO_CPP && UL_COMP_MS_VS_VER == 1900
#define TEMP_REMOVE_STRANGE_WRONG_WARNING_ABOUT_UNREACHABLE_CODE 1
#endif
#if TEMP_REMOVE_STRANGE_WRONG_WARNING_ABOUT_UNREACHABLE_CODE
#pragma warning(push)
#pragma warning(disable : 4702)
#endif
template <class OnConversionErrorPolicy = ConversionErrorToQuestionMark, unsigned int from, unsigned int to>
std::string utf8_to_latin1_range(const std::string& s) {
    std::string ret;

    // magic numbers below, rtfm
    const char* cptr = s.c_str();
    unsigned int ui_codepoint{};
    while (*cptr != 0) {
        const auto uc = static_cast<unsigned char>(*cptr);
        if (uc <= 0x7f) // NOLINT
            ui_codepoint = uc;
        else if (uc <= 0xbf) // NOLINT
            ui_codepoint = (ui_codepoint << 6u) | (uc & 0x3fu); // NOLINT
        else if (uc <= 0xdf) // NOLINT
            ui_codepoint = uc & 0x1fu; // NOLINT
        else if (uc <= 0xef) // NOLINT
            ui_codepoint = uc & 0x0fu; // NOLINT
        else
            ui_codepoint = uc & 0x07u; // NOLINT
        ++cptr; // NOLINT
        if (((static_cast<unsigned char>(*cptr) & 0xc0u) != 0x80u) && (ui_codepoint <= 0x10ffffu)) { // NOLINT
            bool in_range{ui_codepoint <= to};
            if constexpr (from)
                in_range = in_range && from <= ui_codepoint;
            if (in_range)
                ret.append(1, static_cast<char>(ui_codepoint));
            else
                // for that the wrong warning in msvc2015 occurs, but tests prove that the code is
                // actually reached for all template instantiations used
                ret.append(1, OnConversionErrorPolicy::on_conversion_error(ui_codepoint));
        }
    }
    return ret;
#if TEMP_REMOVE_STRANGE_WRONG_WARNING_ABOUT_UNREACHABLE_CODE
#pragma warning(pop)
#undef TEMP_REMOVE_STRANGE_WRONG_WARNING_ABOUT_UNREACHABLE_CODE
#endif
}
} // namespace detail_impl

template <class OnConversionErrorPolicy>
std::string utf8_to_latin1(const std::string& s) {
    return detail_impl::utf8_to_latin1_range<OnConversionErrorPolicy, 0, std::numeric_limits<unsigned char>::max()>(s);
}

inline std::string latin1_to_utf8(const std::string& s) {
    std::string ret;

    for (const auto& c : s) {
        const auto ui8 = static_cast<uint8_t>(c);

        if (ui8 < 0x80) // NOLINT
            ret.append(1, c);
        else {
            const auto c_pt1 = 0xc0u | (ui8 & 0xc0u) >> 6u; // NOLINT
            ret.append(1, static_cast<char>(c_pt1));
            const auto c_pt2 = 0x80u | (ui8 & 0x3fu); // NOLINT
            ret.append(1, static_cast<char>(c_pt2));
        }
    }
    return ret;
}

template <class OnConversionErrorPolicy>
std::string utf8_to_printable_ascii(const std::string& s) {
    return detail_impl::utf8_to_latin1_range<OnConversionErrorPolicy, 32, 126>(s); // NOLINT
}

inline std::string printable_ascii_to_utf8(const std::string& s) {
    return latin1_to_utf8(s);
}

inline std::string to_hex_string(const std::string& s, const std::string& prefix) {
    static const char* const lut{"0123456789abcdef"};
    const size_t length = s.size();
    std::string ret;
    ret.reserve((2 + prefix.size()) * length);
    std::for_each(std::begin(s), std::end(s), [&ret, &prefix](char c) {
        const auto uc = static_cast<unsigned char>(c);
        ret.append(prefix);
        ret.push_back(lut[uc >> 4]); // NOLINT
        ret.push_back(lut[uc & 15]); // NOLINT
    });
    return ret;
}
} // namespace mb::ul::str

#endif
