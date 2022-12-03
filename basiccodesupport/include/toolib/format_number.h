// 2019

//!
/**
 */
//! \file

#ifndef FORMAT_NUMBER_H_sdkihfx378gh87234xgh33h3
#define FORMAT_NUMBER_H_sdkihfx378gh87234xgh33h3

#include <locale>
#include <sstream>
#include <string>


namespace too::fmt
{
namespace detail
{
class GroupThousands_numpunct : public std::numpunct<char>
{
public:
    explicit GroupThousands_numpunct(char sep) noexcept
        : sep_{sep}
    {
    }

protected:
    char do_thousands_sep() const noexcept override
    {
        return sep_;
    }

    std::string do_grouping() const noexcept override
    {
        return "\03";
    }

private:
    char sep_{};
};
} // namespace detail

//! Strange, crashes for non-Linux?!
template <typename T>
inline std::string groupThousands(T t, char sep = '\'')
{
    std::stringstream ss;
    ss.imbue(std::locale(std::locale::classic(), new detail::GroupThousands_numpunct{sep}));
    ss << t;
    return ss.str();
}
} // namespace too::fmt
#endif
