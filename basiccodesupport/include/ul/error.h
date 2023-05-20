//! \file

#ifndef ERROR_H_COSNDZCRIWU4EZR3
#define ERROR_H_COSNDZCRIWU4EZR3

#include "assert.h"
#include "std/std_extensions.h"

#include "ul/warnings.h"
#include <exception>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>

namespace mb::ul {
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
enum class Retcode {
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

inline std::string retcode_str(Retcode rc) {
    // clang-format off
    static const std::map<Retcode, std::string> inst = {
        {Retcode::success, "success"},
        {Retcode::failed, "failed"},
        {Retcode::not_implemented, "not_implemented"},
        {Retcode::did_no_op, "did_no_op"},
        {Retcode::logic_error, "logic_error"},
        {Retcode::domain_error, "domain_error"},
        {Retcode::invalid_arg, "invalid_arg"},
        {Retcode::length_error, "length_error"},
        {Retcode::out_of_range, "out_of_range"},
        {Retcode::runtime_error, "runtime_error"},
        {Retcode::overflow, "overflow"},
        {Retcode::underflow, "underflow"},
        {Retcode::range_error, "range_error"},
        {Retcode::precond_failed, "precond_failed"},
        {Retcode::assertion_failed, "assertion_failed"},
        {Retcode::postcond_failed, "postcond_failed"},
        {Retcode::would_crash, "would_crash"},
        {Retcode::out_of_memory, "out_of_memory"},
        {Retcode::division_by_zero, "division_by_zero"},
        {Retcode::unknown_error, "unknown error"},
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
inline auto operator!(Retcode rc) -> bool {
    return rc != Retcode::none;
}

namespace publish_is_ok {
//! With that you can write \code retcode rc = f(); if(is_ok(rc)) y(); \endcode
inline bool is_ok(Retcode rc) {
    return !operator!(rc);
}

//! Equivalent for convenience.
inline bool is_ok(const std::pair<Retcode, std::string>& rc_str) {
    return is_ok(rc_str.first);
}
} // namespace publish_is_ok

/** Definition of some exceptions for the pendant of throwing functions, not returning
    \code retcode \endcode. Note that the standard library already defines most of the
    exceptions analogous to the \code retcode \endcode values.
    Also note that \code precond_failed, assertion_failed, postcond_failed\endcode
    are handled by the \code UL_EXPECT, UL_ASSERT, UL_ENSURE \endcode macros, resulting
    in throwing \code ul::fail_fast \endcode, cf. assert.h.*/
//!@{
struct WouldCrash : public std::runtime_error {
    explicit WouldCrash(const char* const message)
        : std::runtime_error(message) {
    }
};

struct DivisionByZero : public std::runtime_error {
    explicit DivisionByZero(const char* const message)
        : std::runtime_error(message) {
    }
};

struct NotImplemented : public std::runtime_error {
    explicit NotImplemented(const char* const message)
        : std::runtime_error(message) {
    }
};

struct DidNoOp : public std::runtime_error {
    explicit DidNoOp(const char* const message)
        : std::runtime_error(message) {
    }
};

struct TimeOut : public std::runtime_error {
    explicit TimeOut(const char* const message)
        : std::runtime_error(message) {
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
std::pair<Retcode, std::string> call_noexcept(
    Callable&& f, Callable2&& bad_alloc_handler = []() {
    }) noexcept {
    try {
        f();
        return std::make_pair(Retcode::success, std::string());
    } catch (const ul::FailFast& e) {
        return std::make_pair(Retcode::assertion_failed, std::string(e.what()));
    } catch (const ul::WouldCrash& e) {
        return std::make_pair(Retcode::would_crash, std::string(e.what()));
    } catch (const ul::DivisionByZero& e) {
        return std::make_pair(Retcode::division_by_zero, std::string(e.what()));
    } catch (const ul::NotImplemented& e) {
        return std::make_pair(Retcode::not_implemented, std::string(e.what()));
    } catch (const ul::DidNoOp& e) {
        return std::make_pair(Retcode::did_no_op, std::string(e.what()));
    } catch (const std::domain_error& e) {
        return std::make_pair(Retcode::domain_error, std::string(e.what()));
    } catch (const std::invalid_argument& e) {
        return std::make_pair(Retcode::invalid_arg, std::string(e.what()));
    } catch (const std::length_error& e) {
        return std::make_pair(Retcode::length_error, std::string(e.what()));
    } catch (const std::out_of_range& e) {
        return std::make_pair(Retcode::out_of_range, std::string(e.what()));
    } catch (const std::overflow_error& e) {
        return std::make_pair(Retcode::overflow, std::string(e.what()));
    } catch (const std::underflow_error& e) {
        return std::make_pair(Retcode::underflow, std::string(e.what()));
    } catch (const std::range_error& e) {
        return std::make_pair(Retcode::range_error, std::string(e.what()));
    } catch (const std::logic_error& e) {
        return std::make_pair(Retcode::logic_error, std::string(e.what()));
    } catch (const std::runtime_error& e) {
        return std::make_pair(Retcode::runtime_error, std::string(e.what()));
    } catch (const std::bad_alloc& e) {
        bad_alloc_handler();
        return std::make_pair(Retcode::out_of_memory, std::string(e.what()));
    } catch (const std::exception& e) {
        return std::make_pair(Retcode::failed, std::string(e.what()));
    } catch (...) {
        return std::make_pair(Retcode::unknown_error, std::string{});
    }
}
} // namespace mb::ul

using retcode = mb::ul::Retcode;
UL_PRAGMA_WARNINGS_PUSH
// clang-format off
UL_WARNING_DISABLE_CLANG(header-hygiene)
// clang-format on
// since, `const auto& is_ok = ul::is_ok;` would lead to multiple defs, even with constexpr
using namespace mb::ul::publish_is_ok; // NOLINT
UL_PRAGMA_WARNINGS_POP

#endif
