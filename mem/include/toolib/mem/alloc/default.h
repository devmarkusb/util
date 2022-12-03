// 2018

//!
/** Implements default new+delete as allocation strategy, plus optional statistics.
 */
//! \file

#ifndef DEFAULT_H_4576gdjfcghkwh45t7428335
#define DEFAULT_H_4576gdjfcghkwh45t7428335

#include "toolib/mem/alloc/statistics.h"
#include "toolib/mem/types.h"
#include <cstdint>
#include <new>


namespace too
{
namespace mem
{
namespace alloc
{
template <typename StatisticsPolicy = NoStatistics>
class DefaultNewDelete : public StatisticsPolicy
{
public:
    uint8_t* allocate(Bytes size)
    {
        this->statsCollect_alloc(size);
        return reinterpret_cast<uint8_t*>(::operator new(size.value));
    }

    void deallocate(uint8_t* p, Bytes size) noexcept
    {
        this->statsCollect_dealloc(size);
        ::operator delete(p);
    }
};
} // namespace alloc
} // namespace mem
} // namespace too
#endif
