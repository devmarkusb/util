// 2018

//!
/** Implements default new+delete as example allocation strategy.
 */
//! \file

#ifndef EXAMPLE_H_sdufhi3gn78325tx7635tn27
#define EXAMPLE_H_sdufhi3gn78325tx7635tn27

#include "toolib/mem/types.h"
#include <cstdint>
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
    uint8_t* allocate(Bytes size)
    {
        return reinterpret_cast<uint8_t*>(::operator new(size.value));
    }

    void deallocate(uint8_t* p, Bytes = {}) noexcept
    {
        ::operator delete(p);
    }
};
} // namespace alloc
} // namespace mem
} // namespace too
#endif
