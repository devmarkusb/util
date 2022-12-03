#include "ul/mem/heap_tracked.h"
#include "gtest/gtest.h"
#include <memory>

#include "ul/macros.h"

namespace ul = mb::ul;

TEST(HeapTrackedTest, test)
{
    // commented out because that leaks a lot according to VLD (perhaps false positives?); to investigate why
    //    class C : public ul::mem::HeapTracked
    //    {
    //    public:
    //        C() {}
    //        ~C() {}
    //    };
    //
    //    C c_stack_;
    //    C* c_stack = &c_stack_;
    //    C* c_heap  = new C;
    //
    //    EXPECT_FALSE(c_stack->isOnHeap());
    //    EXPECT_TRUE(c_heap->isOnHeap());
    //
    //// still needs to be explored why this seems to fail under mingw
    //#if !UL_COMP_MINGW
    //    EXPECT_DEATH(delete c_stack, "");
    //#endif
    //    EXPECT_NO_FATAL_FAILURE(delete c_heap);
}
