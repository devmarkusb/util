// 2011-22

//! \file

#ifndef ERROR_H_cosndzcriwu4ezr3
#define ERROR_H_cosndzcriwu4ezr3

#include "ul/assert.h"
#include "ul/std/std_extensions.h"
#include "ul/warnings.h"
#include <exception>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>

namespace mb::ul
{
//! Use these constants to exit your program by \code return \endcode out of \code main \endcode.
/** (Do not use \code std::exit \endcode, because of possible circumvention of stack unwinding.)*/
const int prog_exit_success = 0;
const int prog_exit_failure = 1;

//! An error code return type for functions/methods for interfaces that shall be non-throwing.
/** Typical usage with the convenience alias \code retcode \endcode:
    \code
    retcode f() noexcept;
    // use:
    retcode rc = f();
    if (!rc)
        throw module_xy_exception("error code " << rc << " returned");
    //...
    // or
    if (!!rc) // or: if (is_ok(rc)) // unfortunately it's not yet possible to write it simple
    {
        //...
    }
    \endcode
    In contrast to HRESULT&Co this doesn't mix up true, false with success or failure. If you
    need a further return value indicating true/false, you should write
    \code
    std::pair<ul::retcode, bool> f() noexcept;
    // use:
    ul::retcode ok();
    bool isSth = false;
    std::tie(ok, isSth) = f();
    if (ok)
    {
        if (!!isSth) // or: if (is_ok(isSth)) // unfortunately it's not yet possible to write it simple
        {
            //...
        }
    }
    \endcode
*/
enum class retcode
{
    //! Return values indicating success
    //!@{
    none = 0,
    success = none,
    //!@}

    //! Return values indicating failure
    //!@{
    failed,

    not_implemented,
    did_no_op,

    logic_error,
    domain_error,
    invalid_arg,
    length_error,
    out_of_range,
    runtime_error,
    overflow,
    underflow,
    range_error,

    precond_failed,
    assertion_failed,
    postcond_failed,

    would_crash,
    out_of_memory,
    division_by_zero,

