// Markus Borris, 2011-17
// This file is part of Toolib library.

//!
/**
Note that, the following compiler defines are supported:
- TOO_ASSERT_THROW_DISABLE for downgrading throwing 'asserts'
(that is TOO_ASSERT_THROW, TOO_EXPECT_THROW, TOO_ENSURE_THROW) to only assert,
- TOO_ASSERT_SLEEP_DISABLE for downgrading sleeping (spinning) 'asserts'
(that is the ones from above ending in _SLEEP instead of _THROW) to only assert,
- TOO_ASSERT_TERMINATE_DISABLE for downgrading terminating 'asserts'
(that is the ones from above ending in _TERMINATE instead of _THROW) to only assert.

For diagnostic purposes you can also define
TOO_ASSERT_ALWAYS_THROWING
for letting simple asserts (that is TOO_ASSERT, TOO_EXPECT, TOO_ENSURE) throw.
So these are noticeable in release builds, too.
You get a compile hint for that, which can be turned off defining
TOO_ASSERT_ALWAYS_THROWING__SUPPRESS_COMPILER_MESSAGE.
*/
//! \file

#ifndef ASSERT_H_INCL_onvdr589tz3758ct438tzcn
#define ASSERT_H_INCL_onvdr589tz3758ct438tzcn

#include "Toolib/PPDEFS.h"
#ifndef TOO_COMP_MS_VISUAL_STUDIO_CPP
#include <assert.h>
#endif
#include <chrono>
#include <exception>
#include <stdexcept>
#include <thread>


namespace too
{
//! This is thrown by any throwing assertion.
struct fail_fast : public std::runtime_error
{
    explicit fail_fast(char const* const message) : std::runtime_error(message) {}
};
}


//####################################################################################################################
// impl. details
/*	Note that there is no way around macros here, because we need file and line informations as well as
    complete vanishing for release or compile-define switched builds.*/

// Begin warning suppression, and don't end! There is no other chance, or do you know an alternative to do {...} while
// (false)?
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNING_4127_BEGIN"

#if TOO_COMP_MS_VISUAL_STUDIO_CPP
#if TOO_DEBUG
#define TOO_ASSERT_IMPL(cond) \
    do                        \
    {                         \
        if (!(cond))          \
            __debugbreak();   \
    } while (false)
#else
#define TOO_ASSERT_IMPL(cond) TOO_NOOP
#endif
#else
#define TOO_ASSERT_IMPL(cond) assert(cond)
#endif

#ifdef TOO_ASSERT_THROW_DISABLE
#define TOO_ASSERT_THROW_IMPL(cond, textstart) TOO_ASSERT_IMPL(cond)
#else
#define TOO_ASSERT_THROW_IMPL(cond, textstart)                                         \
    do                                                                                 \
    {                                                                                  \
        if (!(cond))                                                                   \
            throw too::fail_fast(textstart " " __FILE__ ": " TOO_STRINGIFY(__LINE__)); \
    } while (false)

#ifdef TOO_ASSERT_ALWAYS_THROWING
#undef TOO_ASSERT_IMPL
#define TOO_ASSERT_IMPL(cond) TOO_ASSERT_THROW_IMPL(cond, "assertion failed at")
#ifndef TOO_ASSERT_ALWAYS_THROWING__SUPPRESS_COMPILER_MESSAGE
#pragma message( \
    "Note that TOO_ASSERT_ALWAYS_THROWING is defined, which leads to a diagnostic mode of letting every TOO_ASSERTION throw.")
#endif
#endif
#endif

#ifdef TOO_ASSERT_SLEEP_DISABLE
#define TOO_ASSERT_SLEEP_IMPL(cond) TOO_ASSERT_IMPL(cond)
#else
#define TOO_ASSERT_SLEEP_IMPL(cond)                                    \
    do                                                                 \
    {                                                                  \
        for (;;)                                                       \
        {                                                              \
            std::this_thread::sleep_for(std::chrono::milliseconds(1)); \
        }                                                              \
    } while (false)
#endif

#ifdef TOO_ASSERT_TERMINATE_DISABLE
#define TOO_ASSERT_TERMINATE_IMPL(cond) TOO_ASSERT_IMPL(cond)
#else
#define TOO_ASSERT_TERMINATE_IMPL(cond) \
    do                                  \
    {                                   \
        if (!(cond))                    \
            std::terminate();           \
    } while (false)
#endif


//####################################################################################################################
//! Assertion helpers.
/** Proposal of usage guidelines (at the example of the TOO_EXPECT* macros, ASSERTs and ENSUREs are analogous).
    1.	TOO_EXPECT should be used all over the place without any further considerations. Question is, when to tighten
        that to the other variants.
    2.	TOO_EXPECT_TERMINATE and TOO_EXPECT_SLEEP need no further explanation. As soon as there is a possible occasion
        to use them you will just know that.
    3.	More interesting though is, when to use either of the following:
        a) TOO_EXPECT_THROW(cond)
        b) if (!cond) return; // in this case of course paired with a preceding TOO_EXPECT, telling about a real error
                              // case
        That's basically when you know your program would crash or show undefined behavior would it run further down
        the routine.
        Then you could choose b) if there was no harm in letting the error go unnoticed except the program 'just'
        missing some functionality.
        But in most cases you should opt for a), letting programmers and testers know that there is a logically wrong
        behavior happening. The exception too::fail_fast can then be caught at any call stack level that thinks it
        knows how to handle that.
*/
//!@{

