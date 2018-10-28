// Markus Borris, 2018
// This file is part of tfl library.

//!
/** Implements default new+delete as example allocation strategy.
*/
//! \file

#ifndef EXAMPLE_H_sdufhi3gn78325tx7635tn27
#define EXAMPLE_H_sdufhi3gn78325tx7635tn27

#include <cstddef>
#include <new>


namespace too
{
namespace mem
{
namespace alloc
{
class Example
{
public:
    template <typename T>
    T* allocate(size_t s)
    {
        return reinterpret_cast<T*>(::operator new(s * sizeof(T)));
    }

    template <typename T>
    void deallocate(T* p)
    {
        ::operator delete(p);
    }
};
} // alloc
} // mem
} // too
#endif
