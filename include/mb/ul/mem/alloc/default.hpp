//! \file Implements default new+delete as allocation strategy, plus optional statistics.

#ifndef DEFAULT_H_4576GDJFCGHKWH45T7428335
#define DEFAULT_H_4576GDJFCGHKWH45T7428335

#include "mb/ul/buildenv/warnings.hpp"
#include "mb/ul/mem/alloc/statistics.hpp"
#include "mb/ul/mem/types.hpp"
#include <cstdint>
#include <new>

namespace mb::ul::mem::alloc {
template <typename StatisticsPolicy = NoStatistics>
class DefaultNewDelete : public StatisticsPolicy {
public:
    UL_PRAGMA_WARNINGS_PUSH

    UL_WARNING_DISABLE_CLANG_ALLOCATOR_WRAPPERS

    // cppcheck-suppress functionStatic ; allocation strategy API supports stateful statistics policies.
    uint8_t* allocate(Bytes size) {
        this->stats_collect_alloc(size);
        return reinterpret_cast<uint8_t*>(::operator new(size.value));
    }

    UL_PRAGMA_WARNINGS_POP

    // cppcheck-suppress functionStatic ; allocation strategy API supports stateful statistics policies.
    void deallocate(uint8_t* p, Bytes size) noexcept {
        this->stats_collect_dealloc(size);
        ::operator delete(p);
    }
};
} // namespace mb::ul::mem::alloc

#endif
