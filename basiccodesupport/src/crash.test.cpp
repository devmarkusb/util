#include "ul/crash.h"

#include "gtest/gtest.h"
#include <csignal>
#include <cstdlib>

TEST(crashTest, sigsegv) {
    ASSERT_EXIT((ul::crash(SIGSEGV), std::exit(0)), ::testing::KilledBySignal(SIGSEGV), ".*");
    ASSERT_EXIT((ul::crash(SIGSEGV, false), std::exit(0)), ::testing::KilledBySignal(SIGSEGV), ".*");
}

TEST(crashTest, sigabrt) {
    ASSERT_EXIT((ul::crash(SIGABRT), std::exit(0)), ::testing::KilledBySignal(SIGABRT), ".*");
    ASSERT_EXIT((ul::crash(SIGABRT, false), std::exit(0)), ::testing::KilledBySignal(SIGABRT), ".*");
}

TEST(crashTest, not_crashing) {
    ASSERT_EXIT((ul::crash(0), std::exit(0)), ::testing::ExitedWithCode(0), ".*");
}
