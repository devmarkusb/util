#include "ul/mem/new_handler.h"
#include "ul/ignore_unused.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

namespace {
bool g_my_new_handler_got_called = false; // NOLINT

[[noreturn]] void my_new_handler() {
    g_my_new_handler_got_called = true;
    throw std::bad_alloc();
}
} // namespace

TEST(NewHandlerSupportTest, test) {
    class C : public ul::mem::NewHandlerSupport<C> {};

    class CHeavyToAlloc : public ul::mem::NewHandlerSupport<C> {
    public:
        CHeavyToAlloc() {
            ul::ignore_unused(i1_);
            ul::ignore_unused(i2_);
            ul::ignore_unused(i3_);
            ul::ignore_unused(i4_);
        }

    private:
        // about 2GB
        int i1_[0x7ffffff]; // NOLINT
        int i2_[0x7ffffff]; // NOLINT
        int i3_[0x7ffffff]; // NOLINT
        int i4_[0x7ffffff]; // NOLINT
    };

    ul::mem::NewHandlerSupport<C>::set_new_handler(my_new_handler);
    const C c;
    ul::ignore_unused(c);
    EXPECT_FALSE(g_my_new_handler_got_called);
    C* pc = nullptr;
    try {
        pc = new C; // NOLINT
    } catch (std::bad_alloc&) {
        // more an illustration of usage; we don't test out-of-memory
        EXPECT_TRUE(g_my_new_handler_got_called);
        g_my_new_handler_got_called = false;
    }
    EXPECT_FALSE(g_my_new_handler_got_called);
    delete pc; // NOLINT
    CHeavyToAlloc* pc_heavy = nullptr;
    try {
        pc_heavy = new CHeavyToAlloc; // NOLINT
    } catch (std::bad_alloc&) {
        // more an illustration of usage; we don't test out-of-memory
        EXPECT_TRUE(g_my_new_handler_got_called);
        g_my_new_handler_got_called = false;
    }
    EXPECT_FALSE(g_my_new_handler_got_called);
    delete pc_heavy; // NOLINT
}
