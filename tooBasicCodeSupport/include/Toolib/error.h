// Markus Borris, 2011
// This file is part of Toolib library.

//!
/**
*/
//! \file

#ifndef ERROR_H_INCL_cosndzcriwu4ezr3
#define ERROR_H_INCL_cosndzcriwu4ezr3

#include <exception>
#include <stdexcept>
#include <map>
#include <string>


namespace too
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
    std::pair<too::retcode, bool> f() noexcept;
    // use:
    too::retcode ok();
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
    none    = 0,
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
    //!@}
};

const std::map<retcode, std::string> retcode_str ={
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
    {retcode::would_crash, "would_crash"}
};

//(!) With that you can write \code retcode rc = f(); if(rc) y(); if(!rc) n(); \endcode
// inline explicit operator bool(retcode rc)
//{
//    return rc == retcode::none ? true : false;
//}
//! With that you can write \code retcode rc = f(); if(!rc) n(); \endcode
inline auto operator!(retcode rc) -> const bool
{
    return rc == retcode::none ? false : true;
}

namespace publish__is_ok
{
//! With that you can write \code retcode rc = f(); if(is_ok(rc)) y(); \endcode
inline bool is_ok(retcode rc)
{
    return !operator!(rc);
}
}


//! Definition of some exceptions for the pendant of throwing functions, not returning
//! \code retcode \endcode. Note that the standard library already defines most of the
//! exceptions analogous to the \code retcode \endcode values.
//! Also note that \code precond_failed, assertion_failed, postcond_failed\endcode
//! are handled by the \code TOO_EXPECT, TOO_ASSERT, TOO_ENSURE \endcode macros, resulting
//! in throwing \code too::fail_fast \endcode, cf. assert.h.
//!@{
struct would_crash : public std::runtime_error
{
    explicit would_crash(char const* const message) : std::runtime_error(message) {}
};
struct not_implemented : public std::runtime_error
{
    explicit not_implemented(char const* const message) : std::runtime_error(message) {}
};
struct did_no_op : public std::runtime_error
{
    explicit did_no_op(char const* const message) : std::runtime_error(message) {}
};
//!@}
}

using retcode = too::retcode;
// since, `const auto& is_ok = too::is_ok;` would lead to multiple defs, even with constexpr
using namespace too::publish__is_ok;

#endif
