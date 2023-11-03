//! \file

#ifndef FORMAT_NUMBER_H_SDKIHFX378GH87234XGH33H3
#define FORMAT_NUMBER_H_SDKIHFX378GH87234XGH33H3

#include "ul/macros.h"
#include <locale>
#include <sstream>
#include <string>

namespace mb::ul::fmt {
namespace detail {
class GroupThousandsNumpunct : public std::numpunct<char> {
public:
    explicit GroupThousandsNumpunct(char sep) noexcept
        : sep_{sep} {
    }

protected:
    char do_thousands_sep() const noexcept override {
        return sep_;
    }

    std::string do_grouping() const noexcept override {
        return "\03";
    }

private:
    char sep_{};
};
} // namespace detail

//! Strange, crashes for non-Linux?!
template <typename T>
inline std::string group_thousands(T t, [[maybe_unused]] char sep = '\'') {
    std::stringstream ss;
#if UL_HAS_NOCRASH_IMBUE_LOCALE
    ss.imbue(std::locale(std::locale::classic(), new detail::GroupThousandsNumpunct{sep}));
#endif
    ss << t;
    return ss.str();
}
} // namespace mb::ul::fmt
#endif
