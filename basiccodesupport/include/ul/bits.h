//! \file

#ifndef BITS_H_SLJKHVNX2HNG7835GHM3G7H3
#define BITS_H_SLJKHVNX2HNG7835GHM3G7H3

#include "assert.h"
#include "enum_cast.h"
#include "narrow.h"
#include "ul/warnings.h"
#include <array>
#include <climits>
#include <concepts>
#include <cstdint>
#include <numeric>
#include <type_traits>
#if __has_include(<bit>)
#include <bit>
#endif

namespace mb::ul::bits {
using Idx = int;
using Count = Idx;
using Diff = int;

constexpr auto bits_per_byte{CHAR_BIT};

//! \return number of bits of the arbitrary Type.
template <typename Type>
constexpr Count count() noexcept {
    return bits_per_byte * sizeof(Type);
}

constexpr int count_set(uint32_t data) noexcept {
#if __cpp_lib_bitop
    return std::popcount(data);
#else
    // NOLINTBEGIN
    // Hamming Weight algorithm
    data = data - ((data >> 1u) & 0x55555555u);
    data = (data & 0x33333333u) + ((data >> 2u) & 0x33333333u);
    return static_cast<int>((((data + (data >> 4u)) & 0x0F0F0F0Fu) * 0x01010101u) >> 24u);
    // NOLINTEND
#endif
}

template <typename SourceType, typename TargetType = SourceType>
constexpr TargetType set(SourceType from, Idx idx) noexcept {
    UL_EXPECT(idx < ul::bits::count<TargetType>());
    return static_cast<TargetType>(from | (TargetType{1} << idx)); // NOLINT
}

template <typename SourceType, typename TargetType = SourceType>
constexpr TargetType unset(SourceType from, Idx idx) noexcept {
    UL_EXPECT(idx < ul::bits::count<TargetType>());
    return ul::narrow_cast<TargetType>(from & ~(TargetType{1} << idx)); // NOLINT
}

template <typename SourceType, typename TargetType = SourceType>
constexpr TargetType toggle(SourceType from, Idx idx) noexcept {
    UL_EXPECT(idx < ul::bits::count<TargetType>());
    return ul::narrow_cast<TargetType>(from ^ (TargetType{1} << idx)); // NOLINT
}

template <typename SourceType, typename TargetType = SourceType>
constexpr TargetType check(SourceType from, Idx idx) noexcept {
    UL_EXPECT(idx < ul::bits::count<SourceType>());
    return ul::narrow_cast<TargetType>((from >> idx) & TargetType{1}); // NOLINT
}

template <typename SourceType, typename ChangeBitSourceType, typename TargetType = SourceType>
constexpr TargetType change(SourceType from, Idx idx, ChangeBitSourceType x) noexcept {
    UL_EXPECT(idx < ul::bits::count<TargetType>());
    const SourceType newbit = static_cast<bool>(x); // ensure 1 or 0
    return ul::narrow_cast<TargetType>(from ^ ((-newbit ^ from) & (TargetType{1} << idx))); // NOLINT
}

template <typename SourceType, typename TargetType = SourceType>
constexpr TargetType set_mask(SourceType from, SourceType mask) noexcept {
    return from | mask;
}

template <typename SourceType, typename TargetType = SourceType>
constexpr TargetType unset_mask(SourceType from, SourceType mask) noexcept {
    return ul::narrow_cast<TargetType>(from & ~mask); // NOLINT
}

template <typename SourceType, typename TargetType = SourceType>
constexpr TargetType toggle_mask(SourceType from, SourceType mask) noexcept {
    return from ^ mask;
}

template <typename SourceType, typename TargetType = SourceType>
constexpr TargetType check_all_of_mask(SourceType from, SourceType mask) noexcept {
    return (from & mask) == mask;
}

template <typename SourceType, typename TargetType = SourceType>
constexpr TargetType check_any_of_mask(SourceType from, SourceType mask) noexcept {
    return from & mask;
}

//! Makes a bit mask of count 1's, > 0 starting at 0-based index idx.
/** \tparam TargetType is typically chosen to be uintX_t, that is unsigned with some arbitrary bit count.
    You can think of the index starting at the LSB (least significant bit, which comes last in the memory order
    of big endian, but endianness doesn't matter considering the realm of this function alone).*/
template <std::unsigned_integral TargetType = uint64_t>
constexpr TargetType set_range(Idx idx, Count count) noexcept {
    static_assert(std::is_unsigned_v<TargetType>);
    UL_EXPECT(idx >= 0);
    UL_EXPECT(count > 0);
    UL_EXPECT(idx + count <= ul::bits::count<TargetType>());

    return static_cast<TargetType>(
        ((TargetType{1} << static_cast<TargetType>(count)) - 1) << static_cast<TargetType>(idx)); // NOLINT
}

//! Reads count > 0 bits of from starting at 0-based index idx (0 is LSB).
template <typename SourceType>
constexpr SourceType read(SourceType from, Idx idx, Count count) noexcept {
    UL_EXPECT(idx >= 0);
    UL_EXPECT(count > 0);
    UL_EXPECT(idx + count <= ul::bits::count<SourceType>());

    return (from & ul::bits::set_range<SourceType>(idx, count)) >> idx; // NOLINT
}

//! Like read. But here you can also opt for a different target type.
/** As typically reading out a subset of bits results in a smaller type.*/
template <typename TargetType, typename SourceType /*>= TargetType*/>
constexpr TargetType read_and_cast(SourceType from, Idx idx, Count count) noexcept {
    UL_EXPECT(idx >= 0);
    UL_EXPECT(count > 0);
    UL_EXPECT(idx + count <= ul::bits::count<SourceType>());
    UL_EXPECT(count <= ul::bits::count<TargetType>());

    return static_cast<TargetType>((from & ul::bits::set_range<SourceType>(idx, count)) >> idx); // NOLINT
}

//! Write count > 0 bits of from into to starting at 0-based index idx there (0 is LSB).
template <std::unsigned_integral TargetType, std::unsigned_integral SourceType>
constexpr TargetType write(TargetType to, Idx idx, Count count, SourceType from) noexcept {
    UL_EXPECT(idx >= 0);
    UL_EXPECT(count > 0);
    UL_EXPECT(idx + count <= ul::bits::count<TargetType>());
    UL_EXPECT(count <= ul::bits::count<SourceType>());

    return static_cast<TargetType>(
        (to & (~ul::bits::set_range<TargetType>(idx, count))) | (from << static_cast<SourceType>(idx))); // NOLINT
}

//! If 64 bits aren't sufficient, this is the type to go.
/** Choose your desired bit count bits freely. The underlying type of the parts being glued together might
    also be chosen: BaseType.*/
template <Count bits, typename BaseType = uint32_t>
class Array {
public:
    void set(Idx idx) noexcept {
        UL_EXPECT(idx >= 0);
        UL_EXPECT(idx < bits);
        array_[n(idx)] |= part_bit(i(idx)); // NOLINT
    }

