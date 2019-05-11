// Markus Borris, 2019
// This file is part of tfl library.

//!
/**
 */
//! \file

#ifndef BITS_H_sljkhvnx2hng7835ghm3g7h3
#define BITS_H_sljkhvnx2hng7835ghm3g7h3

#include "toolib/assert.h"
#include "toolib/enum_cast.h"
#include <array>
#include <cstdint>
#include <limits>
#include <numeric>


namespace too
{
namespace bits
{
using Idx = uint16_t;
using Count = Idx;
using Diff = int;

//! \Returns number of bits of the arbitrary \param Type.
template<typename Type>
constexpr Count count() noexcept
{
    return 8 * sizeof(Type);
}

constexpr uint32_t countSet(uint32_t data) noexcept
{
    // Hamming Weight algorithm
    data = data - ((data >> 1) & 0x55555555);
    data = (data & 0x33333333) + ((data >> 2) & 0x33333333);
    return (((data + (data >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

template <typename SourceType, typename TargetType = SourceType>
constexpr TargetType set(SourceType from, Idx idx) noexcept
{
    TOO_EXPECT(idx < too::bits::count<TargetType>());
    return from | (TargetType{1} << idx);
}

template <typename SourceType, typename TargetType = SourceType>
constexpr TargetType unset(SourceType from, Idx idx) noexcept
{
    TOO_EXPECT(idx < too::bits::count<TargetType>());
    return from & ~(TargetType{1} << idx);
}

template <typename SourceType, typename TargetType = SourceType>
constexpr TargetType toggle(SourceType from, Idx idx) noexcept
{
    TOO_EXPECT(idx < too::bits::count<TargetType>());
    return from ^ (TargetType{1} << idx);
}

template <typename SourceType, typename TargetType = SourceType>
constexpr TargetType check(SourceType from, Idx idx) noexcept
{
    TOO_EXPECT(idx < too::bits::count<SourceType>());
    return (from >> idx) & TargetType{1};
}

template <typename SourceType, typename ChangeBitSourceType, typename TargetType = SourceType>
constexpr TargetType change(SourceType from, Idx idx, ChangeBitSourceType x) noexcept
{
    TOO_EXPECT(idx < too::bits::count<TargetType>());
    const SourceType newbit = !!x; // ensure 1 or 0
    return from ^ ((-newbit ^ from) & (TargetType{1} << idx));
}

template <typename SourceType, typename TargetType = SourceType>
constexpr TargetType setMask(SourceType from, SourceType mask) noexcept
{
    return from | mask;
}

template <typename SourceType, typename TargetType = SourceType>
constexpr TargetType unsetMask(SourceType from, SourceType mask) noexcept
{
    return from & ~mask;
}

template <typename SourceType, typename TargetType = SourceType>
constexpr TargetType toggleMask(SourceType from, SourceType mask) noexcept
{
    return from ^ mask;
}

template <typename SourceType, typename TargetType = SourceType>
constexpr TargetType checkAllOfMask(SourceType from, SourceType mask) noexcept
{
    return (from & mask) == mask;
}

template <typename SourceType, typename TargetType = SourceType>
constexpr TargetType checkAnyOfMask(SourceType from, SourceType mask) noexcept
{
    return from & mask;
}

//! Makes a bit mask of \param count 1's, > 0 starting at 0-based index \param idx.
/** \Param TargetType is typically chosen to be uintX_t, that is unsigned with some arbitrary bit count.
    You can think of the index starting at the LSB (least significant bit, which comes last in the memory order
    of big endian, but endianess doesn't matter considering the realm of this function alone).*/
template <typename TargetType = uint64_t>
constexpr TargetType setRange(Idx idx, Count count) noexcept
{
    static_assert(std::is_unsigned_v<TargetType>);
    TOO_EXPECT(count > 0);
    TOO_EXPECT(idx + count <= too::bits::count<TargetType>());

    return ((TargetType{1} << count) - 1) << idx;
}

//! Reads \param count > 0 bits of \param from starting at 0-based index \param idx (0 is LSB).
template <typename SourceType>
constexpr SourceType read(SourceType from, Idx idx, Count count) noexcept
{
    TOO_EXPECT(count > 0);
    TOO_EXPECT(idx + count <= too::bits::count<SourceType>());

    return (from & too::bits::setRange<SourceType>(idx, count)) >> idx;
}

//! Like read. But here you can also opt for a different target type.
/** As typically reading out a subset of bits results in a smaller type.*/
template <typename TargetType, typename SourceType /*>= TargetType*/>
constexpr TargetType readAndCast(SourceType from, Idx idx, Count count) noexcept
{
    TOO_EXPECT(count > 0);
    TOO_EXPECT(idx + count <= too::bits::count<SourceType>());
    TOO_EXPECT(count <= too::bits::count<TargetType>());

    return static_cast<TargetType>((from & too::bits::setRange<SourceType>(idx, count)) >> idx);
}

//! Write \param count > 0 bits of \param from into \param to starting at 0-based index \param idx there (0 is LSB).
template <typename TargetType, typename SourceType>
constexpr TargetType write(TargetType to, Idx idx, Count count, SourceType from) noexcept
{
    TOO_EXPECT(count > 0);
    TOO_EXPECT(idx + count <= too::bits::count<TargetType>());
    TOO_EXPECT(count <= too::bits::count<SourceType>());

    return (to & (~too::bits::setRange(idx, count))) | (from << idx);
}

//! If 64 bits aren't sufficient, this is the type to go.
/** Choose your desired bit count \param bits freely. The underlying type of the parts being glued together might
    also be chosen: \param BaseType.*/
template <Count bits, typename BaseType = uint32_t>
class Array
{
public:
    void set(Idx idx) noexcept
    {
        TOO_EXPECT(idx < bits);
        array[N(idx)] |= partBit(I(idx));
    }

    void reset(Idx idx) noexcept
    {
        TOO_EXPECT(idx < bits);
        array[N(idx)] &= ~partBit(I(idx));
    }

    void reset() noexcept
    {
        array.fill({});
    }

    bool isSet(Idx idx) const noexcept
    {
        TOO_EXPECT(idx < bits);
        return array[N(idx)] & partBit(I(idx));
    }

private:
    constexpr static Count partsCount{static_cast<Count>
        ((bits + (too::bits::count<BaseType>() - Count{1})) / too::bits::count<BaseType>())};
    std::array<BaseType, partsCount> array{};
    
    Idx N(Idx idx) const noexcept
    {
        return idx / too::bits::count<BaseType>();
    }

    Idx I(Idx idx) const noexcept
    {
        return idx % too::bits::count<BaseType>();
    }

    BaseType partBit(Idx n) const noexcept
    {
        return BaseType{1} << n;
    }

    template <Count bits_, typename BaseType_>
    friend Array<bits_, BaseType_> operator&(
            const Array<bits_, BaseType_>& lhs, const Array<bits_, BaseType_>& rhs) noexcept;
    template <Count bits_, typename BaseType_>
    friend Array<bits_, BaseType_> operator|(
            const Array<bits_, BaseType_>& lhs, const Array<bits_, BaseType_>& rhs) noexcept;
};

template <Count bits, typename BaseType = uint32_t>
Array<bits, BaseType> operator&(const Array<bits, BaseType>& lhs, const Array<bits, BaseType>& rhs) noexcept
{
    Array<bits, BaseType> ret;
    for (size_t i = 0; i < Array<bits, BaseType>::partsCount; ++i)
        ret.array[i] = lhs.array[i] & rhs.array[i];
    return ret;
}

template <Count bits, typename BaseType = uint32_t>
Array<bits, BaseType> operator|(const Array<bits, BaseType>& lhs, const Array<bits, BaseType>& rhs) noexcept
{
    Array<bits, BaseType> ret;
    for (size_t i = 0; i < Array<bits, BaseType>::partsCount; ++i)
        ret.array[i] = lhs.array[i] | rhs.array[i];
    return ret;
}

//! Needed for FieldsRaw.
template <size_t fields>
struct FieldsLookup
{
    std::array<Count, fields> counts_; // count of bits of each field
    std::array<Count, fields> indices_; // indices where field sits

    template <typename... Counts>
    explicit FieldsLookup(Count maxCount, Counts... counts)
    {
        static_assert(fields == sizeof...(counts));

        counts_ = {static_cast<Count>(counts)...};
        indices_[0] = {};
        std::partial_sum(std::begin(counts_), std::end(counts_) - 1, std::begin(indices_) + 1);
        // runtime check as long as we don't have a compiletime partial_sum
        TOO_ENSURE_THROW(indices_.back() + counts_.back() <= maxCount);
    }
};

//! Just if can't use Fields (cf.) because you need it to be no larger than sizeof(BitDataType).
template <typename BitDataType, typename EnumType, size_t fields>
class FieldsRaw
{
public:
    static_assert(std::is_unsigned_v<BitDataType>);
    static_assert(std::is_enum_v<EnumType>);
    static_assert(fields > 0);

    template <typename SourceDataType>
    constexpr void set(const FieldsLookup<fields>& fieldsLookup, EnumType field, SourceDataType value) noexcept
    {
        const auto fieldnr{as_number(field)};
        data_ = write<BitDataType, SourceDataType>(data_, fieldsLookup.indices_[fieldnr], fieldsLookup.counts_[fieldnr], value);
    }

    template <typename TargetDataType = BitDataType>
    constexpr TargetDataType get(const FieldsLookup<fields>& fieldsLookup, EnumType field) const noexcept
    {
        const auto fieldnr{as_number(field)};
        return readAndCast<TargetDataType, BitDataType>(data_, fieldsLookup.indices_[fieldnr], fieldsLookup.counts_[fieldnr]);
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
    too::bits::Field<uint32_t, Fields, fields_end> bits_{16, 8, 8};

    void f()
    {
        bits_.set(Fields::second, 255);
        assert(bits_.get(Fields::second) == 255);
    }
    */
template <typename BitDataType, typename EnumType, size_t fields>
class Fields
{
public:
    static_assert(std::is_unsigned_v<BitDataType>);
    static_assert(std::is_enum_v<EnumType>);
    static_assert(fields > 0);

    //! Needs exactly as many \param counts arguments as there are fields.
    /** But you don't need to use up the full space of bits.*/
    template <typename... Counts>
    explicit constexpr Fields(Counts... counts)
        : fieldsLookup_{count<BitDataType>(), counts...}
    {
        static_assert(fields == sizeof...(counts));
    }

    template <typename SourceDataType>
    constexpr void set(EnumType field, SourceDataType value) noexcept
    {
        raw_.set(fieldsLookup_, field, value);
    }

    template <typename TargetDataType = BitDataType>
    constexpr TargetDataType get(EnumType field) const noexcept
    {
        return raw_.template get<TargetDataType>(fieldsLookup_, field);
    }

private:
    FieldsLookup<fields> fieldsLookup_;
    FieldsRaw<BitDataType, EnumType, fields> raw_;
};

//! Evaluated at runtime, a compile-time implementation doesn't seem easy.
/** Big endian means that the MSB (most significant bit) comes first in memory.*/
inline bool is_bigendian() noexcept
{
    static const int one{1};
    return *reinterpret_cast<const uint8_t*>(&one) == uint8_t{0};
}
} // bits
} // too
#endif
