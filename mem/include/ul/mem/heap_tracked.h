// 2011-18

//!
/** Taken from Meyers' book.
 */
//! \file

#include "ul/debug.h"
#include "ul/std/std_extensions.h"
#include <algorithm>
#include <exception>
#include <list>


namespace mb::ul
{
namespace mem
{
//! Usage: let your class publically derive from this one.
/** Note: doesn't work with smart pointers, except they are explicitly created by new.
    Testing make_unique/make_shared showed negative results.
    Also note that array allocation (new[]) is not supported.*/
class HeapTracked
{
public:
    virtual ~HeapTracked() = default;

    static void* operator new(size_t size)
    {
        void* memPtr = ::operator new(size);
        addresses().push_front(memPtr);
        return memPtr;
    }

    static void operator delete(void* ptr) noexcept
    {
        std::list<RawAddress>::iterator it = std::find(addresses().begin(), addresses().end(), ptr);

        if (it != addresses().end())
        {
            addresses().erase(it);
            ::operator delete(ptr);
        }
        else
        {
            UL_DEBUG_BREAK_IF(false); // missing address!
            std::terminate(); // delete is nothrow since C++11
        }
    }

    bool isOnHeap() const
    {
        const void* rawAddress = dynamic_cast<const void*>(this);
        std::list<RawAddress>::iterator it = std::find(addresses().begin(), addresses().end(), rawAddress);
        return it != addresses().end();
    }

private:
    using RawAddress = const void*;
    static std::list<RawAddress>& addresses()
    {
        static std::list<RawAddress> addresses_;
        return addresses_;
    }
};
} // namespace mem
} // namespace mb::ul
