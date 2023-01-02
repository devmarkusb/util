#include "ul/type/non_copyable.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

namespace
{
class A : private ul::NonCopyable
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
