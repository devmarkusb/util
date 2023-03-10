//! \file

#ifndef LOCALE_H_condwhxnu23mhy8mr
#define LOCALE_H_condwhxnu23mhy8mr

#include "ul/macros.h"
#include "ul/std/std_extensions.h"
#include <locale>

namespace mb::ul
{
enum class Global_locale
{
    default_classic,
    user_preferred,
};

//! Sets global locale.
/** \param locname "" means the user preferred locale, "C" the classic, minimal one, set by default
    on program start. Other options are e.g. "de", "de_DE", "de_DE.utf8". The preferred locale
    could e.g. be "German_Germany.1252"
    \return the old locale. On first call this is std::locale::classic.*/
inline std::locale set_global_locale(const std::string& locname)
{
    return std::locale::global(std::locale(locname.c_str()));
}

//! For convenience, cf. set_global_locale().
inline std::locale set_global_locale(Global_locale gl)
{
    std::string opt;
    switch (gl)
    {
        case Global_locale::user_preferred:
            opt = "";
            break;
        case Global_locale::default_classic: // fall through
        default:
            opt = "C";
            break;
    }
    return std::locale::global(std::locale(opt.c_str()));
}

//! For convenience, cf. set_global_locale().
inline std::locale set_global_locale(const std::locale& loc)
{
    return std::locale::global(loc);
}

//! This does an auto reset when leaving scope. For convenience, cf. set_global_locale().
struct set_global_locale_scoped
{
    explicit set_global_locale_scoped(const std::string& locname)
    {
        this->backup_ = set_global_locale(locname);
    }

    explicit set_global_locale_scoped(Global_locale gl)
    {
        this->backup_ = set_global_locale(gl);
    }

    ~set_global_locale_scoped() noexcept
    {
        try
        {
            set_global_locale(this->backup_);
        }
        catch (...)
        {
        }
    }

    set_global_locale_scoped(const set_global_locale_scoped&) = delete;
    set_global_locale_scoped& operator=(const set_global_locale_scoped&) = delete;
#if UL_HAS_CPP11_DEFAULT_MOVES
    set_global_locale_scoped(set_global_locale_scoped&&) = delete;
    set_global_locale_scoped& operator=(set_global_locale_scoped&&) = delete;
#endif

    [[nodiscard]] std::locale get_original_locale() const
    {
        return this->backup_;
    }

private:
    std::locale backup_;
};
} // namespace mb::ul

#endif