//! Simple assertion that a condition is met. Affecting debug builds only.
#define TOO_ASSERT(cond) TOO_ASSERT_IMPL(cond)
//! Letting thread sleep if condition is not met. (e.g. to break into the debugger at any time wanted). Release builds
//! are also affected.
#define TOO_ASSERT_SLEEP(cond) TOO_ASSERT_SLEEP_IMPL(cond)
//! Throw if condition is not met. Release builds are also affected.
#define TOO_ASSERT_THROW(cond) TOO_ASSERT_THROW_IMPL(cond, "assertion failed at")
//! Terminate if condition is not met. Release builds are also affected.
#define TOO_ASSERT_TERMINATE(cond) TOO_ASSERT_TERMINATE_IMPL(cond)

//! Like TOO_ASSERT, but expressing precondition failure.
#define TOO_EXPECT(cond) TOO_ASSERT_IMPL(cond)
//! Like TOO_ASSERT_SLEEP, but expressing precondition failure.
#define TOO_EXPECT_SLEEP(cond) TOO_ASSERT_SLEEP_IMPL(cond)
//! Like TOO_ASSERT_THROW, but expressing precondition failure.
#define TOO_EXPECT_THROW(cond) TOO_ASSERT_THROW_IMPL(cond, "precondition failed at")
//! Like TOO_ASSERT_TERMINATE, but expressing precondition failure.
#define TOO_EXPECT_TERMINATE(cond) TOO_ASSERT_TERMINATE_IMPL(cond)

//! Like TOO_ASSERT, but expressing postcondition failure.
#define TOO_ENSURE(cond) TOO_ASSERT_IMPL(cond)
//! Like TOO_ASSERT_SLEEP, but expressing postcondition failure.
#define TOO_ENSURE_SLEEP(cond) TOO_ASSERT_SLEEP_IMPL(cond)
//! Like TOO_ASSERT_THROW, but expressing postcondition failure.
#define TOO_ENSURE_THROW(cond) TOO_ASSERT_THROW_IMPL(cond, "postcondition failed at")
//! Like TOO_ASSERT_TERMINATE, but expressing postcondition failure.
#define TOO_ENSURE_TERMINATE(cond) TOO_ASSERT_TERMINATE_IMPL(cond)

//!@}

//! A verify macro for convenience. The passed condition expression will also be evaluated in release builds.
#if TOO_DEBUG
#define TOO_VERIFY(cond) TOO_ASSERT(cond)
#else
// It was quite some try and error getting the tests run successfully, which means (cond) not getting optimized away
// in release build. The assignment to bool lately seemed to be the crucial point; volatile not even necessary.
// But hopefully chances are that real production code's functionality won't be cut off.
#define TOO_VERIFY(cond)                   \
    do                                     \
    {                                      \
        volatile bool TOO_NONAME = (cond); \
        TOO_NONAME;                        \
    } while (false)
#endif


//####################################################################################################################

namespace too
{
//! Compile time assert.
/** Idea from October 1997 issue of C/C++ Users Journal.*/
template <bool t>
struct AssertCT
{
    enum
    {
        N = 1 - 2 * static_cast<int>(!t)
    };
    static char A[N];
};
template <bool t>
char AssertCT<t>::A[N];

//! Compile time assert.
/** Alexey Malistov, SO*/
template <bool>
struct StaticAssert_v0;
template <>
struct StaticAssert_v0<true>
{
};

//! Compile time assert.
#define TOO_STATIC_ASSERTv1_CONCAT_(a, b) a##b
#define TOO_STATIC_ASSERTv1_CONCAT(a, b) TOO_STATIC_ASSERTv1_CONCAT_(a, b)
#if TOO_COMP_MS_VISUAL_STUDIO_CPP && __COUNTER__
#define TOO_STATIC_ASSERTv1(t, msg)                                                    \
    {                                                                                  \
        enum                                                                           \
        {                                                                              \
            TOO_STATIC_ASSERTv1_CONCAT(msg, __COUNTER__) = 1 / static_cast<int>(!!(t)) \
        };                                                                             \
    }
#else
#define TOO_STATIC_ASSERTv1(t, msg)                                                 \
    {                                                                               \
        enum                                                                        \
        {                                                                           \
            TOO_STATIC_ASSERTv1_CONCAT(msg, __LINE__) = 1 / static_cast<int>(!!(t)) \
        };                                                                          \
    }
#endif

//! Compile time assert.
#define TOO_STATIC_ASSERTv2(t, msg) typedef char static_assertion_##msg[static_cast<int>(!!(t)) * 2 - 1]
#define TOO_STATIC_ASSERTv2_CONCAT3(X, L) TOO_STATIC_ASSERTv2_CONCAT4(X, static_assertion_at_line_##L)
#define TOO_STATIC_ASSERTv2_CONCAT2(X, L) TOO_STATIC_ASSERTv2_CONCAT3(X, L)
#define TOO_STATIC_ASSERTv2L(X) TOO_STATIC_ASSERTv2_CONCAT2(X, __LINE__)

//! Compile time assert.
#define TOO_STATIC_ASSERTv3(t, msg) typedef char static_assertion_##msg[(t) ? 1 : -1]

//! Which one?
//	#define TOO_STATIC_ASSERT TOO_STATIC_ASSERTv3
//	#define TOO_STATIC_ASSERT_L TOO_STATIC_ASSERTv2L
/** ... now with C++11 the problem is settled. You can just plainly use static_assert.*/
#define TOO_STATIC_ASSERT static_assert

//!
template <typename T1, typename T2>
bool XOR(const T1& lhs, const T2& rhs)
{
    return !lhs != !rhs;
}

template <>
inline bool XOR(const bool& lhs, const bool& rhs)
{
    return lhs != rhs;
}
}

#endif
