#include "bits.h"
#include "toolib/enum_cast.h"
#include "gtest/gtest.h"
#include <type_traits>


TEST(bits_num, various)
{
    static_assert(too::bits::count<uint8_t>() == 8);
    static_assert(too::bits::count<uint32_t>() == 32);
    static_assert(too::bits::count<uint64_t>() == 64);

    static_assert(too::bits::count<int8_t>() == 8);
    static_assert(too::bits::count<int32_t>() == 32);
    static_assert(too::bits::count<int64_t>() == 64);
}

TEST(bits_makeMask, empty)
{
    EXPECT_DEBUG_DEATH(too::bits::makeMask(5, 0), "ssert");
}

TEST(bits_makeMask, bitNr0)
{
    EXPECT_EQ(too::bits::makeMask(0, 1), 0b1u);
}

TEST(bits_makeMask, bitNr1)
{
    EXPECT_EQ(too::bits::makeMask(1, 1), 0b10u);
}

TEST(bits_makeMask, bitNr63)
{
    EXPECT_EQ(too::bits::makeMask<uint64_t>(63, 1), uint64_t(1) << uint64_t(63));
}

TEST(bits_makeMask, highestTwoBits)
{
    EXPECT_EQ(too::bits::makeMask<uint64_t>(62, 2), (uint64_t(1) << uint64_t(63)) | (uint64_t(1) << uint64_t(62)));
}

TEST(bits_makeMask, lowestTwoBits)
{
    EXPECT_EQ(too::bits::makeMask(0, 2), 0b11u);
}

TEST(bits_makeMask, someMiddleRangeOfBits)
{
    EXPECT_EQ(too::bits::makeMask(3, 6), 0b111111000u);
}

TEST(bits_makeMask, someMiddleRangeOfBits_castedToSufficientSmallerType)
{
    EXPECT_EQ(too::bits::makeMask<uint8_t>(2, 6),   0b11111100u);
    EXPECT_DEBUG_DEATH(too::bits::makeMask<uint8_t>(2, 7), "ssert");
    EXPECT_EQ(too::bits::makeMask<uint16_t>(2, 7), 0b111111100u);
}

TEST(bits_read, typical)
{
    EXPECT_EQ(too::bits::read(0b11000101100u, 2, 5), 0b01011u);
    const auto x = too::bits::readAndCast<uint8_t, uint16_t>(0b11000101100u, 2, 5);
    EXPECT_EQ(x, uint8_t{0b01011});
    EXPECT_DEBUG_DEATH(too::bits::makeMask<uint8_t>(2, 7), "ssert");
}

namespace
{
// workaround, because gtest understands comma between template parameters as comma for the gtest macro;
// so we need a template with one instead of two parameters
template<typename TargetType>
constexpr TargetType read_from16_testhelper(uint16_t data, too::bits::Idx idx, too::bits::Count count) noexcept
{
    return too::bits::readAndCast<TargetType, uint16_t>(data, idx, count);
}
} // namespace

TEST(bits_read, range_overflows)
{
    EXPECT_DEBUG_DEATH(too::bits::read<uint8_t>(0b10101100u, 2, 7), "ssert");
    EXPECT_DEBUG_DEATH(too::bits::read<uint8_t>(0b10101100u, 9, 0), "ssert");
    EXPECT_DEBUG_DEATH(too::bits::read<uint8_t>(0b10101100u, 0, 9), "ssert");
    EXPECT_DEBUG_DEATH(read_from16_testhelper<uint8_t>(0b11000101100u, 2, 9), "ssert");
}

TEST(bits_read, none)
{
    EXPECT_DEBUG_DEATH(too::bits::read<uint8_t>(0b10101100u, 2, 0), "ssert");
}

TEST(bits_read, one)
{
    EXPECT_EQ(too::bits::read<uint8_t>(0b10101100u, 2, 1), 1);
    EXPECT_EQ(too::bits::read<uint8_t>(0b10101000u, 2, 1), 0);
}

TEST(bits_read, last)
{
    EXPECT_EQ(too::bits::read<uint8_t>(0b10101100u, 7, 1), 1);
    EXPECT_EQ(too::bits::read<uint8_t>(0b00101100u, 7, 1), 0);
}

