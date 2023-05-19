//! \file Implements default new+delete as allocation strategy, plus optional statistics.

#ifndef DEFAULT_H_4576GDJFCGHKWH45T7428335
#define DEFAULT_H_4576GDJFCGHKWH45T7428335

#include "ul/mem/alloc/statistics.h"
#include "ul/mem/types.h"
#include <cstdint>
#include <new>

namespace mb::ul::mem::alloc {
template <typename StatisticsPolicy = NoStatistics>
class DefaultNewDelete : public StatisticsPolicy {
public:
    uint8_t* allocate(Bytes size) {
        this->stats_collect_alloc(size);
        return reinterpret_cast<uint8_t*>(::operator new(size.value));
    }

    void deallocate(uint8_t* p, Bytes size) noexcept {
        this->stats_collect_dealloc(size);
        ::operator delete(p);
    }
};
} // namespace mb::ul::mem::alloc

#endif
