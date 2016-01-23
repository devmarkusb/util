// Markus Borris, 2011
// This file is part of Toolib library.

//!
/** Taken from Meyers' book.
*/
//! \file

#include <list>
#include <algorithm>


namespace too
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
    class MissingAddress : public std::exception
    {
    };

    virtual inline ~HeapTracked() = 0; // to make the class abstract

    static void* operator new(size_t size)
    {
        void* memPtr = ::operator new(size);
        addresses().push_front(memPtr);
        return memPtr;
    }

    static void operator delete(void* ptr)
    {
        std::list<RawAddress>::iterator it = std::find(addresses().begin(), addresses().end(), ptr);

        if (it != addresses().end())
        {
            addresses().erase(it);
            ::operator delete(ptr);
        }
        else
        {
            throw MissingAddress();
        }
    }

    bool isOnHeap() const
    {
        const void* rawAddress             = dynamic_cast<const void*>(this);
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
inline HeapTracked::~HeapTracked()
{
}
}
}
