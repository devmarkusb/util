//! Note: quick_exit instead of exit because of clang thread safety warning.
#include "ul/crash.h"
#include "ul/config_gen.h"

#include "gtest/gtest.h"
#include <csignal>
#include <cstdlib>

namespace wrap {
namespace {
void exit(int status) {
#if UL_OS_MAC
    std::exit(status);
#else
    std::quick_exit(status);
#endif
}
} // namespace
} // namespace wrap

#if !UL_ADDRESS_SAN && !UL_UNDEF_SAN
#if UL_OS_WINDOWS
TEST(crashDeathTest, sigsegv) {
    EXPECT_DEATH(ul::crash(SIGSEGV), ".*");
    EXPECT_DEATH(ul::crash(SIGSEGV, false), ".*");
}
#else
TEST(crashDeathTest, sigsegv) {
    EXPECT_EXIT((ul::crash(SIGSEGV), wrap::exit(0)), ::testing::KilledBySignal(SIGSEGV), ".*");
    EXPECT_EXIT((ul::crash(SIGSEGV, false), wrap::exit(0)), ::testing::KilledBySignal(SIGSEGV), ".*");
}

TEST(crashDeathTest, sigabrt) {
    EXPECT_EXIT((ul::crash(SIGABRT), wrap::exit(0)), ::testing::KilledBySignal(SIGABRT), ".*");
    EXPECT_EXIT((ul::crash(SIGABRT, false), wrap::exit(0)), ::testing::KilledBySignal(SIGABRT), ".*");
}
#endif
#endif

TEST(crash, not_crashing) {
    EXPECT_EXIT((ul::crash(0), wrap::exit(0)), ::testing::ExitedWithCode(0), ".*");
}