    unknown_error,
    //!@}
};

inline std::string retcode_str(retcode rc)
{
    // clang-format off
    static const std::map<retcode, std::string> inst = {
        {retcode::success, "success"},
        {retcode::failed, "failed"},
        {retcode::not_implemented, "not_implemented"},
        {retcode::did_no_op, "did_no_op"},
        {retcode::logic_error, "logic_error"},
        {retcode::domain_error, "domain_error"},
        {retcode::invalid_arg, "invalid_arg"},
        {retcode::length_error, "length_error"},
        {retcode::out_of_range, "out_of_range"},
        {retcode::runtime_error, "runtime_error"},
        {retcode::overflow, "overflow"},
        {retcode::underflow, "underflow"},
        {retcode::range_error, "range_error"},
        {retcode::precond_failed, "precond_failed"},
        {retcode::assertion_failed, "assertion_failed"},
        {retcode::postcond_failed, "postcond_failed"},
        {retcode::would_crash, "would_crash"},
        {retcode::out_of_memory, "out_of_memory"},
        {retcode::division_by_zero, "division_by_zero"},
        {retcode::unknown_error, "unknown error"},
    };
    // clang-format on
    return inst.at(rc);
}

//(!) With that you can write \code retcode rc = f(); if(rc) y(); if(!rc) n(); \endcode
// inline explicit operator bool(retcode rc)
//{
//    return rc == retcode::none ? true : false;
//}
//! With that you can write \code retcode rc = f(); if(!rc) n(); \endcode
inline auto operator!(retcode rc) -> bool
{
    return rc != retcode::none;
}

namespace publish__is_ok
{
//! With that you can write \code retcode rc = f(); if(is_ok(rc)) y(); \endcode
inline bool is_ok(retcode rc)
{
    return !operator!(rc);
}

//! Equivalent for convenience.
inline bool is_ok(const std::pair<retcode, std::string>& rc_str)
{
    return is_ok(rc_str.first);
}
} // namespace publish__is_ok

/** Definition of some exceptions for the pendant of throwing functions, not returning
    \code retcode \endcode. Note that the standard library already defines most of the
    exceptions analogous to the \code retcode \endcode values.
    Also note that \code precond_failed, assertion_failed, postcond_failed\endcode
    are handled by the \code UL_EXPECT, UL_ASSERT, UL_ENSURE \endcode macros, resulting
    in throwing \code ul::fail_fast \endcode, cf. assert.h.*/
//!@{
struct would_crash : public std::runtime_error
{
    explicit would_crash(const char* const message)
        : std::runtime_error(message)
    {
    }
};

struct division_by_zero : public std::runtime_error
{
    explicit division_by_zero(const char* const message)
        : std::runtime_error(message)
    {
    }
};

struct not_implemented : public std::runtime_error
{
    explicit not_implemented(const char* const message)
        : std::runtime_error(message)
    {
    }
};

struct did_no_op : public std::runtime_error
{
    explicit did_no_op(const char* const message)
        : std::runtime_error(message)
    {
    }
};

struct time_out : public std::runtime_error
{
    explicit time_out(const char* const message)
        : std::runtime_error(message)
    {
    }
};

//!@}


/** At certain interface boundaries it could be desirable to not throw anything, but
    return an error code. This function conveniently wraps this as general concept.
    Usage: best with a lambda
    \code
    std::pair<ul::retcode, std::string> myfunction() noexcept
    {
        return ul::call_noexcept([&](){
            // arbitrary code that is allowed to throw anything
        });
    }
    \endcode
*/
template <typename Callable, typename Callable2>
std::pair<retcode, std::string> call_noexcept(
    Callable&& f, Callable2&& bad_alloc_handler =
                      []()
                  {
                  }) noexcept
{
    try
    {
        f();
        return std::make_pair(retcode::success, std::string());
    }
    catch (const ul::fail_fast& e)
    {
        return std::make_pair(retcode::assertion_failed, std::string(e.what()));
    }
    catch (const ul::would_crash& e)
    {
        return std::make_pair(retcode::would_crash, std::string(e.what()));
    }
    catch (const ul::division_by_zero& e)
    {
        return std::make_pair(retcode::division_by_zero, std::string(e.what()));
    }
    catch (const ul::not_implemented& e)
    {
        return std::make_pair(retcode::not_implemented, std::string(e.what()));
    }
    catch (const ul::did_no_op& e)
    {
        return std::make_pair(retcode::did_no_op, std::string(e.what()));
    }
    catch (const std::domain_error& e)
    {
        return std::make_pair(retcode::domain_error, std::string(e.what()));
    }
    catch (const std::invalid_argument& e)
    {
        return std::make_pair(retcode::invalid_arg, std::string(e.what()));
    }
    catch (const std::length_error& e)
    {
        return std::make_pair(retcode::length_error, std::string(e.what()));
    }
    catch (const std::out_of_range& e)
    {
        return std::make_pair(retcode::out_of_range, std::string(e.what()));
    }
    catch (const std::overflow_error& e)
    {
        return std::make_pair(retcode::overflow, std::string(e.what()));
    }
    catch (const std::underflow_error& e)
    {
        return std::make_pair(retcode::underflow, std::string(e.what()));
    }
    catch (const std::range_error& e)
    {
        return std::make_pair(retcode::range_error, std::string(e.what()));
    }
    catch (const std::logic_error& e)
    {
        return std::make_pair(retcode::logic_error, std::string(e.what()));
    }
    catch (const std::runtime_error& e)
    {
        return std::make_pair(retcode::runtime_error, std::string(e.what()));
    }
    catch (const std::bad_alloc& e)
    {
        bad_alloc_handler();
        return std::make_pair(retcode::out_of_memory, std::string(e.what()));
    }
    catch (const std::exception& e)
    {
        return std::make_pair(retcode::failed, std::string(e.what()));
    }
    catch (...)
    {
        return std::make_pair(retcode::unknown_error, std::string{});
    }
}
} // namespace mb::ul

using retcode = mb::ul::retcode;
UL_PRAGMA_WARNINGS_PUSH
// clang-format off
UL_WARNING_DISABLE_CLANG(header-hygiene)
// clang-format on
// since, `const auto& is_ok = ul::is_ok;` would lead to multiple defs, even with constexpr
using namespace mb::ul::publish__is_ok;
UL_PRAGMA_WARNINGS_POP

#endif