TEST(bits_read, all)
{
    EXPECT_EQ(too::bits::read<uint8_t>(0b10101100u, 0, 8), 0b10101100u);
}

TEST(bits_read, types)
{
    auto x = too::bits::read<uint8_t>(0b10101100u, 2, 1);
    static_assert(std::is_same_v<decltype(x), uint8_t>);
    auto x2 = too::bits::read<uint16_t>(0b10101100u, 2, 1);
    static_assert(std::is_same_v<decltype(x2), uint16_t>);
}

TEST(bits_read, types_cast)
{
    auto x = too::bits::readAndCast<uint8_t, uint16_t>(0b10101100u, 2, 1);
    static_assert(std::is_same_v<decltype(x), uint8_t>);
    auto x2 = too::bits::readAndCast<uint16_t, uint32_t>(0b10101100u, 2, 1);
    static_assert(std::is_same_v<decltype(x2), uint16_t>);
}

TEST(bits_write, typical)
{
    EXPECT_EQ(too::bits::write(0b11000101100u, 2, 5, 0b01011u), 0b11000101100u);
    EXPECT_EQ(too::bits::write(0b11000101100u, 2, 5, 0b11111u), 0b11001111100u);
    EXPECT_EQ(too::bits::write(0b11000101100u, 2, 5, 0b00000u), 0b11000000000u);
    EXPECT_EQ(too::bits::write(0b11000101100u, 2, 5, 0b10101u), 0b11001010100u);
}

TEST(bits_write, different_sizes)
{
    uint16_t x16{0b11000101100};
    uint8_t x8{0b101001};
    EXPECT_EQ(too::bits::write(x16, 0, 3, x8), 0b11000101001u);
    EXPECT_EQ(too::bits::write(x8, 0, 3, x16), 0b101100u);
}

TEST(bits_write, none)
{
    EXPECT_DEBUG_DEATH(too::bits::write(0b10101100u, 2, 0, 1), "ssert");
}

TEST(bits_write, one)
{
    EXPECT_EQ(too::bits::write(0b10101100u, 2, 1, 1), 0b10101100u);
    EXPECT_EQ(too::bits::write(0b10101100u, 2, 1, 0), 0b10101000u);
}

namespace
{
// workaround, because gtest understands comma between template parameters as comma for the gtest macro;
// so we need a template with one instead of two parameters
template <typename SourceType = uint8_t>
inline constexpr uint8_t write_to8_testhelper(uint8_t to, too::bits::Idx idx, too::bits::Count count, SourceType from) noexcept
{
    return too::bits::write<uint8_t, SourceType>(to, idx, count, from);
}
} // namespace

TEST(bits_write, range_overflows)
{
    EXPECT_DEBUG_DEATH(too::bits::write<uint16_t>(0, 0, 17, 0), "ssert");
    auto eat = too::bits::write<uint16_t>(0, 0, 16, 0);
    EXPECT_DEBUG_DEATH(write_to8_testhelper<uint8_t >(0, 0, 9, 0), "ssert");
    eat = write_to8_testhelper<uint8_t >(0, 0, 8, 0);
}

TEST(bits_write, last)
{
    EXPECT_EQ(too::bits::write<uint8_t>(0b10101100u, 7, 1, 0), 0b00101100u);
    EXPECT_EQ(too::bits::write<uint8_t>(0b10101100u, 7, 1, 1), 0b10101100u);
    EXPECT_EQ(too::bits::write(0b10101100u, 2, 1, 0), 0b10101000u);
    EXPECT_EQ(write_to8_testhelper<uint8_t>(0b10101100u, 7, 1, 0b1u), 0b10101100u);
    EXPECT_EQ(write_to8_testhelper<uint8_t>(0b10101100u, 7, 1, 0b0u), 0b00101100u);
}

TEST(bits_write, all)
{
    EXPECT_EQ(write_to8_testhelper<uint8_t>(0b10101100u, 0, 8, 0b11111111u), 0b11111111u);
    EXPECT_EQ(write_to8_testhelper<uint8_t>(0b10101100u, 0, 8, 0), 0);
    EXPECT_EQ(write_to8_testhelper<uint8_t>(0b11111111u, 0, 8, 0b10101100u), 0b10101100u);
    EXPECT_EQ(write_to8_testhelper<uint8_t>(0b00000000u, 0, 8, 0b10101100u), 0b10101100u);
}

