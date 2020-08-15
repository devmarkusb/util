// Markus Borris, 2011-2017
// This file is part of tfl library.

//!
/**
 */
//! \file

#ifndef STR_CONVERT_H_827rt82rtrxjr38t
#define STR_CONVERT_H_827rt82rtrxjr38t

//! toostring.h included for the major conversion support; whether used in this file or not.
//! Use utf8::... for ultimate utf8 support with almost arbitrary string types.
#include "toolib/comp_bwds.h"
#include "toolib/string/toostring.h"
#include <stdexcept>
#include <string>

#include "toolib/macros.h"


namespace too
{
namespace str
{
struct conversion_error : public std::runtime_error
{
    explicit conversion_error(char const* const message)
        : std::runtime_error(message)
    {
    }
};

//! Whenever a std::wstring crosses your path (god forbid!).
//! Note that the *_portable versions are recommended over their pendants. They are more future proof.
//! And of course the *_codecvt are in direct competition.
//!@{

//! Uses winapi. Encoding is unchecked. Calls utf16or32to8_ws2s_portable() for non-Windows, which could be used for
//! Windows as well.
inline std::string utf16to8_ws2s(const std::wstring& wstr);

//! Doesn't use winapi (but note, wchar_t isn't recommended at all). Encoding is unchecked.
inline std::string utf16or32to8_ws2s_portable(const std::wstring& wstr);

//! Uses winapi. Encoding is unchecked. Calls utf16or32to8_ws2s_portable() for non-Windows, which could be used for
//! Windows
//! as well.
inline std::wstring utf8to16_s2ws(const std::string& str);

//! Doesn't use winapi (but note, wchar_t isn't recommended at all). Encoding is unchecked.
inline std::wstring utf8to16or32_s2ws_portable(const std::string& str);

#if !TOO_HAS_NO_CODECVT
//! Implementation using C++11 codecvt. Cf. utf16or32to8_ws2s_portable().
inline std::string utf16to8_ws2s_codecvt(const std::wstring& wstr);

//! Implementation using C++11 codecvt. Cf. utf8to16or32_s2ws_portable().
inline std::wstring utf8to16_s2ws_codecvt(const std::string& str);
#endif


//! DEPRECATED!
/** You probably want to use sth. more precise for your encodings.*/
inline std::wstring locenc_s2ws(const std::string& s);


//! REMOVED! Keeping UTF-8. Use with caution: I still have a hard time imagining the usefulness of a correct
//! utf8-wstring.
//! Removed because of that. I don't think there is any application.
//! inline std::string utf8_ws2s(const std::wstring& wstr);
//! REMOVED! Keeping UTF-8. Cf. utf8_s2ws().
//! inline std::wstring utf8_s2ws(const std::string& str);

//!@}

//! Helper function, e.g. for dealing with utf8::iterator in combination with std::string (nothing like a
//! 'const_iterator' supported there).
inline char* s2psz(const std::string& s)
{
    const char* tmp(s.c_str());
    char* s_ = const_cast<char*>(tmp);
    return s_;
}

//! Converts s containing utf8 stuff like "\xc3\xa4" to the same string regarding ASCII characters but
//! replacements
//! like "&#228;" for non-ASCII. Throws if s is not valid UTF8.
inline std::string utf8toHTML(const std::string& s)
{
    std::string ret;
    ret.reserve(s.size());
    char* s_ = s2psz(s);
    utf8::iterator<char*> itend(s_ + s.size(), s_, s_ + s.size());
    for (utf8::iterator<char*> it(s_, s_, s_ + s.size()); it != itend; ++it)
    {
        if (*it == 0x9 || *it == 0xa || *it == 0xd || (*it >= 0x20 && *it <= 0x7e))
            ret += it.base()[0];
        else
            ret += "&#" + too::to_string(*it) + ";";
    }
    return ret;
}

struct ThrowOnConversionError
{
    static char onConversionError(unsigned int codepoint)
    {
        std::string s{"conversion error on codepoint "};
        s += too::to_string(codepoint);
        throw conversion_error(s.c_str());
    }
};
struct ConversionErrorToQuestionMark
{
    static char onConversionError(unsigned int)
    {
        return '?';
    }
};

//! Latin1 means ISO-8859-1.
template <class OnConversionErrorPolicy = ConversionErrorToQuestionMark>
inline std::string utf8_to_latin1(const std::string& s);
inline std::string latin1_to_utf8(const std::string& s);

template <class OnConversionErrorPolicy = ConversionErrorToQuestionMark>
inline std::string utf8_to_printableASCII(const std::string& s);
inline std::string printableASCII_to_utf8(const std::string& s);


inline std::string toHexString(const std::string& s, const std::string& prefix = {});
} // namespace str
} // namespace too


//####################################################################################################################
// implementation details

#include "detail/str_convert_detail.hpp"

#include "toolib/macros_end.h"

#endif
