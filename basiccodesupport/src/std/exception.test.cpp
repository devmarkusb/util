#include "ul/std/exception.h"
#include "gtest/gtest.h"
#include <exception>

#include "ul/macros.h"

namespace ul = mb::ul;

TEST(exception_Capture, basics)
{
    ul::exception::Capture ec1{
        [](std::string s)
        {
            throw std::runtime_error{s};
        },
        "test error 1"};
    EXPECT_THROW(ec1.rethrow(), std::runtime_error);
    try
    {
        ec1.rethrow();
    }
    catch (const std::runtime_error& e)
    {
        EXPECT_STREQ(e.what(), "test error 1");
    }

    ul::exception::Capture ec2{
        []()
        {
            throw std::runtime_error{"test error 2"};
        },
    };
    EXPECT_THROW(ec2.rethrow(), std::runtime_error);
    try
    {
        ec2.rethrow();
    }
    catch (const std::runtime_error& e)
    {
        EXPECT_STREQ(e.what(), "test error 2");
    }

    ul::exception::Capture ec3{[]()
                               {
                                   UL_NOOP;
                               }};
    EXPECT_NO_THROW(ec3.rethrow());
}
