//! \file

#ifndef STRING_TRIM_H_cidusbfi243i7324
#define STRING_TRIM_H_cidusbfi243i7324

#include <string>

namespace mb::ul::str {
inline std::string& ltrim(std::string& s, const std::string& trimchars = " \t\n") {
    // Following suggestion from Evan Teran, stackoverflow doesn't compile.
    // s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    s.erase(0, s.find_first_not_of(trimchars));
    return s;
}

inline std::string& rtrim(std::string& s, const std::string& trimchars = " \t\n") {
    // s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    s.erase(s.find_last_not_of(trimchars) + 1);
    return s;
}

inline std::string& trim(std::string& s) {
    ltrim(rtrim(s));
    return s;
}
} // namespace mb::ul::str

#endif
