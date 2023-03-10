//! \file Replaces (overrides) global new and delete to collect memory usage statistics.
/** Usage: You just need to include this header in a single arbitrary source file of your target. Then every
    use of new/delete will be replaced with the custom version here (even standard library calls, everything).
    So please take care that you're absolutely conscious whether you want to do this.*/

#ifndef NEW_STATISTICS_H_iug34gh347xh38gx348gx34yg2g
#define NEW_STATISTICS_H_iug34gh347xh38gx348gx34yg2g

#include "types.h"
#include "ul/array.h"
#include "ul/bits.h"
#include "ul/enum_cast.h"
#include "ul/std/array.h"
#include "ul/std/integer_sequence.h"
#include "ul/thread/atomic.h"
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <new>


#if !UL_I_AM_SURE_TO_REPLACE_NEW_DELETE
#error "To really activate, also define this to 1 before the include."
#endif

namespace mb::ul::mem
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
    static constexpr auto fieldCount{ul::as_number(Field::end)};

    static ul::bits::FieldsLookup<fieldCount> makeFieldLookup()
    {
        return makeFieldLookup_impl(ul::idx::gen_seq<bitCounts_.size()>());
    }

    template <typename SourceDataType>
    constexpr void set(
        const ul::bits::FieldsLookup<fieldCount>& fieldsLookup, Field field, SourceDataType value) noexcept
    {
        bits_.set(fieldsLookup, field, value);
    }

    template <typename TargetDataType = BitsType>
    [[nodiscard]] constexpr TargetDataType get(
        const ul::bits::FieldsLookup<fieldCount>& fieldsLookup, Field field) const noexcept
    {
        return bits_.template get<TargetDataType>(fieldsLookup, field);
    }

private:
    static constexpr auto bitCounts_{ul::array::make(
        40, 5, 10 // 40 bits can store sizes of about 1TB of memory, should be sufficient
        )};
    static_assert(fieldCount == bitCounts_.size());
    static_assert(ul::bits::count<BitsType>() >= ul::ct_accumulate(bitCounts_, 0));

    ul::bits::FieldsRaw<BitsType, Field, ul::as_number(Field::end)> bits_;

    template <int... Is>
    static ul::bits::FieldsLookup<fieldCount> makeFieldLookup_impl(ul::idx::seq<Is...>)
    {
        return ul::bits::FieldsLookup<fieldCount>{
            ul::bits::count<StatsHeader::BitsType>(), StatsHeader::bitCounts_[Is]...};
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
        ul::thread::atomic::updateMaximum(peakSize_, currentSize_.load(std::memory_order_seq_cst));
        allocatedSize_.fetch_add(size.value, std::memory_order_relaxed);
        auto sh = new (p) StatsHeader;
        sh->set(fieldsLookup_, StatsHeader::Field::size, size.value);
    }

    void deleteCall(void* p) noexcept
    {
        deleteCalls_.fetch_add(1, std::memory_order_relaxed);
        auto sh = reinterpret_cast<StatsHeader*>(p);
        const auto size = sh->get<size_t>(fieldsLookup_, StatsHeader::Field::size);
        currentSize_.fetch_sub(size, std::memory_order_seq_cst);
        deallocatedSize_.fetch_add(size, std::memory_order_relaxed);
        sh->~StatsHeader();
    }

    [[nodiscard]] std::size_t newCalls() const noexcept
    {
        return newCalls_.load();
    }

    [[nodiscard]] std::size_t deleteCalls() const noexcept
    {
        return deleteCalls_.load();
    }

    [[nodiscard]] Bytes allocatedSize() const noexcept
    {
        return Bytes{allocatedSize_.load()};
    }

    [[nodiscard]] Bytes deallocatedSize() const noexcept
    {
        return Bytes{deallocatedSize_.load()};
    }

    /** \return the maximum/peak size allocated.*/
    [[nodiscard]] Bytes peakSize() const noexcept
    {
        return Bytes{peakSize_.load()};
    }

    void reset() noexcept
    {
        newCalls_ = {};
        deleteCalls_ = {};
        currentSize_ = {};
        peakSize_ = {};
        allocatedSize_ = {};
        deallocatedSize_ = {};
    }

private:
    std::atomic<std::size_t> newCalls_{};
    std::atomic<std::size_t> deleteCalls_{};
    std::atomic<std::size_t> currentSize_{};
    std::atomic<std::size_t> peakSize_{};
    std::atomic<std::size_t> allocatedSize_{};
    std::atomic<std::size_t> deallocatedSize_{};
    ul::bits::FieldsLookup<StatsHeader::fieldCount> fieldsLookup_{StatsHeader::makeFieldLookup()};

    Statistics() = default;
};
} // namespace mb::ul::mem

// redundant declarations by standard; you get the implementations overrides by linking the lib
//void* operator new(std::size_t sizeInBytes);
//void operator delete(void* p) noexcept;
void operator delete(void* p, size_t) noexcept;

#endif
