/**
\file
Note that, the following compiler defines are supported:
- UL_ASSERT_THROW_DISABLE for downgrading throwing 'asserts'
(that is UL_ASSERT_THROW, UL_EXPECT_THROW, UL_ENSURE_THROW) to only assert,
- UL_ASSERT_SLEEP_DISABLE for downgrading sleeping (spinning) 'asserts'
(that is the ones from above ending in _SLEEP instead of _THROW) to only assert,
- UL_ASSERT_TERMINATE_DISABLE for downgrading terminating 'asserts'
(that is the ones from above ending in _TERMINATE instead of _THROW) to only assert.

For diagnostic purposes you can also define
UL_ASSERT_ALWAYS_THROWING
for letting simple asserts (that is UL_ASSERT, UL_EXPECT, UL_ENSURE) throw.
So these are noticeable in release builds, too.
You get a compile hint for that, which can be turned off defining
UL_ASSERT_ALWAYS_THROWING__SUPPRESS_COMPILER_MESSAGE.
*/

#ifndef ASSERT_H_onvdr589tz3758ct438tzcn
#define ASSERT_H_onvdr589tz3758ct438tzcn

#include "ul/config.h"
#include "ul/macros.h"
#include "ul/warnings.h"
#ifndef UL_COMP_MS_VISUAL_STUDIO_CPP
#include <cassert>
#endif
#include <chrono>
#include <exception>
#include <stdexcept>
#include <thread>

namespace mb::ul
{
//! This is thrown by any throwing assertion.
struct fail_fast : public std::runtime_error
{
    explicit fail_fast(const char* const message)
        : std::runtime_error(message)
    {
    }
};
} // namespace mb::ul


//####################################################################################################################
// impl. details
/* Note that there is no way around macros here, because we need file and line information as well as
   complete vanishing for release or compile-define switched builds.*/

// Begin warning suppression, and don't end! There is no other chance, or do you know an alternative to do {...} while
// (false)?
UL_PRAGMA_WARNINGS_PUSH
UL_WARNING_DISABLE_MSVC(4127)

#if UL_COMP_MS_VISUAL_STUDIO_CPP
#if UL_DEBUG
#define UL_ASSERT_IMPL(cond) \
    do \
    { \
        if (!(cond)) \
            __debugbreak(); \
    } while (false)
#else
#define UL_ASSERT_IMPL(cond) UL_NOOP
#endif
#else
#define UL_ASSERT_IMPL(cond) assert(cond)
#endif

#ifdef UL_ASSERT_THROW_DISABLE
#define UL_ASSERT_THROW_IMPL(cond, textstart) UL_ASSERT_IMPL(cond)
#else
#define UL_ASSERT_THROW_IMPL(cond, textstart) \
    do \
    { \
        if (!(cond)) \
            throw ul::fail_fast(textstart " " __FILE__ ": " UL_STRINGIFY_VALUE(__LINE__)); \
    } while (false)

#ifdef UL_ASSERT_ALWAYS_THROWING
#undef UL_ASSERT_IMPL
#define UL_ASSERT_IMPL(cond) UL_ASSERT_THROW_IMPL(cond, "assertion failed at")
#ifndef UL_ASSERT_ALWAYS_THROWING__SUPPRESS_COMPILER_MESSAGE
#pragma message( \
    "Note that UL_ASSERT_ALWAYS_THROWING is defined, which leads to a diagnostic mode of letting every UL_ASSERTION throw.")
#endif
#endif
#endif

#ifdef UL_ASSERT_SLEEP_DISABLE
#define UL_ASSERT_SLEEP_IMPL(cond) UL_ASSERT_IMPL(cond)
#else
#define UL_ASSERT_SLEEP_IMPL(cond) \
    do \
    { \
        for (;;) \
        { \
            std::this_thread::sleep_for(std::chrono::milliseconds(1)); \
        } \
    } while (false)
#endif

#ifdef UL_ASSERT_TERMINATE_DISABLE
#define UL_ASSERT_TERMINATE_IMPL(cond) UL_ASSERT_IMPL(cond)
#else
#define UL_ASSERT_TERMINATE_IMPL(cond) \
    do \
    { \
        if (!(cond)) \
            std::terminate(); \
    } while (false)
