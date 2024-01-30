// VLD detects lots of memory leaks for this file (unclear)
#include "ul/assert.h"
#include "ul/gtest_utils.h"
#include "ul/macros.h"
#include "ul/warnings.h"
#include "gtest/gtest.h"
#include <iostream>


UL_PRAGMA_WARNINGS_PUSH
UL_WARNING_DISABLE_MSVC(4127)
// clang-format off
UL_WARNING_DISABLE_CLANG(used-but-marked-unused)

// clang-format on

TEST(AssertExpectEnsureTest, Assert) {
#ifdef UL_ASSERT_ALWAYS_THROWING
    EXPECT_THROW(UL_ASSERT(false), ul::FailFast);
    EXPECT_THROW(UL_EXPECT(false), ul::FailFast);
    EXPECT_THROW(UL_ENSURE(false), ul::FailFast);
#else
#if UL_DEBUG && GTEST_HAS_DEATH_TEST
    EXPECT_DEBUG_DEATH(UL_ASSERT(false), "");
    EXPECT_DEBUG_DEATH(UL_EXPECT(false), "");
    EXPECT_DEBUG_DEATH(UL_ENSURE(false), "");
#endif
#endif
}

TEST(AssertExpectEnsureTest, Throw) {
#ifdef UL_ASSERT_THROW_DISABLE
#if UL_DEBUG && GTEST_HAS_DEATH_TEST
    EXPECT_DEBUG_DEATH(UL_ASSERT_THROW(false), "");
    EXPECT_DEBUG_DEATH(UL_EXPECT_THROW(false), "");
    EXPECT_DEBUG_DEATH(UL_ENSURE_THROW(false), "");
#endif
#else
    EXPECT_THROW(UL_ASSERT_THROW(false), ul::FailFast);
    EXPECT_NO_THROW(UL_ASSERT_THROW(true));
    EXPECT_THROW(UL_EXPECT_THROW(false), ul::FailFast);
    EXPECT_NO_THROW(UL_EXPECT_THROW(true));
    EXPECT_THROW(UL_ENSURE_THROW(false), ul::FailFast);
    EXPECT_NO_THROW(UL_ENSURE_THROW(true));
#endif
}

TEST(AssertExpectEnsureTest, Terminate) {
#if GTEST_HAS_DEATH_TEST
#ifdef UL_ASSERT_TERMINATE_DISABLE
    EXPECT_DEBUG_DEATH(UL_ASSERT_TERMINATE(false), "");
    EXPECT_DEBUG_DEATH(UL_EXPECT_TERMINATE(false), "");
    EXPECT_DEBUG_DEATH(UL_ENSURE_TERMINATE(false), "");
#else
#if UL_DEBUG
    EXPECT_DEATH(UL_ASSERT_TERMINATE(false), "");
    EXPECT_DEATH(UL_EXPECT_TERMINATE(false), "");
    EXPECT_DEATH(UL_ENSURE_TERMINATE(false), "");
#endif
#endif
#endif
}

TEST(AssertExpectEnsureTest, Sleep) {
#ifdef UL_ASSERT_SLEEP_DISABLE
#if UL_DEBUG && GTEST_HAS_DEATH_TEST
    EXPECT_DEBUG_DEATH(UL_ASSERT_SLEEP(false), "");
    EXPECT_DEBUG_DEATH(UL_EXPECT_SLEEP(false), "");
    EXPECT_DEBUG_DEATH(UL_ENSURE_SLEEP(false), "");
#endif
#else
// untestable so far, whether not returning after some reasonable time
// UL_ASSERT_SLEEP, UL_EXPECT_SLEEP, UL_ENSURE_SLEEP
#endif
}

TEST(VERIFY_ASSERT_Test, Test) {
#if UL_DEBUG
    int i = 1;
#else
    /*volatile*/ int i = 1;
#endif
    UL_VERIFY(++i == 2);
    EXPECT_EQ(2, i);
    --i;
    UL_ASSERT(++i == 2);
#if UL_DEBUG
    EXPECT_EQ(2, i);
#else
    EXPECT_NE(2, i);
#endif
}

namespace {
void h() {
    UL_ASSERT_THROW(false);
}

void g() {
    h();
}

void f1() {
    g();
}

void f2() {
    g();
}
} // namespace

TEST(AssertThrowTest, stacktrace) {
    ul::dump_test_name();
    try {
        f1();
    } catch (const ul::FailFast& e) {
        std::cout << e.what();
    }
    try {
        f2();
    } catch (const ul::FailFast& e) {
        std::cout << e.what();
    }
    try {
        g();
    } catch (const ul::FailFast& e) {
        std::cout << e.what();
    }
}

UL_PRAGMA_WARNINGS_POP
