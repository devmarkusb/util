#include "ul/enum_bitset.h"
#include "ul/gtest_utils.h"
#include "ul/std/array.h"

#include "gtest/gtest.h"
#include <compare>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

using ul::EnumBitset;

enum class Ex1 : uint8_t {
    first,
    second,
    third,
    end
};

std::ostream& operator<<(std::ostream& os, Ex1 e) {
    switch (e) {
        case Ex1::first:
            os << "first";
            break;
        case Ex1::second:
            os << "second";
            break;
        case Ex1::third:
            os << "third";
            break;
        default:
            os << "?";
            break;
    }
    return os;
}

using Ex1Bitset = EnumBitset<Ex1>;

TEST(EnumBitSet, construction) {
    const Ex1Bitset bitset;
    EXPECT_FALSE(bitset & Ex1::first);
    EXPECT_FALSE(bitset);
    EXPECT_FALSE(bitset.count());
    EXPECT_FALSE(bitset.bits());

    Ex1Bitset bitset2{Ex1::first};
    EXPECT_TRUE(bitset2 & Ex1::first);
    EXPECT_TRUE((bitset2 & Ex1::first) == bitset2);
    EXPECT_TRUE(bitset2);
    EXPECT_EQ(bitset2.count(), 1);
    EXPECT_TRUE(bitset2.bits());

    Ex1Bitset bitset3{Ex1::first, Ex1::second};
    EXPECT_TRUE(bitset3 & Ex1::first);
    EXPECT_TRUE(bitset3 & Ex1::second);
    EXPECT_FALSE(bitset3 & Ex1::third);
    EXPECT_TRUE(bitset3);
    EXPECT_EQ(bitset3.count(), 2);
    EXPECT_TRUE(bitset3.bits());

    const Ex1Bitset bitset4{bitset3};
    EXPECT_EQ(bitset3, bitset4);

    bitset3.clear();
    EXPECT_FALSE(bitset3 & Ex1::first);
    EXPECT_FALSE(bitset3);
    EXPECT_FALSE(bitset3.count());
    EXPECT_FALSE(bitset3.bits());

    bitset2 = {};
    EXPECT_FALSE(bitset2 & Ex1::first);
    EXPECT_FALSE(bitset2);
    EXPECT_FALSE(bitset2.count());
    EXPECT_FALSE(bitset2.bits());
}

TEST(EnumBitSet, assignment) {
    const Ex1Bitset bitset{Ex1::third, Ex1::second};
    Ex1Bitset bitset2;

    bitset2 = bitset;
    EXPECT_EQ(bitset, bitset2);

    bitset2 = Ex1Bitset{Ex1::first};
    EXPECT_TRUE((bitset2 & Ex1::first) == bitset2);
}

TEST(EnumBitSet, operators) {
    const Ex1Bitset bitset;
    EXPECT_FALSE(bitset);

    const Ex1Bitset bitset2{Ex1::third};

    const Ex1Bitset bitset3{Ex1::third, Ex1::second};

    EXPECT_EQ(bitset2 & Ex1::second, bitset);
    EXPECT_EQ(Ex1::second & bitset2, bitset);
    EXPECT_EQ(bitset2 & bitset3, bitset2);

    EXPECT_EQ(bitset2 | Ex1::second, bitset3);
    EXPECT_EQ(Ex1::second | bitset2, bitset3);
    EXPECT_EQ(bitset2 | bitset3, bitset3);

    EXPECT_EQ(bitset2 ^ bitset3, Ex1Bitset{Ex1::second});
    EXPECT_EQ(bitset3 ^ Ex1::third, Ex1Bitset{Ex1::second});
    EXPECT_EQ(Ex1::third ^ bitset3, Ex1Bitset{Ex1::second});

    EXPECT_TRUE(~bitset3 & Ex1Bitset{Ex1::first});
    EXPECT_FALSE(~bitset3 & Ex1Bitset{Ex1::second});
    EXPECT_FALSE(~bitset3 & Ex1Bitset{Ex1::third});

    EXPECT_NE(bitset2, bitset3);

    EXPECT_EQ(~bitset, Ex1Bitset::from_bits(~static_cast<Ex1Bitset::UnderlyingT>(0)));
}

