#include "toolib/error.h"
#include "gtest/gtest.h"


TEST(prog_exit_successTest, test)
{
    EXPECT_EQ(0, too::prog_exit_success);
    EXPECT_NE(0, too::prog_exit_failure);
}

namespace
{
retcode f_with_retcode(char x) noexcept
{
    switch (x)
    {
    case 's':
        return retcode::success;
    case 'f':
        return retcode::failed;
    case 'n':
        return retcode::not_implemented;
    case 'r':
        return retcode::runtime_error;
    case 'c':
        return retcode::would_crash;
    default:
        return retcode::none;
    }
}
}

TEST(too_retcodeTest, test)
{
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

    EXPECT_TRUE(f_with_retcode('s') == retcode::none);
    EXPECT_TRUE(f_with_retcode('s') == retcode::success);
    EXPECT_TRUE(f_with_retcode('f') == retcode::failed);
    EXPECT_TRUE(f_with_retcode('n') == retcode::not_implemented);
    EXPECT_TRUE(f_with_retcode('r') == retcode::runtime_error);
    EXPECT_TRUE(f_with_retcode('c') == retcode::would_crash);
}
