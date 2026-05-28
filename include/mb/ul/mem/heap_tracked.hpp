/** \file Taken from Meyers' book.*/

#ifndef HEAP_TRACKED_H_X4TM7839G934
#define HEAP_TRACKED_H_X4TM7839G934

#include "mb/ul/basiccodesupport/debug.hpp"
#include "mb/ul/basiccodesupport/std/std_extensions.hpp"
#include "mb/ul/buildenv/warnings.hpp"
#include <algorithm>
#include <exception>
#include <list>

namespace mb::ul::mem {
//! Usage: let your class publicly derive from this one.
/** Note: doesn't work with smart pointers, except they are explicitly created by new.
    Testing make_unique/make_shared showed negative results.
    Also note that array allocation (new[]) is not supported.*/
class HeapTracked {
public:
    virtual ~HeapTracked() = default;

    UL_PRAGMA_WARNINGS_PUSH

    UL_WARNING_DISABLE_CLANG_ALLOCATOR_WRAPPERS

    void* operator new(size_t size) {
        void* mem_ptr = ::operator new(size);
        addresses().push_front(mem_ptr);
        return mem_ptr;
    }

    UL_PRAGMA_WARNINGS_POP

    // cppcheck-suppress functionConst ; class deallocation functions cannot be const-qualified.
    void operator delete(void* ptr) noexcept {
        auto it = std::find(addresses().begin(), addresses().end(), ptr);

        if (it != addresses().end()) {
            addresses().erase(it);
            ::operator delete(ptr);
        } else {
            UL_DEBUG_BREAK_IF(true); // missing address!
            std::terminate(); // delete is nothrow since C++11
        }
    }

    [[nodiscard]] bool is_on_heap() const {
        const void* raw_address = dynamic_cast<const void*>(this);
        auto it = std::find(addresses().begin(), addresses().end(), raw_address);
        return it != addresses().end();
    }

private:
    using RawAddress = const void*;

    static std::list<RawAddress>& addresses() {
        static std::list<RawAddress> addresses;
        return addresses;
    }
};
} // namespace mb::ul::mem

#endif
