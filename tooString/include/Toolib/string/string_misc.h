// Markus Borris, 2011
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file


#ifndef STRING_MISC_H_INCL_ciubhwduibgwi37
#define STRING_MISC_H_INCL_ciubhwduibgwi37

#include <string>
#include <ctype.h>
#include <algorithm>

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
}
} // too

#endif
