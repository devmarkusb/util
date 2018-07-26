#include "toolib/mem/checked_delete.h"
#include "gtest/gtest.h"


namespace misc_test_helper
{
class C;
extern C* c;
}

TEST(checked_deleteTest, test)
{
    // just for demonstration, since it is a compile topic only

    // delete misc_test_helper::c;								// <- this compiles, shows a warning if you're lucky
    // too::mem::checked_delete(misc_test_helper::c);			// <- this doesn't compile; nice :)
}
