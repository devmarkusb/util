#include "Toolib/mem/new_handler.h"
#include "Toolib/ignore_arg.h"
#include "gtest/gtest.h"


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
    public:
        C_heavy_to_alloc()
        {
            too::ignore_arg(i1);
            too::ignore_arg(i2);
            too::ignore_arg(i3);
            too::ignore_arg(i4);
        }

    private:
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
