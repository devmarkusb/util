/** \file Taken from Meyers' book.*/

#ifndef HEAP_TRACKED_H_X4TM7839G934
#define HEAP_TRACKED_H_X4TM7839G934

#include "ul/debug.h"
#include "ul/std/std_extensions.h"
#include <algorithm>
#include <exception>
#include <list>

namespace mb::ul::mem {
//! Usage: let your class publically derive from this one.
/** Note: doesn't work with smart pointers, except they are explicitly created by new.
    Testing make_unique/make_shared showed negative results.
    Also note that array allocation (new[]) is not supported.*/
class HeapTracked {
public:
    virtual ~HeapTracked() = default;

    static void* operator new(size_t size) {
        void* mem_ptr = ::operator new(size);
        addresses().push_front(mem_ptr);
        return mem_ptr;
    }

    static void operator delete(void* ptr) noexcept {
        auto it = std::find(addresses().begin(), addresses().end(), ptr);

        if (it != addresses().end()) {
            addresses().erase(it);
            ::operator delete(ptr);
        } else {
            UL_DEBUG_BREAK_IF(false); // missing address!
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
