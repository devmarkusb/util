// 2018

//!
/** An STL compatible allocator class. This has to be used in conjunction with one of the memory allocation strategies
    or arenas implemented in subdir alloc.*/
//! \file

#ifndef ALLOCATOR_H_hfnx837478ygr78234f7wgmf23uhq4
#define ALLOCATOR_H_hfnx837478ygr78234f7wgmf23uhq4

#include "toolib/mem/types.h"
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>

#include "toolib/macros.h"


namespace too
{
namespace mem
{
template <typename T, typename AllocArenaStrategy>
class Allocator
{
public:
    using value_type = T;

    //Allocator() noexcept = default;
    explicit Allocator(AllocArenaStrategy& a) noexcept
        : a_{a}
    {
    }

    template <typename T2>
    /*implicit*/ Allocator(const Allocator<T2, AllocArenaStrategy>& other) noexcept
        : a_{other.a_}
    {
    }

    value_type* allocate(size_t objcount)
    {
        return reinterpret_cast<value_type*>(a_.allocate(Bytes{objcount * sizeof(value_type)}));
    }

    void deallocate(value_type* p, size_t objcount)
    {
        a_.deallocate(reinterpret_cast<uint8_t*>(p), Bytes{objcount * sizeof(value_type)});
    }

    size_t max_size() const noexcept
    {
        return std::numeric_limits<size_t>::max() / sizeof(value_type);
    }

private:
    AllocArenaStrategy& a_;

    template <typename T1, typename T2, typename AllocArenaStrategy1, typename AllocArenaStrategy2>
    friend bool operator==(
        const Allocator<T1, AllocArenaStrategy1>& lhs, const Allocator<T2, AllocArenaStrategy2>& rhs) noexcept;

    template <typename T2, typename AllocArenaStrategy2>
    friend class Allocator;
};

template <typename T1, typename T2, typename AllocArenaStrategy1, typename AllocArenaStrategy2>
bool operator==(const Allocator<T1, AllocArenaStrategy1>& lhs, const Allocator<T2, AllocArenaStrategy2>& rhs) noexcept
{
    return lhs.a_ == rhs.a_;
}

template <typename T1, typename T2, typename AllocArenaStrategy1, typename AllocArenaStrategy2>
bool operator!=(const Allocator<T1, AllocArenaStrategy1>& lhs, const Allocator<T2, AllocArenaStrategy2>& rhs) noexcept
{
    return !(lhs == rhs);
}
} // namespace mem
} // namespace too

#include "toolib/macros_end.h"

#endif
