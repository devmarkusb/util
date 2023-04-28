//! \file

#ifndef STRING_TOKEN_H_uwireiwubefeubf
#define STRING_TOKEN_H_uwireiwubefeubf

#include <string>
#include <vector>

namespace mb::ul::str {
inline void tokenizeString(const std::string& s, const std::string& delimiters, std::vector<std::string>& out) {
    size_t pos_start = s.find_first_not_of(delimiters);
    size_t pos_end = s.find_first_of(delimiters, pos_start);
    while (pos_start != std::string::npos) {
        out.push_back(s.substr(pos_start, pos_end - pos_start));
        pos_start = s.find_first_not_of(delimiters, pos_end);
        pos_end = s.find_first_of(delimiters, pos_start);
    }
}
} // namespace mb::ul::str

#endif