TEST(bits_Field, basics)
{
    struct S
    {
        enum Field
        {
            octdigit,
            toggle,
            flags,
            end, // one behind last
        };
        too::bits::Fields<uint8_t, Field, too::as_number(Field::end)> bits_{3, 1, 4};
    } s;

    EXPECT_EQ(s.bits_.get(S::Field::octdigit), 0);
    s.bits_.set(S::Field::octdigit, 7);
    EXPECT_EQ(s.bits_.get(S::Field::octdigit), 7);
    EXPECT_EQ(s.bits_.get(S::Field::toggle), 0);
    EXPECT_EQ(s.bits_.get(S::Field::flags), 0);
    s.bits_.set(S::Field::octdigit, 0);
    EXPECT_EQ(s.bits_.get(S::Field::octdigit), 0);

    EXPECT_EQ(s.bits_.get(S::Field::toggle), 0);
    s.bits_.set(S::Field::toggle, 0);
    EXPECT_EQ(s.bits_.get(S::Field::toggle), 0);
    s.bits_.set(S::Field::toggle, 1);
    EXPECT_EQ(s.bits_.get(S::Field::octdigit), 0);
    EXPECT_EQ(s.bits_.get(S::Field::toggle), 1);
    EXPECT_EQ(s.bits_.get(S::Field::flags), 0);

    EXPECT_EQ(s.bits_.get(S::Field::flags), 0);
    s.bits_.set(S::Field::flags, 0b0001);
    EXPECT_EQ(s.bits_.get(S::Field::flags), 0b0001);
    EXPECT_EQ(s.bits_.get(S::Field::octdigit), 0);
    EXPECT_EQ(s.bits_.get(S::Field::toggle), 1);
    s.bits_.set(S::Field::flags, 0b0010);
    EXPECT_EQ(s.bits_.get(S::Field::flags), 0b0010);
    s.bits_.set(S::Field::flags, 0b0011);
    EXPECT_EQ(s.bits_.get(S::Field::flags), 0b0011);
    s.bits_.set(S::Field::flags, 0b1000);
    EXPECT_EQ(s.bits_.get(S::Field::flags), 0b1000);
}

TEST(bits_Field, experiment_char)
{
    struct S
    {
        enum Field
        {
            character,
            flags,
            end, // one behind last
        };
        too::bits::Fields<uint16_t, Field, too::as_number(Field::end)> bits_{8, 8};
    } s;

    EXPECT_EQ(s.bits_.get(S::Field::character), 0);
    s.bits_.set(S::Field::character, 'M');
    EXPECT_EQ(s.bits_.get(S::Field::character), 'M');
    s.bits_.set(S::Field::character, 255);
    EXPECT_EQ(s.bits_.get(S::Field::character), 255);
}

TEST(bits_Field, one)
{
    struct S
    {
        enum Field
        {
            number,
            end, // one behind last
        };
        too::bits::Fields<uint16_t, Field, too::as_number(Field::end)> bits_{16};
    } s;

    EXPECT_EQ(s.bits_.get(S::Field::number), 0);
    s.bits_.set(S::Field::number, 1234);
    EXPECT_EQ(s.bits_.get(S::Field::number), 1234);
}

TEST(bits_Field, some_capacity_left)
{
    struct S
    {
        enum Field
        {
            number,
            end, // one behind last
        };
        too::bits::Fields<uint32_t, Field, too::as_number(Field::end)> bits_{20};
    } s;

    EXPECT_EQ(s.bits_.get(S::Field::number), 0);
    s.bits_.set(S::Field::number, 1234);
    EXPECT_EQ(s.bits_.get(S::Field::number), 1234);
}

TEST(bits_Field, overflow)
{
    auto lambda = []() {
        struct S
        {
            enum Field
            {
                number,
                numberLarger,
                end, // one behind last
            };
            too::bits::Fields<uint32_t, Field, too::as_number(Field::end)> bits_{16, 17};
        } s;
    };
    EXPECT_THROW(lambda(), too::fail_fast);
}
