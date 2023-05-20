#include "ul/error.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(prog_exit_successTest, test) {
    EXPECT_EQ(0, ul::prog_exit_success);
    EXPECT_NE(0, ul::prog_exit_failure);
}

namespace {
Retcode f_with_retcode(char x) noexcept {
    switch (x) {
        case 's':
            return Retcode::success;
        case 'f':
            return Retcode::failed;
        case 'n':
            return Retcode::not_implemented;
        case 'r':
            return Retcode::runtime_error;
        case 'c':
            return Retcode::would_crash;
        default:
            return Retcode::none;
    }
}
} // namespace

TEST(ul_retcodeTest, test) {
    bool ok = false;
    if (is_ok(f_with_retcode('s')))
        ok = true;
    EXPECT_TRUE(ok);
    ok = false;
    if (!!f_with_retcode(' '))
        ok = true;
    EXPECT_TRUE(ok);
    ok = false;
    if (is_ok(f_with_retcode('f')))
        ok = true;
    EXPECT_FALSE(ok);

    EXPECT_TRUE(f_with_retcode('s') == Retcode::none);
    EXPECT_TRUE(f_with_retcode('s') == Retcode::success);
    EXPECT_TRUE(f_with_retcode('f') == Retcode::failed);
    EXPECT_TRUE(f_with_retcode('n') == Retcode::not_implemented);
    EXPECT_TRUE(f_with_retcode('r') == Retcode::runtime_error);
    EXPECT_TRUE(f_with_retcode('c') == Retcode::would_crash);
}
