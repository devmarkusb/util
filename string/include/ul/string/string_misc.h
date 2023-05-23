//! \file


#ifndef STRING_MISC_H_CIUBHWDUIBGWI37
#define STRING_MISC_H_CIUBHWDUIBGWI37

#include "ulstring.h"
#include "ul/assert.h"
#include "ul/narrow.h"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace mb::ul::str {
inline std::string make_upper(std::string& s) {
    std::transform(s.begin(), s.end(), s.begin(), [](char c) {
        return ul::narrow_cast<char>(std::toupper(c));
    });
    return s;
}

inline char make_upper(char& c) {
    c = ul::narrow_cast<char>(std::toupper(c));
    return c;
}

inline std::string make_lower(std::string& s) {
    std::transform(s.begin(), s.end(), s.begin(), [](char c) {
        return ul::narrow_cast<char>(std::tolower(c));
    });
    return s;
}

inline char make_lower(char& c) {
    c = ul::narrow_cast<char>(std::tolower(c));
    return c;
}

//! Expects fromSub non-empty.
inline void replace_all(std::string& s, const std::string& from_sub, const std::string& to_sub) {
    UL_EXPECT(!from_sub.empty());
    size_t start_pos{};
    while ((start_pos = s.find(from_sub, start_pos)) != std::string::npos) {
        s.replace(start_pos, from_sub.length(), to_sub);
        start_pos += to_sub.length();
    }
}

/** Expects fromSub non-empty. Expects toSubs to be of at least the
    count of occurrences of fromSub within s.*/
inline void replace_all(std::string& s, const std::string& from_sub, const std::vector<std::string>& to_subs) {
    UL_EXPECT(!from_sub.empty());
    if (s.empty()) {
        return;
    }
    size_t counter{};
    size_t start_pos{};
    while ((start_pos = s.find(from_sub, start_pos)) != std::string::npos) {
        UL_ASSERT(to_subs.size() > counter);
        s.replace(start_pos, from_sub.length(), to_subs[counter]);
        start_pos += to_subs[counter].length();
        ++counter;
    }
}

struct BoolStr10 {
    static std::string to_str(bool b) {
        return b ? "1" : "0";
    }

    static bool to_bool(const std::string& s) {
        return s == "1";
    }
};

struct BoolStrBoolalpha {
    static std::string to_str(bool b) {
        std::stringstream ss;
        ss << std::boolalpha << b;
        return ss.str();
    }

    static bool to_bool(const std::string& s) {
        std::stringstream ss;
        ss << std::boolalpha << true;
        return s == ss.str();
    }
};

template <class BoolStrFormat = BoolStr10>
inline std::string bool2str(bool b) {
    return BoolStrFormat::to_str(b);
}

template <class BoolStrFormat = BoolStr10>
inline bool str2bool(const std::string& s) {
    return BoolStrFormat::to_bool(s);
}

//! For text lengths greater than textellipse_threshold the text is abbreviated by an
//! ellipse '...'. No ellipse is ever used if the param is 0. Expects in_str to be utf8.*/
inline std::string apply_ellipse(const std::string& in_str, size_t ellipse_threshold) {
    if (!ellipse_threshold)
        return in_str;
    if (static_cast<size_t>(utf8::distance(std::begin(in_str), std::end(in_str))) <= ellipse_threshold)
        return in_str;
    auto it = std::begin(in_str);
    utf8::unchecked::advance(it, ellipse_threshold);
    std::string ret{in_str.substr(0, static_cast<size_t>(std::distance(std::begin(in_str), it)))};
    ret.append("...");
    return ret;
}

inline bool ends_with(const std::string& s, const std::string& ending) {
    if (ending.size() > s.size())
        return false;
    return std::equal(ending.rbegin(), ending.rend(), s.rbegin());
}
} // namespace mb::ul::str

#endif
