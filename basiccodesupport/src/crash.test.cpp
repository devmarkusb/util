//! Note: quick_exit instead of exit because of clang thread safety warning.
#include "ul/crash.h"

#include "gtest/gtest.h"
#include <csignal>
#include <cstdlib>

TEST(crashTest, sigsegv) {
    ASSERT_EXIT((ul::crash(SIGSEGV), std::quick_exit(0)), ::testing::KilledBySignal(SIGSEGV), ".*");
    ASSERT_EXIT((ul::crash(SIGSEGV, false), std::quick_exit(0)), ::testing::KilledBySignal(SIGSEGV), ".*");
}

TEST(crashTest, sigabrt) {
    ASSERT_EXIT((ul::crash(SIGABRT), std::quick_exit(0)), ::testing::KilledBySignal(SIGABRT), ".*");
    ASSERT_EXIT((ul::crash(SIGABRT, false), std::quick_exit(0)), ::testing::KilledBySignal(SIGABRT), ".*");
}

TEST(crashTest, not_crashing) {
    ASSERT_EXIT((ul::crash(0), std::quick_exit(0)), ::testing::ExitedWithCode(0), ".*");
}
