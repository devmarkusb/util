#include "ul/enum_bitset.h"
#include "ul/std/array.h"

#include "gtest/gtest.h"
#include <array>

using ul::EnumBitset;

enum class Ex1 : uint8_t {
    first,
    second,
    third,
    end
};

using Ex1Bitset = EnumBitset<Ex1>;

TEST(EnumBitSet, construction) {
    Ex1Bitset bitset;
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
}

TEST(EnumBitSet, operators) {
    Ex1Bitset bitset;
    EXPECT_FALSE(bitset);

    Ex1Bitset bitset2{Ex1::third};

    Ex1Bitset bitset3{Ex1::third, Ex1::second};

    EXPECT_EQ(bitset2 & Ex1::second, bitset);
    EXPECT_EQ(bitset2 & bitset3, bitset2);

    EXPECT_EQ(bitset2 | Ex1::second, bitset3);
    EXPECT_EQ(bitset2 | bitset3, bitset3);

    EXPECT_NE(bitset2, bitset3);

    EXPECT_EQ(~bitset, Ex1Bitset::from_bits(~static_cast<Ex1Bitset::UnderlyingType>(0)));
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

enum class SignedEnum : int8_t {
    neg = -1,
    pos = 1,
    end
};

TEST(EnumBitSet, signedEnum) {
    EnumBitset<SignedEnum> bitset{SignedEnum::pos};
    EXPECT_TRUE(bitset);
    EXPECT_TRUE((bitset & SignedEnum::pos) == bitset);
}
