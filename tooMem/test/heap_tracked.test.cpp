#include "Toolib/mem/heap_tracked.h"
#include "Toolib/PPDEFS.h"
#include <memory>
#include "gtest/gtest.h"


TEST(HeapTrackedTest, test)
{
    class C : public too::mem::HeapTracked
    {
    };

    C c_stack_;
    C* c_stack = &c_stack_;
    C* c_heap  = new C;

    EXPECT_FALSE(c_stack->isOnHeap());
    EXPECT_TRUE(c_heap->isOnHeap());

// todo explore why this seem to fail under mingw
#if !TOO_COMP_MINGW
    EXPECT_THROW(delete c_stack, too::mem::HeapTracked::MissingAddress);
#endif
    EXPECT_NO_FATAL_FAILURE(delete c_heap);

    std::shared_ptr<C> c_smart(new C);
    EXPECT_TRUE(c_smart->isOnHeap());
}
