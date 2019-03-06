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

#include "types.h"
#include "toolib/bits.h"
#include "toolib/compiletime/ct_array.h"
#include "toolib/enum_cast.h"
#include "toolib/std/array.h"
#include "toolib/std/integer_sequence.h"
#include "toolib/thread/atomic.h"
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
class StatsHeader
{
public:
    using BitsType = uintmax_t;
    enum class Field
    {
        size,
        f2,
        f3,
        end,
    };
    static constexpr auto fieldCount{too::as_number(Field::end)};

    static too::bits::FieldsLookup<fieldCount> makeFieldLookup()
    {
        return makeFieldLookup_impl(too::idx::gen_seq<bitCounts.size()>());
    }

    template <typename SourceDataType>
    constexpr void set(
        const too::bits::FieldsLookup<fieldCount>& fieldsLookup, Field field, SourceDataType value) noexcept
    {
        bits_.set(fieldsLookup, field, value);
    }

    template <typename TargetDataType = BitsType>
    constexpr TargetDataType get(const too::bits::FieldsLookup<fieldCount>& fieldsLookup, Field field) const noexcept
    {
        return bits_.template get<TargetDataType>(fieldsLookup, field);
    }

private:
    static constexpr auto bitCounts{too::array::make(
            40, 5, 10 // 40 bits can store sizes of about 1TB of memory, should be sufficient
    )};
    static_assert(fieldCount == bitCounts.size());
    static_assert(too::bits::count<BitsType>() >= too::ct_accumulate(bitCounts, 0));

    too::bits::FieldsRaw<BitsType, Field, too::as_number(Field::end)> bits_;

    template <int... Is>
    static too::bits::FieldsLookup<fieldCount> makeFieldLookup_impl(too::idx::seq<Is...>)
    {
        return too::bits::FieldsLookup<fieldCount>{too::bits::count<StatsHeader::BitsType>(),
                                                   StatsHeader::bitCounts[Is]...};
    }
};

static_assert(alignof(StatsHeader) == alignof(uintmax_t) || alignof(StatsHeader) == alignof(uint64_t));

class Statistics
{
public:
    static Statistics& instance() noexcept
    {
        static Statistics stats;
        return stats;
    }

    void newCall(Bytes size, void* p) noexcept
    {
        newCalls_.fetch_add(1, std::memory_order_relaxed);
        currentSize_.fetch_add(size.value, std::memory_order_seq_cst);
        too::thread::atomic::updateMaximum(peakSize_, currentSize_.load(std::memory_order_seq_cst));
        allocatedSize_.fetch_add(size.value, std::memory_order_relaxed);
        auto sh = new(p) StatsHeader;
        sh->set(fieldsLookup_, StatsHeader::Field::size, size.value);
    }

    void deleteCall(void* p) noexcept
    {
        deleteCalls_.fetch_add(1, std::memory_order_relaxed);
        auto sh = reinterpret_cast<StatsHeader*>(p);
        const auto size = sh->get(fieldsLookup_, StatsHeader::Field::size);
        currentSize_.fetch_sub(size, std::memory_order_seq_cst);
        deallocatedSize_.fetch_add(size, std::memory_order_relaxed);
        sh->~StatsHeader();
    }

    std::size_t newCalls() const noexcept
    {
        return newCalls_.load();
    }

    std::size_t deleteCalls() const noexcept
    {
        return deleteCalls_.load();
    }

    std::size_t allocatedSize() const noexcept
    {
        return allocatedSize_.load();
    }

    std::size_t deallocatedSize() const noexcept
    {
        return deallocatedSize_.load();
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
    std::atomic<std::size_t> allocatedSize_{};
    std::atomic<std::size_t> deallocatedSize_{};
    too::bits::FieldsLookup<StatsHeader::fieldCount> fieldsLookup_{StatsHeader::makeFieldLookup()};

    Statistics() = default;
};
} // too::mem

void* operator new(std::size_t sizeInBytes) {
    const auto p = reinterpret_cast<uint8_t*>(std::malloc(sizeof(too::mem::StatsHeader) + sizeInBytes));
    if (!p)
        throw std::bad_alloc{};
    too::mem::Statistics::instance().newCall(too::mem::Bytes{sizeInBytes}, p);
    return p + sizeof(too::mem::StatsHeader);
}

void operator delete(void* p) noexcept
{
    p = reinterpret_cast<uint8_t*>(p) - sizeof(too::mem::StatsHeader);
    too::mem::Statistics::instance().deleteCall(p);
    std::free(p);
}

void operator delete(void* p, size_t) noexcept
{
    p = reinterpret_cast<uint8_t*>(p) - sizeof(too::mem::StatsHeader);
    too::mem::Statistics::instance().deleteCall(p);
    std::free(p);
}

#endif
