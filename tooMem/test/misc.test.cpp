#include "gtest/gtest.h"
#include <memory>
#include "Toolib/mem/checked_delete.h"
#include "Toolib/mem/heap_tracked.h"
#include "Toolib/mem/new_handler.h"
#include "Toolib/ignore_arg.h"
#include "Toolib/PPDEFS.h"


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

namespace
{
bool g_my_new_handler_got_called = false;

void my_new_handler()
{
    g_my_new_handler_got_called = true;
    throw std::bad_alloc();
}
}

TEST(NewHandlerSupportTest, test)
{
    class C : public too::mem::NewHandlerSupport<C>
    {
    };
    class C_heavy_to_alloc : public too::mem::NewHandlerSupport<C>
    {
        int i1[0x7ffffff];
        int i2[0x7ffffff];
        int i3[0x7ffffff];
        int i4[0x7ffffff];
    };
    too::mem::NewHandlerSupport<C>::set_new_handler(my_new_handler);
    C c;
    too::ignore_arg(c);
    EXPECT_FALSE(g_my_new_handler_got_called);
    C* pc = nullptr;
    try
    {
        pc = new C;
    }
    catch (std::bad_alloc&)
    {
        // more an illustration of usage; we don't test out-of-memory
        EXPECT_TRUE(g_my_new_handler_got_called);
        g_my_new_handler_got_called = false;
    }
    EXPECT_FALSE(g_my_new_handler_got_called);
    delete pc;
    C_heavy_to_alloc* pc_heavy = nullptr;
    try
    {
        pc_heavy = new C_heavy_to_alloc;
    }
    catch (std::bad_alloc&)
    {
        // more an illustration of usage; we don't test out-of-memory
        EXPECT_TRUE(g_my_new_handler_got_called);
        g_my_new_handler_got_called = false;
    }
    EXPECT_FALSE(g_my_new_handler_got_called);
    delete pc_heavy;
}
