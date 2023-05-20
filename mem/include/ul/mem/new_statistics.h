//! \file Replaces (overrides) global new and delete to collect memory usage statistics.
/** Usage: You just need to include this header in a single arbitrary source file of your target. Then every
    use of new/delete will be replaced with the custom version here (even standard library calls, everything).
    So please take care that you're absolutely conscious whether you want to do this.*/

#ifndef NEW_STATISTICS_H_IUG34GH347XH38GX348GX34YG2G
#define NEW_STATISTICS_H_IUG34GH347XH38GX348GX34YG2G

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

namespace mb::ul::mem {
class StatsHeader {
public:
    using BitsType = uintmax_t;
    enum class Field {
        size,
        f2,
        f3,
        end,
    };
    static constexpr auto field_count{ul::as_number(Field::end)};

    static ul::bits::FieldsLookup<field_count> make_field_lookup() {
        return make_field_lookup_impl(ul::idx::GenSeq<bit_counts.size()>());
    }

    template <typename SourceDataType>
    constexpr void set(
        const ul::bits::FieldsLookup<field_count>& fieldsLookup, Field field, SourceDataType value) noexcept {
        bits_.set(fieldsLookup, field, value);
    }

    template <typename TargetDataType = BitsType>
    [[nodiscard]] constexpr TargetDataType get(
        const ul::bits::FieldsLookup<field_count>& fieldsLookup, Field field) const noexcept {
        return bits_.template get<TargetDataType>(fieldsLookup, field);
    }

private:
    static constexpr auto bit_counts{ul::array::make(
        40, 5, 10 // 40 bits can store sizes of about 1TB of memory, should be sufficient
        )};
    static_assert(field_count == bit_counts.size());
    static_assert(ul::bits::count<BitsType>() >= ul::ct_accumulate(bit_counts, 0));

    ul::bits::FieldsRaw<BitsType, Field, ul::as_number(Field::end)> bits_;

    template <int... is>
    static ul::bits::FieldsLookup<field_count> make_field_lookup_impl(ul::idx::Seq<is...>) {
        return ul::bits::FieldsLookup<field_count>{
            ul::bits::count<StatsHeader::BitsType>(), StatsHeader::bit_counts[is]...};
    }
};

static_assert(alignof(StatsHeader) == alignof(uintmax_t) || alignof(StatsHeader) == alignof(uint64_t));

class Statistics {
public:
    static Statistics& instance() {
        static Statistics stats;
        return stats;
    }

    void new_call(Bytes size, void* p) noexcept {
        new_calls_.fetch_add(1, std::memory_order_relaxed);
        current_size_.fetch_add(size.value, std::memory_order_seq_cst);
        ul::thread::atomic::update_maximum(peak_size_, current_size_.load(std::memory_order_seq_cst));
        allocated_size_.fetch_add(size.value, std::memory_order_relaxed);
        auto sh = new (p) StatsHeader;
        sh->set(fields_lookup_, StatsHeader::Field::size, size.value);
    }

    void delete_call(void* p) noexcept {
        delete_calls_.fetch_add(1, std::memory_order_relaxed);
        auto sh = reinterpret_cast<StatsHeader*>(p);
        const auto size = sh->get<size_t>(fields_lookup_, StatsHeader::Field::size);
        current_size_.fetch_sub(size, std::memory_order_seq_cst);
        deallocated_size_.fetch_add(size, std::memory_order_relaxed);
        sh->~StatsHeader();
    }

    [[nodiscard]] std::size_t new_calls() const noexcept {
        return new_calls_.load();
    }

    [[nodiscard]] std::size_t delete_calls() const noexcept {
        return delete_calls_.load();
    }

    [[nodiscard]] Bytes allocated_size() const noexcept {
        return Bytes{allocated_size_.load()};
    }

    [[nodiscard]] Bytes deallocated_size() const noexcept {
        return Bytes{deallocated_size_.load()};
    }

    /** \return the maximum/peak size allocated.*/
    [[nodiscard]] Bytes peak_size() const noexcept {
        return Bytes{peak_size_.load()};
    }

    void reset() noexcept {
        new_calls_ = {};
        delete_calls_ = {};
        current_size_ = {};
        peak_size_ = {};
        allocated_size_ = {};
        deallocated_size_ = {};
    }

private:
    std::atomic<std::size_t> new_calls_{};
    std::atomic<std::size_t> delete_calls_{};
    std::atomic<std::size_t> current_size_{};
    std::atomic<std::size_t> peak_size_{};
    std::atomic<std::size_t> allocated_size_{};
    std::atomic<std::size_t> deallocated_size_{};
    ul::bits::FieldsLookup<StatsHeader::field_count> fields_lookup_{StatsHeader::make_field_lookup()};

    Statistics() = default;
};
} // namespace mb::ul::mem

// redundant declarations by standard; you get the implementations overrides by linking the lib
//void* operator new(std::size_t sizeInBytes);
//void operator delete(void* p) noexcept;
void operator delete(void* p, size_t) noexcept;

#endif
