// Markus Borris, 2011
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef ASSERT_H_INCL_onvdr589tz3758ct438tzcn
#define ASSERT_H_INCL_onvdr589tz3758ct438tzcn

#include "Toolib/PPDEFS.h"
#include <assert.h>
#include <stdexcept>
#include <thread>
#include <chrono>


namespace too
{
	struct fail_fast : public std::runtime_error 
	{
		explicit fail_fast(char const* const message) : std::runtime_error(message) {}
	};
}


#define TOO_ASSERT_IMPL(cond)						assert(cond)

#ifdef TOO_ASSERT_THROW_DISABLE
#define TOO_ASSERT_THROW_IMPL(cond, textstart)		TOO_ASSERT_IMPL(cond)
#else
#define TOO_ASSERT_THROW_IMPL(cond, textstart) \
	do { \
	if (!(cond)) \
		throw too::fail_fast(textstart " " __FILE__ ": " TOO_STRINGIFY(__LINE__)); \
	} while (false)
#endif

#ifdef TOO_ASSERT_SLEEP_DISABLE
#define TOO_ASSERT_SLEEP_IMPL(cond)					TOO_ASSERT_IMPL(cond)
#else
#define TOO_ASSERT_SLEEP_IMPL(cond)					do { for (;;) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); } } while (false)
#endif

#ifdef TOO_ASSERT_TERMINATE_DISABLE
#define TOO_ASSERT_TERMINATE_IMPL(cond)				TOO_ASSERT_IMPL(cond)
#else
#define TOO_ASSERT_TERMINATE_IMPL(cond)				do { if (!(cond)) std::terminate(); } while (false)
#endif


#define TOO_ASSERT(cond)							TOO_ASSERT_IMPL(cond)
#define TOO_ASSERT_SLEEP(cond)						TOO_ASSERT_SLEEP_IMPL(cond)
#define TOO_ASSERT_THROW(cond)						TOO_ASSERT_THROW_IMPL(cond, "assertion failed at")
#define TOO_ASSERT_TERMINATE(cond)					TOO_ASSERT_TERMINATE_IMPL(cond)

#define TOO_EXPECT(cond)							TOO_ASSERT_IMPL(cond)
#define TOO_EXPECT_SLEEP(cond)						TOO_ASSERT_SLEEP_IMPL(cond)
#define TOO_EXPECT_THROW(cond)						TOO_ASSERT_THROW_IMPL(cond, "precondition failed at")
#define TOO_EXPECT_TERMINATE(cond)					TOO_ASSERT_TERMINATE_IMPL(cond)

#define TOO_ENSURE(cond)							TOO_ASSERT_IMPL(cond)
#define TOO_ENSURE_SLEEP(cond)						TOO_ASSERT_SLEEP_IMPL(cond)
#define TOO_ENSURE_THROW(cond)						TOO_ASSERT_THROW_IMPL(cond, "postcondition failed at")
#define TOO_ENSURE_TERMINATE(cond)					TOO_ASSERT_TERMINATE_IMPL(cond)

#if TOO_DEBUG
#define TOO_VERIFY(cond)							TOO_ASSERT(cond)
#else
#define TOO_VERIFY(cond)							sizeof((cond))
#endif

namespace too
{
	//! Assert AssertionExpression and otherwise throw exception ExceptionType.
	/** Usage:
	\code
	Assert<division_by_zero>(!ASSERT_CHECKING || denominator != 0);
	\endcode
	Perhaps you have defined a global switch like ASSERT_CHECK_DIV_BY_ZERO, then
	\code
	Assert<division_by_zero>(!ASSERT_CHECK_DIV_BY_ZERO || denominator != 0);
	\endcode
	would be appropriate.
	*/
	template <typename ExceptionType, typename AssertionExpression> inline void Assert_throw(AssertionExpression
#ifdef NDEBUG
		) {}
#else
		a)
	{
		if (!a) throw ExceptionType();
	}
#endif


	//################################################################################################################################################

	//! Compile time assert.
	/** Idea from October 1997 issue of C/C++ Users Journal.*/
	template <bool t> struct AssertCT
	{
		enum { N = 1 - 2 * static_cast<int>(!t) };
		static char A[N];
	};
	template <bool t> char AssertCT<t>::A[N];

	//! Compile time assert.
	/** Alexey Malistov, SO*/
    template <bool> struct StaticAssert_v0;
    template <> struct StaticAssert_v0<true> {};

	//! Compile time assert.
	#define TOO_STATIC_ASSERTv1_CONCAT_(a, b) a##b
	#define TOO_STATIC_ASSERTv1_CONCAT(a, b) TOO_STATIC_ASSERTv1_CONCAT_(a, b)
    #if TOO_COMP_MS_VISUAL_STUDIO_CPP && __COUNTER__
		#define TOO_STATIC_ASSERTv1(t, msg) \
		{ enum { TOO_STATIC_ASSERTv1_CONCAT(msg, __COUNTER__) = 1/static_cast<int>(!!(t)) }; }
	#else
		#define TOO_STATIC_ASSERTv1(t, msg) \
		{ enum { TOO_STATIC_ASSERTv1_CONCAT(msg, __LINE__) = 1/static_cast<int>(!!(t)) }; }
	#endif

	//! Compile time assert.
	#define TOO_STATIC_ASSERTv2(t, msg) typedef char static_assertion_##msg[static_cast<int>(!!(t))*2-1]
	#define TOO_STATIC_ASSERTv2_CONCAT3(X, L) TOO_STATIC_ASSERTv2_CONCAT4(X, static_assertion_at_line_##L)
	#define TOO_STATIC_ASSERTv2_CONCAT2(X, L) TOO_STATIC_ASSERTv2_CONCAT3(X, L)
	#define TOO_STATIC_ASSERTv2L(X) TOO_STATIC_ASSERTv2_CONCAT2(X, __LINE__)

	//! Compile time assert.
	#define TOO_STATIC_ASSERTv3(t, msg) typedef char static_assertion_##msg[(t)?1:-1]

	//! Which one?
//	#define TOO_STATIC_ASSERT TOO_STATIC_ASSERTv3
//	#define TOO_STATIC_ASSERT_L TOO_STATIC_ASSERTv2L
    /** ... now with C++11 the problem is settled.*/
    #define TOO_STATIC_ASSERT   static_assert
}

#endif
