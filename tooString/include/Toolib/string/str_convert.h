// Markus Borris, 2011-2016
// This file is part of Toolib library.

//!
/**
*/
//! \file


#ifndef STR_CONVERT_H_INCL_827rt82rtrxjr38t
#define STR_CONVERT_H_INCL_827rt82rtrxjr38t

#include <string>

//! utf8.h included for the major conversion support; whether used in this file or not.
//! Use utf8::... for ultimate utf8 support with almost arbitrary string types.
#include "../../../sdks/utf8cpp/source/utf8.h"


namespace too
{
namespace str
{

//! Whenever a std::wstring crosses your path (god forbid!).
//! Note that the *_portable versions are recommended over their pendants. They are more future proof.
//! And of course the *_codecvt are in direct competition.
//!@{

//! Uses winapi. Encoding is unchecked. Calls utf16to8_ws2s_portable() for non-Windows, which could be used for Windows as well.
inline std::string utf16to8_ws2s(const std::wstring& wstr);

//! Doesn't use winapi (but note, wchar_t isn't recommended at all). Encoding is unchecked.
inline std::string utf16to8_ws2s_portable(const std::wstring& wstr);

//! Uses winapi. Encoding is unchecked. Calls utf16to8_ws2s_portable() for non-Windows, which could be used for Windows as well.
inline std::wstring utf8to16_s2ws(const std::string& str);

//! Doesn't use winapi (but note, wchar_t isn't recommended at all). Encoding is unchecked.
inline std::wstring utf8to16_s2ws_portable(const std::string& str);

#if !TOO_HAS_NO_CODECVT
//! Implementation using C++11 codecvt. Cf. utf16to8_ws2s_portable().
inline std::string utf16to8_ws2s_codecvt(const std::wstring& wstr);

//! Implementation using C++11 codecvt. Cf. utf8to16_s2ws_portable().
inline std::wstring utf8to16_s2ws_codecvt(const std::string& str);
#endif


//! DEPRECATED!
/** You probably want to use sth. more precise with you encodings.*/
inline std::wstring locenc_s2ws(const std::string& s);


//! REMOVED! Keeping UTF-8. Use with caution: I still have a hard time imagining the usefulness of a correct utf8-wstring.
//! Removed because of that. I don't think there is any application.
//! inline std::string utf8_ws2s(const std::wstring& wstr);
//! REMOVED! Keeping UTF-8. Cf. utf8_s2ws().
//! inline std::wstring utf8_s2ws(const std::string& str);

//!@}

//! Helper function, e.g. for dealing with utf8::iterator in combination with std::string (nothing like a 'const_iterator' supported there).
inline char* s2psz(const std::string& s)
{
    const char* tmp(s.c_str());
    char* s_ = const_cast<char*>(tmp);
    return s_;
}

//! Converts \param s containing utf8 stuff like "\xc3\xa4" to the same string regarding ASCII characters but replacements
//! like "&#228;" for non-ASCII. Throws if \param s is not valid UTF8.
inline std::string utf8toHTML(const std::string& s)
{
    std::string ret;
    ret.reserve(s.size());
    char* s_ = s2psz(s);    
	utf8::iterator<char*> itend(s_ + s.size(), s_, s_ + s.size());
	for (utf8::iterator<char*> it(s_, s_, s_ + s.size()); it != itend; ++it)
    {
        if (*it == 0x9 || *it == 0xa || *it == 0xd || (*it >= 0x20 && *it <= 0x7e))
            ret+= it.base()[0];
        else
            ret+= "&#" + std::to_string(*it) + ";";
    }
    return ret;
}

}
}


//####################################################################################################################
// implementation details

#include "detail/str_convert_detail.hpp"

#endif
