/** \file*/

#ifndef ENUM_BITSET_H_KJVIUEBVIUBVOIU6542IGOI
#define ENUM_BITSET_H_KJVIUEBVIUBVOIU6542IGOI

#include "bits.h"
#include "concepts.h"
#include "enum_cast.h"
#include "ul/config.h"
#include "ul/math.h"
#include <ostream>
#if __has_include(<concepts> )
#include <concepts>
#endif

#if __cpp_concepts && __cpp_lib_concepts
namespace mb::ul {
/** Wrap your enum EnumType into a type allowing logical combinations of the enum's elements by mapping (all) the enum
    values to different bit positions (starting with LSB). Note, that the EnumBitset automatically provides an
    optional-like empty value - the default contruction.
    Requirements: The enum must contain an element called `end` being the last one (having an enum value greater
    than everyone else). The enum value of `end` needs to be less than or equal the number of bits within type SizeType.
    The enum should not contain values smaller than 0 (at least you have to take care yourself, that you don't do
    manipulations with it with this class). It is allowed to contain gaps though.
    Special case: the end enum value could in principle be identified with 'empty' or 'no enum_value' or 0, but for
    performance reasons it isn't. But you can imagine it as such, and it is the default initialization also. In
    follow-up processing you can arrive there by calling clear or assigning {} again. Assigning end won't work.
    It is important to know that your very first enum value, despite being 0, does not represent an empty bitset here,
    but is a proper OR-combinable value.
    Note, you should not strive for supporting bitwise combinations for any enum type to use them flag-like. It makes
    sense to keep the ordinary enum a separate type, representing only one single value exclusively.*/
template <
    EnumConcept EnumType, std::integral SizeType = TypeLeast<enum_cast(EnumType::end)>,
    bool experimental_implicit = false>
    requires requires { EnumType::end; }
class EnumBitset {
public:
    static_assert(enum_cast(EnumType::end) <= sizeof(SizeType) * bits::bits_per_byte);

    using EnumT = EnumType;
    using UnderlyingT = SizeType;

    // needed for a pack expansion to terminate (presumably because of constexpr, if not yet default for constr.)
    constexpr EnumBitset() noexcept = default;

    //! Results in OR-combination.
    template <EnumConcept... EnumTypeArgs>
    constexpr explicit(!experimental_implicit) EnumBitset(EnumTypeArgs... enum_values) noexcept
        : bits_{enum_values_to_bitset(enum_values...)} {
    }

    // can't support this, as it would break {} assignment, which should be deduced as of type EnumBitset instead of
    // EnumType, at least too complicated, might need to provide all the constructors
    // EnumBitset& operator=(EnumType enum_value) noexcept;

    static constexpr EnumBitset from_bits(SizeType bits) noexcept {
        EnumBitset res;
        res.bits_ = bits;
        return res;
    }

    static constexpr EnumBitset from_range(const Range auto& c) noexcept {
        return EnumBitset::from_bits(std::apply(enum_values_to_bitset, c));
    }

    //! Cast back to enum type if ensured that only a single value is contained, cf. is_single.
    explicit(!experimental_implicit) operator EnumType() const {
        UL_EXPECT_THROW(math::is_power_of_two(bits_));
        return ul::enum_cast<EnumType>(bits_);
    }

    [[nodiscard]] constexpr bool is_single() const noexcept {
        return math::is_power_of_two(bits_);
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

    friend constexpr EnumBitset operator~(const EnumBitset& b) noexcept {
        return EnumBitset::from_bits(~b.bits_);
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

    friend constexpr std::ostream& operator<<(std::ostream& os, const EnumBitset& b) noexcept {
        // +? trick, ensuring number format, no ascii chars for int8_t e.g.
        os << +b.bits_;
        return os;
    }

private:
    SizeType bits_{};

    static constexpr SizeType enum_value_to_bitset(EnumType enum_value) noexcept {
        return static_cast<SizeType>(1u << enum_cast(enum_value));
    }

    // lambda because of use with apply
    static constexpr auto enum_values_to_bitset{[](EnumConcept auto... enum_values) noexcept -> SizeType {
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