TEST(EnumBitSet, comparisons) {
    const Ex1Bitset bitset{Ex1::first};
    const Ex1Bitset bitset2{Ex1::second, Ex1::third};
    EXPECT_NE(bitset, bitset2);
    EXPECT_TRUE(bitset < bitset2 || bitset > bitset2);
    EXPECT_TRUE(bitset <= bitset2 || bitset >= bitset2);
    EXPECT_FALSE((bitset <=> bitset2) == std::strong_ordering::equal);

    const auto bitset3{bitset2};
    EXPECT_EQ(bitset3, bitset2);
    EXPECT_FALSE(bitset3 < bitset2 || bitset3 > bitset2);
    EXPECT_TRUE(bitset3 <= bitset2 && bitset3 >= bitset2);
    EXPECT_TRUE((bitset3 <=> bitset2) == std::strong_ordering::equal);
}

TEST(EnumBitSet, fromRange) {
    const Ex1Bitset bitset{Ex1::third, Ex1::second};
    constexpr auto t{ul::array::make(Ex1::third, Ex1::second)};
    const auto bitset2{Ex1Bitset::from_range(t)};
    EXPECT_EQ(bitset, bitset2);
}

TEST(EnumBitSet, fromBits) {
    const Ex1Bitset bitset{Ex1::third, Ex1::second};
    const auto bitset2{Ex1Bitset::from_bits(Ex1Bitset{Ex1::third}.bits() | Ex1Bitset{Ex1::second}.bits())};
    EXPECT_EQ(bitset, bitset2);
}

TEST(EnumBitSet, out) {
    ul::dump_test_name();
    const Ex1Bitset bitset{Ex1::third, Ex1::second};
    std::stringstream ss;
    ASSERT_NO_FATAL_FAILURE(ss << bitset);
    EXPECT_NE(std::stoull(ss.str()), 0);
    EXPECT_FALSE(bitset.is_single());
    std::cout << bitset << "\n";
    std::cout << "\n";
    const Ex1Bitset bitset2{Ex1::second};
    std::cout << bitset2 << "\n";
    ASSERT_TRUE(bitset2.is_single());
    const auto e{static_cast<Ex1Bitset::EnumT>(bitset2)};
    std::cout << e;
}

enum class SignedEnum : int8_t {
    neg = -1,
    pos = 1,
    end
};

TEST(EnumBitSet, signedEnum) {
    const EnumBitset<SignedEnum> bitset{SignedEnum::pos};
    EXPECT_TRUE(bitset);
    EXPECT_TRUE((bitset & SignedEnum::pos) == bitset);
}

enum class Ex2 : uint8_t {
    first,
    second,
    third = 7,
    end
};

enum class Ex3 : uint8_t {
    first,
    second,
    third = 8,
    end
};

TEST(EnumBitSet, size) {
    const EnumBitset<Ex2> bitset{Ex2::third};
    EXPECT_TRUE((bitset & Ex2::third) == bitset);

    const EnumBitset<Ex2, false, uint16_t> bitset2{Ex2::third};
    EXPECT_TRUE((bitset2 & Ex2::third) == bitset2);

    // not representable in uint8_t anymore - try, won't compile
#if 1
    using S = uint16_t;
#else
    using S = uint8_t;
#endif
    const EnumBitset<Ex3, false, S> bitset3{Ex3::third};
    EXPECT_TRUE((bitset3 & Ex3::third) == bitset3);
}

// to estimate what can happen if someone confuses the empty bitset with equally interpretable enum values
enum class Ex4 : uint8_t {
    unknown,
    not_set,
    first,
    second,
    third,
    end
};

TEST(EnumBitSet, emptyness) {
    const EnumBitset<Ex4> bitset;
    const EnumBitset<Ex4> bitset2{Ex4::unknown};
    EXPECT_NE(bitset, bitset2);
}
