#include "gtest/gtest.h"
#include "Toolib/assert.h"
#include <exception>


TEST(AssertThrowTest, Test)
{
#ifdef NDEBUG
    EXPECT_NO_THROW(too::Assert_throw<std::exception>(1 == 2));
#else
    EXPECT_THROW(too::Assert_throw<std::exception>(1 == 2), std::exception);
    EXPECT_NO_THROW(too::Assert_throw<std::exception>(1 == 1));
#endif
}

TEST(AssertExpectEnsureTest, Assert)
{
	EXPECT_DEBUG_DEATH(TOO_ASSERT(false), "");
	EXPECT_DEBUG_DEATH(TOO_EXPECT(false), "");
	EXPECT_DEBUG_DEATH(TOO_ENSURE(false), "");
}

#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNING_4127_BEGIN"

TEST(AssertExpectEnsureTest, Throw)
{
#ifdef TOO_ASSERT_THROW_DISABLE
	EXPECT_DEBUG_DEATH(TOO_ASSERT_THROW(false), "");
	EXPECT_DEBUG_DEATH(TOO_EXPECT_THROW(false), "");
	EXPECT_DEBUG_DEATH(TOO_ENSURE_THROW(false), "");
#else
	EXPECT_THROW(TOO_ASSERT_THROW(false), too::fail_fast);
	EXPECT_NO_THROW(TOO_ASSERT_THROW(true));
	EXPECT_THROW(TOO_EXPECT_THROW(false), too::fail_fast);
	EXPECT_NO_THROW(TOO_EXPECT_THROW(true));
	EXPECT_THROW(TOO_ENSURE_THROW(false), too::fail_fast);
	EXPECT_NO_THROW(TOO_ENSURE_THROW(true));
#endif
}

TEST(AssertExpectEnsureTest, Terminate)
{
#ifdef TOO_ASSERT_TERMINATE_DISABLE
	EXPECT_DEBUG_DEATH(TOO_ASSERT_TERMINATE(false), "");
	EXPECT_DEBUG_DEATH(TOO_EXPECT_TERMINATE(false), "");
	EXPECT_DEBUG_DEATH(TOO_ENSURE_TERMINATE(false), "");
#else
	EXPECT_DEATH(TOO_ASSERT_TERMINATE(false), "");
	EXPECT_DEATH(TOO_EXPECT_TERMINATE(false), "");
	EXPECT_DEATH(TOO_ENSURE_TERMINATE(false), "");
#endif
}

TEST(AssertExpectEnsureTest, Sleep)
{
#ifdef TOO_ASSERT_SLEEP_DISABLE
	EXPECT_DEBUG_DEATH(TOO_ASSERT_SLEEP(false), "");
	EXPECT_DEBUG_DEATH(TOO_EXPECT_SLEEP(false), "");
	EXPECT_DEBUG_DEATH(TOO_ENSURE_SLEEP(false), "");
#else
	// untestable so far, whether not returning after some reasonable time
	// TOO_ASSERT_SLEEP, TOO_EXPECT_SLEEP, TOO_ENSURE_SLEEP
#endif
}

#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNING_END"
