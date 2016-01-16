// Markus Borris, 2011
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file


#ifndef STR2WSTR_H_INCL_827rt82rtrxjr38t
#define STR2WSTR_H_INCL_827rt82rtrxjr38t

#include <string>
#include <limits>
#include <vector>
#include "Toolib/PPDEFS.h"
#include "Toolib/argsused.h"
#include "Toolib/error.h"

//! Unfortunately the following C++11 feature isn't yet supported by mingw (and ... ?). Try it with future versions.
#define STR2WSTR_TRY_CODECVT_SUPPORT 0
#if STR2WSTR_TRY_CODECVT_SUPPORT == 1
#include <codecvt>
#endif
#if TOO_OS_WINDOWS
#include <windows.h>
#endif

namespace too
{
namespace str
{

inline std::string Utf16_wstring_To_Utf8_string(const std::wstring& wstr)
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
    //#error "You may need/want to re-implement the code from the if-branch."
    assert(false);
    too::ignore_arg(wstr);
    return std::string();
#endif
}

inline std::wstring Utf8_string_To_Utf16_wstring(const std::string& str)
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
    //#error "You may need/want to re-implement the code from the if-branch."
    assert(false);
    too::ignore_arg(str);
    return std::string();
#endif
}

//! Convert string to wstring. Keeping UTF-8.
inline std::wstring Utf8_string_To_Utf8_wstring(const std::string& str)
{
#if STR2WSTR_TRY_CODECVT_SUPPORT
    typedef std::codecvt_utf8<wchar_t> convert_typeX;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.from_bytes(str);
#else
    //#error "You may need/want to re-implement the code from the if-branch."
	too::ignore_arg(str);
	throw too::not_implemented();
    //return std::wstring();
#endif
}

//! Convert wstring to string. Keeping UTF-8.
inline std::string Utf8_wstring_To_Utf8_string(const std::wstring& wstr)
{
#if STR2WSTR_TRY_CODECVT_SUPPORT
    typedef std::codecvt_utf8<wchar_t> convert_typeX;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.to_bytes(wstr);
#else
    //#error "You may need/want to re-implement the code from the if-branch."
    too::ignore_arg(wstr);
	throw too::not_implemented();
	//return std::string();
#endif
}

//! DEPRECATED! Convert string to wstring implementations.
namespace str_impl
{
#if TOO_OS_WINDOWS
inline std::wstring s2ws_windows(const std::string& s)
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
inline std::wstring s2ws_std(const std::string& s)
{
    std::wstring ws(s.length(), L' ');
    std::copy(s.begin(), s.end(), ws.begin());
    return ws;
}
}

//! DEPRECATED! Convert string to wstring.
/** You probably want to use sth. more precise, like Utf8_string_To_Utf16_wstring or
    Utf8_string_To_Utf8_wstring.*/
inline std::wstring s2ws(const std::string& s)
{
#if TOO_OS_WINDOWS
    return str_impl::s2ws_windows(s);
#else
    return str_impl::s2ws_std(s);
#endif
}

//! DEPRECATED! Convert wstring to string. Cf. comment for s2ws.
inline std::string ws2s(const std::wstring& s)
{
    return std::string(s.begin(), s.end());
}
}
}
#endif
