#include "ul/mem/new_handler.h"
#include "ul/ignore_unused.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

namespace {
bool g_my_new_handler_got_called = false;

void my_new_handler() {
    g_my_new_handler_got_called = true;
    throw std::bad_alloc();
}
} // namespace

TEST(NewHandlerSupportTest, test) {
    class C : public ul::mem::NewHandlerSupport<C> {};

    class C_heavy_to_alloc : public ul::mem::NewHandlerSupport<C> {
    public:
        C_heavy_to_alloc() {
            ul::ignore_unused(i1);
            ul::ignore_unused(i2);
            ul::ignore_unused(i3);
            ul::ignore_unused(i4);
        }

    private:
        // about 2GB
        int i1[0x7ffffff];
        int i2[0x7ffffff];
        int i3[0x7ffffff];
        int i4[0x7ffffff];
    };

    ul::mem::NewHandlerSupport<C>::set_new_handler(my_new_handler);
    C c;
    ul::ignore_unused(c);
    EXPECT_FALSE(g_my_new_handler_got_called);
    C* pc = nullptr;
    try {
        pc = new C;
    } catch (std::bad_alloc&) {
        // more an illustration of usage; we don't test out-of-memory
        EXPECT_TRUE(g_my_new_handler_got_called);
        g_my_new_handler_got_called = false;
    }
    EXPECT_FALSE(g_my_new_handler_got_called);
    delete pc;
    C_heavy_to_alloc* pc_heavy = nullptr;
    try {
        pc_heavy = new C_heavy_to_alloc;
    } catch (std::bad_alloc&) {
        // more an illustration of usage; we don't test out-of-memory
        EXPECT_TRUE(g_my_new_handler_got_called);
        g_my_new_handler_got_called = false;
    }
    EXPECT_FALSE(g_my_new_handler_got_called);
    delete pc_heavy;
}
