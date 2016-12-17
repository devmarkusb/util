// Markus Borris, 2011-16
// This file is part of Toolib library.

//!
/**
*/
//! \file


#ifndef STRING_MISC_H_INCL_ciubhwduibgwi37
#define STRING_MISC_H_INCL_ciubhwduibgwi37

#include "Toolib/assert.h"
#include <algorithm>
#include <ctype.h>
#include <string>
#include <vector>


namespace too
{
namespace str
{
inline std::string& makeUpper(std::string& s)
{
    std::transform(s.begin(), s.end(), s.begin(), toupper);
    return s;
}

inline std::string& makeLower(std::string& s)
{
    std::transform(s.begin(), s.end(), s.begin(), tolower);
    return s;
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
} // str
} // too

#endif