    void reset(Idx idx) noexcept {
        UL_EXPECT(idx >= 0);
        UL_EXPECT(idx < bits);
        array_[n(idx)] &= ~part_bit(i(idx)); // NOLINT
    }

    void reset() noexcept {
        array_.fill({});
    }

    [[nodiscard]] bool is_set(Idx idx) const noexcept {
        UL_EXPECT(idx >= 0);
        UL_EXPECT(idx < bits);
        return array_[n(idx)] & part_bit(i(idx)); // NOLINT
    }

private:
    static constexpr Count parts_count{
        static_cast<Count>((bits + (ul::bits::count<BaseType>() - Count{1})) / ul::bits::count<BaseType>())};
    std::array<BaseType, parts_count> array_{};

    [[nodiscard]] size_t n(Idx idx) const noexcept {
        return static_cast<size_t>(idx / ul::bits::count<BaseType>());
    }

    [[nodiscard]] Idx i(Idx idx) const noexcept {
        return idx % ul::bits::count<BaseType>();
    }

    [[nodiscard]] BaseType part_bit(Idx n) const noexcept {
        return static_cast<BaseType>(BaseType{1} << n); // NOLINT
    }

    template <Count bits_other, typename BaseTypeOther>
    friend Array<bits_other, BaseTypeOther> operator&(
        const Array<bits_other, BaseTypeOther>& lhs, const Array<bits_other, BaseTypeOther>& rhs) noexcept;
    template <Count bits_other, typename BaseTypeOther>
    friend Array<bits_other, BaseTypeOther> operator|(
        const Array<bits_other, BaseTypeOther>& lhs, const Array<bits_other, BaseTypeOther>& rhs) noexcept;
};

template <Count bits, typename BaseType = uint32_t>
Array<bits, BaseType> operator&(const Array<bits, BaseType>& lhs, const Array<bits, BaseType>& rhs) noexcept {
    Array<bits, BaseType> ret;
    for (size_t i = 0; i < Array<bits, BaseType>::partsCount; ++i)
        ret.array_[i] = lhs.array_[i] & rhs.array_[i];
    return ret;
}

template <Count bits, typename BaseType = uint32_t>
Array<bits, BaseType> operator|(const Array<bits, BaseType>& lhs, const Array<bits, BaseType>& rhs) noexcept {
    Array<bits, BaseType> ret;
    for (size_t i = 0; i < Array<bits, BaseType>::partsCount; ++i)
        ret.array_[i] = lhs.array_[i] | rhs.array_[i];
    return ret;
}

//! Needed for FieldsRaw.
template <size_t fields>
struct FieldsLookup {
    std::array<Count, fields> counts; // count of bits of each field
    std::array<Count, fields> indices; // indices where field sits

