/** \file*/

#ifndef ENUM_BITSET_H_KJVIUEBVIUBVOIU6542IGOI
#define ENUM_BITSET_H_KJVIUEBVIUBVOIU6542IGOI

#include "assert.h"
#include "bits.h"
#include "concepts.h"
#include "enum_cast.h"
#include "foundations.h"
#include "ul/config.h"
#if __has_include(<concepts> )
#include <concepts>
#endif

#if __cpp_concepts && __cpp_lib_concepts
namespace mb::ul {
/** Wrap your enum EnumType into a type allowing logical combinations of the enum's elements by mapping the enum
    enum_values to bit positions (starting with LSB). Requirements: The enum must contain an element called `end` being
    the last one (having a enum_value greater than everyone else). The enum_value of `end` needs to be less than or
    equal the number of bits within type SizeType plus 1. The enum should not contain enum_values smaller than 0 (at
    least you have to take care yourself, that you don't do manipulations with it with this class). It is allowed to
    contain gaps though.
    Special case: the end enum_value could in principle be identified with 'empty' or 'no enum_value' or 0, but for
    performance reasons it isn't. But you can view it as such, and it is the default initialization also. In follow-up
    processing you can arrive there by calling clear also. Assigning end won't work.
    Note, you should not strive for supporting bitwise combinations for any enum type to use them flag-like. It makes
    sense to keep that a separate type, representing only a single value exclusively.
    */
template <
    EnumConcept EnumType, std::integral SizeType = std::underlying_type_t<EnumType>, bool experimental_implicit = false>
    requires requires { EnumType::end; }
class EnumBitset {
public:
    static_assert(enum_cast(EnumType::end) <= sizeof(SizeType) * bits::bits_per_byte);

    using UnderlyingType = SizeType;

    // needed for a pack expansion to terminate (presumably because of constexpr, if not yet default for constr.)
    constexpr EnumBitset() noexcept = default;

    //! Results in OR-combination.
    template <EnumConcept... EnumTypeArgs>
    constexpr explicit(!experimental_implicit) EnumBitset(EnumTypeArgs... enum_values) noexcept
        : bits_{enum_values_to_bitset(enum_values...)} {
    }

    EnumBitset& operator=(EnumType enum_value) noexcept {
        bits_ = enum_value_to_bitset(enum_value);
        return *this;
    }

    static constexpr EnumBitset from_bits(SizeType bits) noexcept {
        EnumBitset res;
        res.bits_ = bits;
        return res;
    }

    static constexpr EnumBitset from_range(const Range auto& c) noexcept {
        return EnumBitset::from_bits(std::apply(enum_values_to_bitset, c));
    }

    constexpr void clear() noexcept {
        bits_ = {};
    }

    [[nodiscard]] constexpr explicit(!experimental_implicit) operator bool() const noexcept {
        return bits_;
    }

    [[nodiscard]] constexpr SizeType bits() const noexcept {
        return bits_;
    }

    [[nodiscard]] constexpr int count() const noexcept {
        return bits::count_set(bits_);
    }

    // note, because of this we can't support EnumBitset-EnumType comparisons, but that won't be apples-apples anyway
    constexpr auto operator<=>(const EnumBitset&) const noexcept = default;

    constexpr EnumBitset operator&=(const EnumBitset& rhs) noexcept {
        bits_ &= rhs.bits_;
        return *this;
    }

    constexpr EnumBitset operator|=(const EnumBitset& rhs) noexcept {
        bits_ |= rhs.bits_;
        return *this;
    }

    constexpr EnumBitset operator^=(const EnumBitset& rhs) noexcept {
        bits_ ^= rhs.bits_;
        return *this;
    }

    friend constexpr EnumBitset operator&(const EnumBitset& lhs, const EnumBitset& rhs) noexcept {
        return EnumBitset::from_bits(lhs.bits_ & rhs.bits_);
    }

    friend constexpr EnumBitset operator|(const EnumBitset& lhs, const EnumBitset& rhs) noexcept {
        return EnumBitset::from_bits(lhs.bits_ | rhs.bits_);
    }

    friend constexpr EnumBitset operator^(const EnumBitset& lhs, const EnumBitset& rhs) noexcept {
        return EnumBitset::from_bits(lhs.bits_ ^ rhs.bits_);
    }

    friend constexpr EnumBitset operator~(const EnumBitset& a) noexcept {
        return EnumBitset::from_bits(~a.bits_);
    }

    constexpr EnumBitset operator&=(const EnumType& rhs) noexcept {
        bits_ &= enum_value_to_bitset(rhs);
        return *this;
    }

    constexpr EnumBitset operator|=(const EnumType& rhs) noexcept {
        bits_ |= enum_value_to_bitset(rhs);
        return *this;
    }

    constexpr EnumBitset operator^=(const EnumType& rhs) noexcept {
        bits_ ^= enum_value_to_bitset(rhs);
        return *this;
    }

    friend constexpr EnumBitset operator&(const EnumBitset& lhs, const EnumType& rhs) noexcept {
        return EnumBitset::from_bits(lhs.bits_ & EnumBitset::enum_value_to_bitset(rhs));
    }

    friend constexpr EnumBitset operator|(const EnumBitset& lhs, const EnumType& rhs) noexcept {
        return EnumBitset::from_bits(lhs.bits_ | EnumBitset::enum_value_to_bitset(rhs));
    }

    friend constexpr EnumBitset operator^(const EnumBitset& lhs, const EnumType& rhs) noexcept {
        return EnumBitset::from_bits(lhs.bits_ ^ EnumBitset::enum_value_to_bitset(rhs));
    }

    friend constexpr EnumBitset operator&(const EnumType& lhs, const EnumBitset& rhs) noexcept {
        return rhs & lhs;
    }

    friend constexpr EnumBitset operator|(const EnumType& lhs, const EnumBitset& rhs) noexcept {
        return rhs | lhs;
    }

    friend constexpr EnumBitset operator^(const EnumType& lhs, const EnumBitset& rhs) noexcept {
        return rhs ^ lhs;
    }

private:
    SizeType bits_{};

    static constexpr SizeType enum_value_to_bitset(EnumType enum_value) noexcept {
        return static_cast<SizeType>(1u << enum_cast(enum_value));
    }

    // lambda because of use with apply
    static constexpr auto enum_values_to_bitset{[](auto... enum_values) noexcept -> SizeType {
        using FirstType = std::tuple_element_t<0, std::tuple<decltype(enum_values)...>>;
        static_assert(EnumConcept<FirstType>);
        static_assert(AllOf<FirstType, decltype(enum_values)...>);
        return ((enum_value_to_bitset(enum_values)) | ...);
    }};
};
} // namespace mb::ul
#endif

UL_HEADER_END

#endif
