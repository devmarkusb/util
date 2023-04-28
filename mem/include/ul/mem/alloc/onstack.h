/** \file
    Implements allocation on the stack as arena/strategy.
    Important note:
    You should be aware that deallocate here does nothing (except for corner cases when deallocating
    a memory range allocated immediately before). So you will use up a lot more memory than you think, if you are
    erasing a lot. Since erasing doesn't decrease the counter of memory used.*/

#ifndef ONSTACK_H_eourtz3478xth378tgh
#define ONSTACK_H_eourtz3478xth378tgh

#include "ul/assert.h"
#include "ul/mem/alloc/statistics.h"
#include "ul/mem/types.h"
#include "ul/mem/utility.h"
#include "ul/type/non_copyable.h"
#include "ul/warnings.h"
#include <cstddef>
#include <cstdint>

namespace mb::ul::mem::alloc {
/** You should adapt max_alignment_in_bytes to at least the alignment of the largest type you're going to
    allocate frequently. But not larger as this wastes space due to padding.*/
template <size_t capacity_in_bytes, size_t max_alignment_in_bytes, typename StatisticsPolicy = NoStatistics>
class OnStack
    : private ul::NonCopyable
    , public StatisticsPolicy {
public:
    uint8_t* allocate(Bytes size) {
        const auto padded_size = padUp(size.value, max_alignment_in_bytes);
        if (static_cast<decltype(padded_size)>(buf_ + capacity_in_bytes - curr_memptr_) < padded_size) // NOLINT
            throw std::bad_alloc{};

        auto* old_memptr = curr_memptr_;
        curr_memptr_ += padded_size; // NOLINT

        this->statsCollect_currentSize(this->size());

        return old_memptr;
    }

    void deallocate(uint8_t* p, Bytes size) noexcept {
        size.value = padUp(size.value, max_alignment_in_bytes);
        if (p + size.value == curr_memptr_) // NOLINT
            curr_memptr_ = p;
    }

    static constexpr Bytes capacity() noexcept {
        return Bytes{capacity_in_bytes};
    }

    [[nodiscard]] Bytes size() const noexcept {
        return Bytes{static_cast<size_t>(curr_memptr_ - buf_)}; // NOLINT
    }

    void reset() noexcept {
        curr_memptr_ = buf_;
    }

private:
    UL_PRAGMA_WARNINGS_PUSH
    UL_WARNING_DISABLE_MSVC(4324) // structure was padded due to __declspec(align())
    alignas(max_alignment_in_bytes) uint8_t buf_[capacity_in_bytes]{}; // NOLINT
    UL_PRAGMA_WARNINGS_POP
    uint8_t* curr_memptr_{buf_}; // NOLINT
};
} // namespace mb::ul::mem::alloc

#endif