    template <typename... Counts>
    explicit FieldsLookup(Count max_count, Counts... cs)
        : counts({static_cast<Count>(cs)...}) {
        static_assert(fields == sizeof...(cs));

        indices[0] = {};
        UL_PRAGMA_WARNINGS_PUSH
        // clang-format off
        UL_WARNING_DISABLE_CLANG(unsafe-buffer-usage)
        // clang-format on
        std::partial_sum(std::begin(counts), std::end(counts) - 1, std::begin(indices) + 1);
        UL_PRAGMA_WARNINGS_POP
        // runtime check as long as we don't have a compiletime partial_sum
        UL_ENSURE_THROW(indices.back() + counts.back() <= max_count);
    }
};

//! Just if can't use Fields (cf.) because you need it to be no larger than sizeof(BitDataType).
template <std::unsigned_integral BitDataType, typename EnumType, size_t fields>
class FieldsRaw {
public:
    static_assert(std::is_unsigned_v<BitDataType>);
    static_assert(std::is_enum_v<EnumType>);
    static_assert(fields > 0);

    template <std::unsigned_integral SourceDataType>
    constexpr void set(const FieldsLookup<fields>& fields_lookup, EnumType field, SourceDataType value) noexcept {
        const auto fieldnr{as_number(field)};
        UL_ASSERT(fieldnr >= 0);
        const auto fieldnr_c{static_cast<size_t>(fieldnr)};
        data_ = write<BitDataType, SourceDataType>(
            data_, fields_lookup.indices[fieldnr_c], fields_lookup.counts[fieldnr_c], value); // NOLINT
    }

    template <std::unsigned_integral TargetDataType = BitDataType>
    [[nodiscard]] constexpr TargetDataType get(
        const FieldsLookup<fields>& fields_lookup, EnumType field) const noexcept {
        const auto fieldnr{as_number(field)};
        UL_ASSERT(fieldnr >= 0);
        const auto fieldnr_c{static_cast<size_t>(fieldnr)};
        return read_and_cast<TargetDataType, BitDataType>(
            data_, fields_lookup.indices[fieldnr_c], fields_lookup.counts[fieldnr_c]); // NOLINT
    }

private:
    BitDataType data_{};
};

//! A replacement for the suboptimal standard bitfields.
/** Usage: Decide for a data type large enough to hold your bits, declare an enum (class) with names for your
    distinct fields sharing the bits, with a last entry for simple calculation of the field count.
    Then finally construct Fields with the desired counts of bits of each field.
    enum Fields
    {
        first,
        second,
        third,
        fields_end, // one behind last
    }
    ul::bits::Field<uint32_t, Fields, fields_end> bits_{16, 8, 8};

    void f()
    {
        bits_.set(Fields::second, 255);
        UL_ASSERT(bits_.get(Fields::second) == 255);
    }
    */
template <std::unsigned_integral BitDataType, typename EnumType, size_t fields>
class Fields {
public:
    static_assert(std::is_unsigned_v<BitDataType>);
    static_assert(std::is_enum_v<EnumType>);
    static_assert(fields > 0);

    //! Needs exactly as many counts arguments as there are fields.
    /** But you don't need to use up the full space of bits.*/
    template <typename... Counts>
    explicit constexpr Fields(Counts... counts)
        : fields_lookup_{count<BitDataType>(), counts...} {
        static_assert(fields == sizeof...(counts));
    }

    template <std::unsigned_integral SourceDataType>
    constexpr void set(EnumType field, SourceDataType value) noexcept {
        raw_.set(fields_lookup_, field, value);
    }

    template <std::unsigned_integral TargetDataType = BitDataType>
    [[nodiscard]] constexpr TargetDataType get(EnumType field) const noexcept {
        return raw_.template get<TargetDataType>(fields_lookup_, field);
    }

private:
    FieldsLookup<fields> fields_lookup_;
    FieldsRaw<BitDataType, EnumType, fields> raw_;
};

//! Evaluated at runtime, a compile-time implementation doesn't seem easy.
/** Big endian means that the MSB (most significant bit) comes first in memory.*/
inline bool is_bigendian() noexcept {
    static const int one{1};
    return *reinterpret_cast<const uint8_t*>(&one) == uint8_t{0};
}
} // namespace mb::ul::bits

namespace mb::ul {
// NOLINTBEGIN
using Bits8 = bits::Array<8, uint8_t>;
using Bits16 = bits::Array<16, uint16_t>;
using Bits32 = bits::Array<32, uint32_t>;
using Bits64 = bits::Array<64, uint64_t>;
// NOLINTEND

// NOLINTBEGIN
//! Gets you the unsigned type with at least the required amount least_nr_of_bits of bits.
template <bits::Count least_nr_of_bits>
    requires(least_nr_of_bits <= 64)
using TypeLeast = std::conditional_t<
    (least_nr_of_bits > 8),
    std::conditional_t<
        (least_nr_of_bits > 16), std::conditional_t<(least_nr_of_bits > 32), uint_least64_t, uint_least32_t>,
        uint_least16_t>,
    uint_least8_t>;
// NOLINTEND
} // namespace mb::ul

#endif
