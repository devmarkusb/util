// VLD detects lots of memory leaks for this file (unclear)
#include "Toolib/assert.h"
#include <exception>
#include <iostream>
#include "gtest/gtest.h"


#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNING_4127_BEGIN"

TEST(AssertExpectEnsureTest, Assert)
{
#ifdef TOO_ASSERT_ALWAYS_THROWING
    EXPECT_THROW(TOO_ASSERT(false), too::fail_fast);
    EXPECT_THROW(TOO_EXPECT(false), too::fail_fast);
    EXPECT_THROW(TOO_ENSURE(false), too::fail_fast);
#else
#if TOO_DEBUG && GTEST_HAS_DEATH_TEST
    EXPECT_DEBUG_DEATH(TOO_ASSERT(false), "");
    EXPECT_DEBUG_DEATH(TOO_EXPECT(false), "");
    EXPECT_DEBUG_DEATH(TOO_ENSURE(false), "");
#endif
#endif
}

TEST(AssertExpectEnsureTest, Throw)
{
#ifdef TOO_ASSERT_THROW_DISABLE
#if TOO_DEBUG && GTEST_HAS_DEATH_TEST
    EXPECT_DEBUG_DEATH(TOO_ASSERT_THROW(false), "");
    EXPECT_DEBUG_DEATH(TOO_EXPECT_THROW(false), "");
    EXPECT_DEBUG_DEATH(TOO_ENSURE_THROW(false), "");
#endif
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
#if GTEST_HAS_DEATH_TEST
#ifdef TOO_ASSERT_TERMINATE_DISABLE
    EXPECT_DEBUG_DEATH(TOO_ASSERT_TERMINATE(false), "");
    EXPECT_DEBUG_DEATH(TOO_EXPECT_TERMINATE(false), "");
    EXPECT_DEBUG_DEATH(TOO_ENSURE_TERMINATE(false), "");
#else
#if TOO_DEBUG
    EXPECT_DEATH(TOO_ASSERT_TERMINATE(false), "");
    EXPECT_DEATH(TOO_EXPECT_TERMINATE(false), "");
    EXPECT_DEATH(TOO_ENSURE_TERMINATE(false), "");
#endif
#endif
#endif
}

TEST(AssertExpectEnsureTest, Sleep)
{
#ifdef TOO_ASSERT_SLEEP_DISABLE
#if TOO_DEBUG && GTEST_HAS_DEATH_TEST
    EXPECT_DEBUG_DEATH(TOO_ASSERT_SLEEP(false), "");
    EXPECT_DEBUG_DEATH(TOO_EXPECT_SLEEP(false), "");
    EXPECT_DEBUG_DEATH(TOO_ENSURE_SLEEP(false), "");
#endif
#else
// untestable so far, whether not returning after some reasonable time
// TOO_ASSERT_SLEEP, TOO_EXPECT_SLEEP, TOO_ENSURE_SLEEP
#endif
}

TEST(VERIFY_ASSERT_Test, Test)
{
#if TOO_DEBUG
    int i = 1;
#else
    volatile int i = 1;
#endif
    TOO_VERIFY(++i == 2);
    EXPECT_EQ(2, i);
    --i;
    TOO_ASSERT(++i == 2);
#if TOO_DEBUG
    EXPECT_EQ(2, i);
#else
    EXPECT_NE(2, i);
#endif
}

#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNING_END"
