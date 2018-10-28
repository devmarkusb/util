// Markus Borris, 2018
// This file is part of tfl library.

//!
/** An STL compatible allocator class. This has to be used in conjunction with one of the memory allocation strategies
    implemented in subdir alloc.
*/
//! \file

#ifndef ALLOCATOR_H_hfnx837478ygr78234f7wgmf23uhq4
#define ALLOCATOR_H_hfnx837478ygr78234f7wgmf23uhq4

#include "toolib/PPDEFS.h"
#include <cstddef>
#include <limits>
#include <type_traits>

#include <string>
#include <utility>


namespace too
{
namespace mem
{
template <typename T, typename AllocStrategy>
class Allocator : private AllocStrategy
{
public:
    using value_type = T;

    static_assert(!std::is_polymorphic_v<AllocStrategy>);

    Allocator() noexcept = default;
    template <class U>
    /*implicit*/ Allocator(const Allocator<U, AllocStrategy>&) noexcept {}

    value_type* allocate(size_t s)
    {
        return AllocStrategy::template allocate<value_type>(s);
    }
    void deallocate(value_type* p, size_t)
    {
        AllocStrategy::template deallocate<value_type>(p);
    }

    size_t max_size() const noexcept { return std::numeric_limits<size_t>::max() / sizeof(value_type); }
};

template <typename T1, typename T2, typename AllocStrategy>
bool operator==(const Allocator<T1, AllocStrategy>&, const Allocator<T2, AllocStrategy>&) noexcept
{
    return true;
}

template <typename T1, typename T2, typename AllocStrategy>
bool operator!=(const Allocator<T1, AllocStrategy>& lhs, const Allocator<T2, AllocStrategy>& rhs) noexcept
{
    return !(lhs == rhs);
}
} // mem
} // too
#endif