#endif


//####################################################################################################################
//! Assertion helpers.
/** Proposal of usage guidelines (at the example of the UL_EXPECT* macros, ASSERTs and ENSUREs are analogous).
    1.	UL_EXPECT should be used all over the place without any further considerations. Question is, when to tighten
        that to the other variants.
    2.	UL_EXPECT_TERMINATE and UL_EXPECT_SLEEP need no further explanation. As soon as there is a possible occasion
        to use them you will just know that.
    3.	More interesting though is, when to use either of the following:
        a) UL_EXPECT_THROW(cond)
        b) if (!cond) return; // in this case of course paired with a preceding UL_EXPECT, telling about a real error
                              // case
        That's basically when you know your program would crash or show undefined behavior would it run further down
        the routine.
        Then you could choose b) if there was no harm in letting the error go unnoticed except the program 'just'
        missing some functionality.
        But in most cases you should opt for a), letting programmers and testers know that there is a logically wrong
        behavior happening. The exception ul::fail_fast can then be caught at any call stack level that thinks it
        knows how to handle that.
*/
//!@{

//! Simple assertion that a condition is met. Affecting debug builds only.
#define UL_ASSERT(cond) UL_ASSERT_IMPL(cond)
//! Letting thread sleep if condition is not met. (e.g. to break into the debugger at any time wanted). Release builds
//! are also affected.
#define UL_ASSERT_SLEEP(cond) UL_ASSERT_SLEEP_IMPL(cond)
//! Throw if condition is not met. Release builds are also affected.
#define UL_ASSERT_THROW(cond) UL_ASSERT_THROW_IMPL(cond, "assertion failed at")
//! Terminate if condition is not met. Release builds are also affected.
#define UL_ASSERT_TERMINATE(cond) UL_ASSERT_TERMINATE_IMPL(cond)

//! Like UL_ASSERT, but expressing precondition failure.
#define UL_EXPECT(cond) UL_ASSERT_IMPL(cond)
//! Like UL_ASSERT_SLEEP, but expressing precondition failure.
#define UL_EXPECT_SLEEP(cond) UL_ASSERT_SLEEP_IMPL(cond)
//! Like UL_ASSERT_THROW, but expressing precondition failure.
#define UL_EXPECT_THROW(cond) UL_ASSERT_THROW_IMPL(cond, "precondition failed at")
//! Like UL_ASSERT_TERMINATE, but expressing precondition failure.
#define UL_EXPECT_TERMINATE(cond) UL_ASSERT_TERMINATE_IMPL(cond)

//! Like UL_ASSERT, but expressing postcondition failure.
#define UL_ENSURE(cond) UL_ASSERT_IMPL(cond)
//! Like UL_ASSERT_SLEEP, but expressing postcondition failure.
#define UL_ENSURE_SLEEP(cond) UL_ASSERT_SLEEP_IMPL(cond)
//! Like UL_ASSERT_THROW, but expressing postcondition failure.
#define UL_ENSURE_THROW(cond) UL_ASSERT_THROW_IMPL(cond, "postcondition failed at")
//! Like UL_ASSERT_TERMINATE, but expressing postcondition failure.
#define UL_ENSURE_TERMINATE(cond) UL_ASSERT_TERMINATE_IMPL(cond)

//!@}

//! A verify macro for convenience. The passed condition expression will also be evaluated in release builds.
#if UL_DEBUG
#define UL_VERIFY(cond) UL_ASSERT(cond)
#else
// It was quite some try and error getting the tests run successfully, which means (cond) not getting optimized away
// in release build. The assignment to bool lately seemed to be the crucial point; volatile not even necessary.
// But hopefully chances are that real production code's functionality won't be cut off.
#define UL_VERIFY(cond) \
    do \
    { \
        volatile bool UL_DUMMY = (cond); \
        UL_DUMMY; \
    } while (false)
#endif

namespace mb::ul
{
#if UL_COMP_MS_VISUAL_STUDIO_CPP
static const char* const death_assert_regex{""};
#else
static const char* const death_assert_regex{"[Aa]ssert"};
#endif
} // namespace mb::ul

UL_HEADER_END

#endif
