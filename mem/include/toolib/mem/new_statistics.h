// Markus Borris, 2019
// This file is part of tfl library.

//! Replaces (overrides) global new and delete to collect memory usage statistics.
/** *Important* This is work in progress!
    I couldn't yet find a way to get delete called with the size being deleted or obtain that size in another
    way without the need of additional storage.

    Usage: You just need to include this header in a single arbitrary source file of your target. Then every
    use of new/delete will be replaced with the custom version here (even standard library calls, everything).
    So please take care that you're absolutely conscious whether you want to do this.
 */
//! \file

#ifndef NEW_STATISTICS_H_iug34gh347xh38gx348gx34yg2g
#define NEW_STATISTICS_H_iug34gh347xh38gx348gx34yg2g

#include "toolib/PPDEFS.h"
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <new>


#if !TOO_I_AM_SURE_TO_REPLACE_NEW_DELETE
#error "To really activate, also define this to 1 before the include."
#endif

namespace too::mem
{
class Statistics
{
public:
    static Statistics& instance() noexcept
    {
        static Statistics stats;
        return stats;
    }

    void newCall(std::size_t sz, void*) noexcept
    {
        newCalls_.fetch_add(1, std::memory_order_relaxed);
        currentSize_.fetch_add(sz, std::memory_order_seq_cst);
        update_maximum(peakSize_, currentSize_.load(std::memory_order_seq_cst));
    }

    void deleteCall(void*) noexcept
    {
        deleteCalls_.fetch_add(1, std::memory_order_relaxed);
        //currentSize_.fetch_sub(?, std::memory_order_seq_cst);
    }

    std::size_t newCalls() const noexcept
    {
        return newCalls_.load();
    }

    std::size_t deleteCalls() const noexcept
    {
        return deleteCalls_.load();
    }

    /** \Returns the maximum/peak size allocated (typically in bytes). Not yet working correctly! Cf. file comment.*/
    std::size_t peakSize() const noexcept
    {
        return peakSize_.load();
    }

private:
    std::atomic<std::size_t> newCalls_{};
    std::atomic<std::size_t> deleteCalls_{};
    std::atomic<std::size_t> currentSize_{};
    std::atomic<std::size_t> peakSize_{};

    Statistics() = default;

    template <typename T>
    void update_maximum(std::atomic<T>& maximum_value, const T& value) const noexcept
    {
        T prev_value = maximum_value;
        while (prev_value < value &&
              !maximum_value.compare_exchange_weak(prev_value, value))
        {
            TOO_NOOP;
        }
    }
};
} // too::mem

void* operator new(std::size_t sz) {
    const auto ret = std::malloc(sz);
    too::mem::Statistics::instance().newCall(sz, ret);
    return ret;
}

void operator delete(void* ptr) noexcept
{
    too::mem::Statistics::instance().deleteCall(ptr);
    std::free(ptr);
}

#endif
