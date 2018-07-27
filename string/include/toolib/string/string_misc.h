// Markus Borris, 2011-17
// This file is part of tfl library.

//!
/**
*/
//! \file


#ifndef STRING_MISC_H_INCL_ciubhwduibgwi37
#define STRING_MISC_H_INCL_ciubhwduibgwi37

#include "toolib/assert.h"
#include "toolib/narrow.h"
#include "toolib/string/toostring.h"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>


namespace too
{
namespace str
{
inline std::string makeUpper(std::string& s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](char c) { return too::narrow_cast<char>(std::toupper(c)); });
    return s;
}

inline char makeUpper(char& c)
{
    c = too::narrow_cast<char>(std::toupper(c));
    return c;
}

inline std::string makeLower(std::string& s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](char c) { return too::narrow_cast<char>(std::tolower(c)); });
    return s;
}

inline char makeLower(char& c)
{
    c = too::narrow_cast<char>(std::tolower(c));
    return c;
}

//! Expects \param fromSub non-empty.
inline void replaceAll(std::string& s, const std::string& fromSub, const std::string& toSub)
{
    TOO_EXPECT(!fromSub.empty());
    size_t start_pos{};
    while ((start_pos = s.find(fromSub, start_pos)) != std::string::npos)
    {
        s.replace(start_pos, fromSub.length(), toSub);
        start_pos += toSub.length();
    }
}

//! Expects \param fromSub non-empty. Expects \params toSubs to be of at least the
//! count of occurrences of \param fromSub within \param s.
inline void replaceAll(std::string& s, const std::string& fromSub, const std::vector<std::string>& toSubs)
{
    TOO_EXPECT(!fromSub.empty());
    size_t counter{};
    size_t start_pos{};
    while ((start_pos = s.find(fromSub, start_pos)) != std::string::npos)
    {
        TOO_ASSERT(toSubs.size() > counter);
        s.replace(start_pos, fromSub.length(), toSubs[counter]);
        start_pos += toSubs[counter].length();
        ++counter;
    }
}

struct BoolStr_1_0
{
    static std::string toStr(bool b) { return b ? "1" : "0"; }
    static bool toBool(const std::string& s) { return s == "1" ? true : false; }
};

struct BoolStr_boolalpha
{
    static std::string toStr(bool b)
    {
        std::stringstream ss;
        ss << std::boolalpha << b;
        return ss.str();
    }
    static bool toBool(const std::string& s)
    {
        std::stringstream ss;
        ss << std::boolalpha << true;
        return s == ss.str() ? true : false;
    }
};

template <class BoolStrFormat = BoolStr_1_0>
inline std::string bool2str(bool b)
{
    return BoolStrFormat::toStr(b);
}

template <class BoolStrFormat = BoolStr_1_0>
inline bool str2bool(const std::string& s)
{
    return BoolStrFormat::toBool(s);
}

//! For text lengths greater than \param textellipse_threshold the text is abbreviated by an
//! ellipse '...'. No ellipse is ever used if the param is 0. Expects \param in_str to be utf8.*/
inline std::string apply_ellipse(const std::string& in_str, size_t ellipse_threshold)
{
    if (!ellipse_threshold)
        return in_str;
    if (static_cast<size_t>(utf8::distance(std::begin(in_str), std::end(in_str))) <= ellipse_threshold)
        return in_str;
    auto it = std::begin(in_str);
    utf8::unchecked::advance(it, ellipse_threshold);
    std::string ret{in_str.substr(0, std::distance(std::begin(in_str), it))};
    ret.append("...");
    return ret;
}

inline bool ends_with(const std::string& s, const std::string& ending)
{
    if (ending.size() > s.size())
        return false;
    return std::equal(ending.rbegin(), ending.rend(), s.rbegin());
}
} // str
} // too

#endif
