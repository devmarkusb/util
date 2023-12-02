//! Note: quick_exit instead of exit because of clang thread safety warning.
#include "ul/crash.h"
#include "ul/macros.h"

#include "gtest/gtest.h"
#include <csignal>
#include <cstdlib>

namespace wrap {
void exit(int status) {
#if UL_OS_MAC
    std::exit(status);
#else
    std::quick_exit(status);
#endif
}
} // namespace wrap

TEST(crashTest, sigsegv) {
    ASSERT_EXIT((ul::crash(SIGSEGV), wrap::exit(0)), ::testing::KilledBySignal(SIGSEGV), ".*");
    ASSERT_EXIT((ul::crash(SIGSEGV, false), wrap::exit(0)), ::testing::KilledBySignal(SIGSEGV), ".*");
}

TEST(crashTest, sigabrt) {
    ASSERT_EXIT((ul::crash(SIGABRT), wrap::exit(0)), ::testing::KilledBySignal(SIGABRT), ".*");
    ASSERT_EXIT((ul::crash(SIGABRT, false), wrap::exit(0)), ::testing::KilledBySignal(SIGABRT), ".*");
}

TEST(crashTest, not_crashing) {
    ASSERT_EXIT((ul::crash(0), wrap::exit(0)), ::testing::ExitedWithCode(0), ".*");
}
