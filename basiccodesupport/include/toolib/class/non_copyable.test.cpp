#include "toolib/class/non_copyable.h"
#include "gtest/gtest.h"


namespace
{
class A : private too::non_copyable
{
};

class B : public A
{
};
} // namespace

TEST(non_copyableTest, test)
{
    // also B is non-copyable!
    //    A a;
    //    A acopy(a);
    //    B b;
    //    B bcopy(b);
}
