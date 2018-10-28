// Markus Borris, 2018
// This file is part of tfl library.

//!
/** Implements the most simple (but perhaps also most efficient) linear memory allocation strategy.
*/
//! \file

#ifndef LINEAR_H_jsdkdbhfzu34gt2837tnyg13
#define LINEAR_H_jsdkdbhfzu34gt2837tnyg13

#include <cstddef>


namespace too
{
namespace mem
{
namespace alloc
{
//class Linear
//{
//public:
//    explicit Linear(size_t capacity) : capacity_{capacity}
//    {
//        mem_start_ = ::operator new(s * sizeof (T)) malloc(m_totalSize);
//    }
//    template <typename T, typename size_type>
//    T* allocate(size_type s)
//    {
//        return reinterpret_cast<T*>(::operator new(s * sizeof (T)));
//    }
//
//    template <typename T>
//    void deallocate(T* p)
//    {
//        ::operator delete(p);
//    }
//
//private:
//    void* mem_start_{};
//    size_t offset_;
//    size_t capacity_{};
//};
} // alloc
} // mem
} // too
#endif
