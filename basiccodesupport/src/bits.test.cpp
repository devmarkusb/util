#include "ul/bits.h"
#include "ul/assert.h"
#include "ul/enum_cast.h"
#include "ul/ignore_unused.h"
#include "gtest/gtest.h"
#include <type_traits>

namespace ul = mb::ul;

// NOLINTBEGIN

TEST(bits_count, various)
{
    static_assert(ul::bits::count<uint8_t>() == 8);
    static_assert(ul::bits::count<uint32_t>() == 32);
    static_assert(ul::bits::count<uint64_t>() == 64);

    static_assert(ul::bits::count<int8_t>() == 8);
    static_assert(ul::bits::count<int32_t>() == 32);
    static_assert(ul::bits::count<int64_t>() == 64);
}

TEST(bits_countSetBits, various)
{
    EXPECT_EQ(ul::bits::countSet(0), 0);
    EXPECT_EQ(ul::bits::countSet(1), 1);
    EXPECT_EQ(ul::bits::countSet(0b11), 2);
    EXPECT_EQ(ul::bits::countSet(0b101), 2);
    EXPECT_EQ(ul::bits::countSet(0b111), 3);
    EXPECT_EQ(ul::bits::countSet(0b1111110), 6);
    EXPECT_EQ(ul::bits::countSet(0b1111110111), 9);
    EXPECT_EQ(ul::bits::countSet(0b10000000000), 1);
}

TEST(bits_set, various)
{
    EXPECT_EQ(ul::bits::set(0b000u, 0u), 0b001);
    EXPECT_EQ(ul::bits::set(0b000u, 1u), 0b010);
    EXPECT_EQ(ul::bits::set<uint8_t>(0b1, 7), 0b10000001);
    EXPECT_DEBUG_DEATH(ul::bits::set<uint8_t>(0b1, 8), ul::death_assert_regex);
}

TEST(bits_unset, various)
{
    EXPECT_EQ(ul::bits::unset(0b111, 0), 0b110);
    EXPECT_EQ(ul::bits::unset(0b111, 1), 0b101);
    EXPECT_EQ(ul::bits::unset<uint8_t>(1 << 7, 7), 0);
    EXPECT_DEBUG_DEATH(ul::bits::unset<uint8_t>(0b1, 8), ul::death_assert_regex);
}

TEST(bits_toggle, various)
{
    EXPECT_EQ(ul::bits::toggle(0b111, 0), 0b110);
    EXPECT_EQ(ul::bits::toggle(0b110, 0), 0b111);
    EXPECT_EQ(ul::bits::toggle(0b101, 1), 0b111);
    EXPECT_EQ(ul::bits::toggle<uint8_t>(1 << 7, 7), 0);
    EXPECT_DEBUG_DEATH(ul::bits::unset<uint8_t>(0b1, 8), ul::death_assert_regex);
}

TEST(bits_check, various)
{
    EXPECT_EQ(ul::bits::check(0b111, 0), 1);
    EXPECT_EQ(ul::bits::check(0b110, 0), 0);
    EXPECT_EQ(ul::bits::check(0b101, 1), 0);
    EXPECT_EQ(ul::bits::check<uint8_t>(1 << 7, 7), 1);
    EXPECT_DEBUG_DEATH(ul::bits::check<uint8_t>(0b1, 8), ul::death_assert_regex);
}

TEST(bits_change, various)
{
    EXPECT_EQ(ul::bits::change(0b111, 0, 1), 0b111);
    EXPECT_EQ(ul::bits::change(0b110, 0, 1), 0b111);
    EXPECT_EQ(ul::bits::change(0b111, 0, 0), 0b110);
    EXPECT_EQ(ul::bits::change(0b110, 0, 0), 0b110);
    EXPECT_EQ(ul::bits::change(0b101, 1, 1), 0b111);
    EXPECT_EQ(ul::bits::change<uint8_t>(1 << 7, 7, 0), 0);
    EXPECT_DEBUG_DEATH(ul::bits::change<uint8_t>(0b1, 8, 1), ul::death_assert_regex);
}

TEST(bits_setMask, various)
{
    EXPECT_EQ(ul::bits::setMask(0b000, 0b001), 0b001);
    EXPECT_EQ(ul::bits::setMask(0b000, 0b010), 0b010);
    EXPECT_EQ(ul::bits::setMask(0b000, 0b101), 0b101);
    EXPECT_EQ(ul::bits::setMask<uint8_t>(0b1, 1 << 7), (1 << 7) | 0b1);
}

TEST(bits_unsetMask, various)
{
    EXPECT_EQ(ul::bits::unsetMask(0b111, 0b001), 0b110);
    EXPECT_EQ(ul::bits::unsetMask(0b111, 0b010), 0b101);
    EXPECT_EQ(ul::bits::unsetMask(0b111, 0b101), 0b010);
    EXPECT_EQ(ul::bits::unsetMask<uint8_t>((1 << 7) | 0b1, 1 << 7), 0b1);
}

TEST(bits_toggleMask, various)
{
    EXPECT_EQ(ul::bits::toggleMask(0b111, 0b001), 0b110);
    EXPECT_EQ(ul::bits::toggleMask(0b110, 0b001), 0b111);
    EXPECT_EQ(ul::bits::toggleMask(0b111, 0b101), 0b010);
    EXPECT_EQ(ul::bits::toggleMask<uint8_t>((1 << 7) | 0b1, 1 << 7), 0b1);
}

TEST(bits_checkAllOfMask, various)
{
    EXPECT_TRUE(ul::bits::checkAllOfMask(0b111, 0b101));
    EXPECT_TRUE(ul::bits::checkAllOfMask(0b101, 0b101));
    EXPECT_FALSE(ul::bits::checkAllOfMask(0b110, 0b101));
    EXPECT_FALSE(ul::bits::checkAllOfMask(0b010, 0b101));
    EXPECT_TRUE(ul::bits::checkAllOfMask(0b010, 0b010));
    EXPECT_FALSE(ul::bits::checkAllOfMask(0b101, 0b010));
}

TEST(bits_checkAnyOfMask, various)
{
    EXPECT_TRUE(ul::bits::checkAnyOfMask(0b111, 0b101));
    EXPECT_TRUE(ul::bits::checkAnyOfMask(0b101, 0b101));
    EXPECT_TRUE(ul::bits::checkAnyOfMask(0b100, 0b101));
    EXPECT_TRUE(ul::bits::checkAnyOfMask(0b001, 0b101));
    EXPECT_TRUE(ul::bits::checkAnyOfMask(0b110, 0b101));
    EXPECT_FALSE(ul::bits::checkAnyOfMask(0b010, 0b101));
    EXPECT_TRUE(ul::bits::checkAnyOfMask(0b010, 0b010));
    EXPECT_FALSE(ul::bits::checkAnyOfMask(0b101, 0b010));
}

TEST(bits_setRange, empty)
{
    EXPECT_DEBUG_DEATH(ul::bits::setRange(5, 0), ul::death_assert_regex);
}

TEST(bits_setRange, bitNr0)
{
    EXPECT_EQ(ul::bits::setRange(0, 1), 0b1u);
}

TEST(bits_setRange, bitNr1)
{
    EXPECT_EQ(ul::bits::setRange(1, 1), 0b10u);
}

TEST(bits_setRange, bitNr63)
{
    EXPECT_EQ(ul::bits::setRange<uint64_t>(63, 1), uint64_t(1) << uint64_t(63));
}

TEST(bits_setRange, highestTwoBits)
{
    EXPECT_EQ(ul::bits::setRange<uint64_t>(62, 2), (uint64_t(1) << uint64_t(63)) | (uint64_t(1) << uint64_t(62)));
}

TEST(bits_setRange, lowestTwoBits)
{
    EXPECT_EQ(ul::bits::setRange(0, 2), 0b11u);
}

TEST(bits_setRange, someMiddleRangeOfBits)
{
    EXPECT_EQ(ul::bits::setRange(3, 6), 0b111111000u);
}

TEST(bits_setRange, someMiddleRangeOfBits_castedToSufficientSmallerType)
{
    EXPECT_EQ(ul::bits::setRange<uint8_t>(2, 6), 0b11111100u);
    EXPECT_DEBUG_DEATH(ul::bits::setRange<uint8_t>(2, 7), ul::death_assert_regex);
    EXPECT_EQ(ul::bits::setRange<uint16_t>(2, 7), 0b111111100u);
}

TEST(bits_read, typical)
{
    EXPECT_EQ(ul::bits::read(0b11000101100u, 2, 5), 0b01011u);
    const auto x = ul::bits::readAndCast<uint8_t>(uint16_t{0b11000101100u}, 2, 5);
    EXPECT_EQ(x, uint8_t{0b01011});
    EXPECT_DEBUG_DEATH(ul::bits::setRange<uint8_t>(2, 7), ul::death_assert_regex);
}

namespace
{
// workaround, because gtest understands comma between template parameters as comma for the gtest macro;
// so we need a template with one instead of two parameters
template <typename TargetType>
constexpr TargetType read_from16_testhelper(uint16_t data, ul::bits::Idx idx, ul::bits::Count count) noexcept
{
    return ul::bits::readAndCast<TargetType>(data, idx, count);
}
} // namespace

TEST(bits_read, range_overflows)
{
    EXPECT_DEBUG_DEATH(ul::bits::read<uint8_t>(0b10101100u, 2, 7), ul::death_assert_regex);
    EXPECT_DEBUG_DEATH(ul::bits::read<uint8_t>(0b10101100u, 9, 0), ul::death_assert_regex);
    EXPECT_DEBUG_DEATH(ul::bits::read<uint8_t>(0b10101100u, 0, 9), ul::death_assert_regex);
    EXPECT_DEBUG_DEATH(read_from16_testhelper<uint8_t>(0b11000101100u, 2, 9), ul::death_assert_regex);
}

TEST(bits_read, none)
{
    EXPECT_DEBUG_DEATH(ul::bits::read<uint8_t>(0b10101100u, 2, 0), ul::death_assert_regex);
}

TEST(bits_read, one)
{
    EXPECT_EQ(ul::bits::read<uint8_t>(0b10101100u, 2, 1), 1);
    EXPECT_EQ(ul::bits::read<uint8_t>(0b10101000u, 2, 1), 0);
}

TEST(bits_read, last)
{
    EXPECT_EQ(ul::bits::read<uint8_t>(0b10101100u, 7, 1), 1);
    EXPECT_EQ(ul::bits::read<uint8_t>(0b00101100u, 7, 1), 0);
}

TEST(bits_read, all)
{
    EXPECT_EQ(ul::bits::read<uint8_t>(0b10101100u, 0, 8), 0b10101100u);
}

TEST(bits_read, types)
{
    auto x = ul::bits::read<uint8_t>(0b10101100u, 2, 1);
    static_assert(std::is_same_v<decltype(x), uint8_t>);
    auto x2 = ul::bits::read<uint16_t>(0b10101100u, 2, 1);
    static_assert(std::is_same_v<decltype(x2), uint16_t>);

    // the actually interesting one
    uint8_t u8{0b10101100u};
    auto x3 = ul::bits::read(u8, 2, 1);
    static_assert(std::is_same_v<decltype(x3), uint8_t>);
}

TEST(bits_read, types_cast)
{
    auto x = ul::bits::readAndCast<uint8_t, uint16_t>(0b10101100u, 2, 1);
    static_assert(std::is_same_v<decltype(x), uint8_t>);
    auto x2 = ul::bits::readAndCast<uint16_t>(uint32_t{0b10101100u}, 2, 1);
    static_assert(std::is_same_v<decltype(x2), uint16_t>);
}

TEST(bits_write, typical)
{
    EXPECT_EQ(ul::bits::write(0b11000101100u, 2, 5, 0b01011u), 0b11000101100u);
    EXPECT_EQ(ul::bits::write(0b11000101100u, 2, 5, 0b11111u), 0b11001111100u);
    EXPECT_EQ(ul::bits::write(0b11000101100u, 2, 5, 0b00000u), 0b11000000000u);
    EXPECT_EQ(ul::bits::write(0b11000101100u, 2, 5, 0b10101u), 0b11001010100u);
}

TEST(bits_write, different_sizes)
{
    uint16_t x16{0b11000101100};
    uint8_t x8{0b101001};
    EXPECT_EQ(ul::bits::write(x16, 0, 3, x8), 0b11000101001u);
    EXPECT_EQ(ul::bits::write(x8, 0, 3, x16), 0b101100u);
}

TEST(bits_write, none)
{
    EXPECT_DEBUG_DEATH(ul::bits::write(0b10101100u, 2, 0, 1), ul::death_assert_regex);
}

TEST(bits_write, one)
{
    EXPECT_EQ(ul::bits::write(0b10101100u, 2, 1, 1), 0b10101100u);
    EXPECT_EQ(ul::bits::write(0b10101100u, 2, 1, 0), 0b10101000u);
}

namespace
{
// workaround, because gtest understands comma between template parameters as comma for the gtest macro;
// so we need a template with one instead of two parameters
template <typename SourceType = uint8_t>
constexpr uint8_t write_to8_testhelper(uint8_t to, ul::bits::Idx idx, ul::bits::Count count, SourceType from) noexcept
{
    return ul::bits::write<uint8_t, SourceType>(to, idx, count, from);
}
} // namespace

TEST(bits_write, range_overflows)
{
    EXPECT_DEBUG_DEATH(ul::bits::write<uint16_t>(0, 0, 17, 0), ul::death_assert_regex);
    auto _ = ul::bits::write<uint16_t>(0, 0, 16, 0);
    EXPECT_DEBUG_DEATH(write_to8_testhelper<uint8_t>(0, 0, 9, 0), ul::death_assert_regex);
    _ = write_to8_testhelper<uint8_t>(0, 0, 8, 0);
    ul::ignore_unused(_);
}

TEST(bits_write, last)
{
    EXPECT_EQ(ul::bits::write<uint8_t>(0b10101100u, 7, 1, 0), 0b00101100u);
    EXPECT_EQ(ul::bits::write<uint8_t>(0b10101100u, 7, 1, 1), 0b10101100u);
    EXPECT_EQ(ul::bits::write(0b10101100u, 2, 1, 0), 0b10101000u);
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

TEST(bits_BitArray, construct)
{
    ul::bits::Array<128, uint64_t> a1;
    for (ul::bits::Idx i = 0; i < 128; ++i)
        EXPECT_FALSE(a1.isSet(i));

    ul::bits::Array<2> a2;
    for (ul::bits::Idx i = 0; i < 2; ++i)
        EXPECT_FALSE(a2.isSet(i));

    ul::bits::Array<9, uint8_t> a3;
    for (ul::bits::Idx i = 0; i < 9; ++i)
        EXPECT_FALSE(a3.isSet(i));
}

TEST(bits_BitArray, set)
{
    ul::bits::Array<42> a1;
    a1.set(41);
    EXPECT_TRUE(a1.isSet(41));
    EXPECT_DEBUG_DEATH(a1.set(42), ul::death_assert_regex);

    a1.set(0);
    EXPECT_TRUE(a1.isSet(0));

    a1.set(31);
    EXPECT_TRUE(a1.isSet(31));
    a1.set(32);
    EXPECT_TRUE(a1.isSet(32));
    a1.set(33);
    EXPECT_TRUE(a1.isSet(33));
}

TEST(bits_BitArray, reset)
{
    ul::bits::Array<42> a1;
    a1.set(41);
    a1.set(0);
    a1.set(31);
    a1.set(32);
    a1.set(33);

    a1.reset(41);
    EXPECT_FALSE(a1.isSet(41));
    EXPECT_TRUE(a1.isSet(0));
    EXPECT_TRUE(a1.isSet(31));
    EXPECT_TRUE(a1.isSet(32));
    EXPECT_TRUE(a1.isSet(33));
    a1.reset(0);
    EXPECT_FALSE(a1.isSet(41));
    EXPECT_FALSE(a1.isSet(0));
    EXPECT_TRUE(a1.isSet(31));
    EXPECT_TRUE(a1.isSet(32));
    EXPECT_TRUE(a1.isSet(33));
    a1.reset();
    for (ul::bits::Idx i = 0; i < 42; ++i)
        EXPECT_FALSE(a1.isSet(i));
}

//TEST(bits_BitArray, TODO_and_or)
//{
//}

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

        ul::bits::Fields<uint8_t, Field, ul::as_number(Field::end)> bits_{3, 1, 4};
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

        ul::bits::Fields<uint16_t, Field, ul::as_number(Field::end)> bits_{8, 8};
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

        ul::bits::Fields<uint16_t, Field, ul::as_number(Field::end)> bits_{16};
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

        ul::bits::Fields<uint32_t, Field, ul::as_number(Field::end)> bits_{20};
    } s;

    EXPECT_EQ(s.bits_.get(S::Field::number), 0);
    s.bits_.set(S::Field::number, 1234);
    EXPECT_EQ(s.bits_.get(S::Field::number), 1234);
}

TEST(bits_Field, overflow)
{
    auto lambda = []()
    {
        struct S
        {
            enum Field
            {
                number,
                numberLarger,
                end, // one behind last
            };
            ul::bits::Fields<uint32_t, Field, ul::as_number(Field::end)> bits_{16, 17};
        } s;
    };
#if UL_COMP_GNU_CPP && UL_COMP_GNU_CPP_VER >= 80200 && UL_COMP_GNU_CPP_VER < 90000
    ul::ignore_unused(lambda);
    // strange and disturbing, it just terminates when throwing
    //{
    //    bool ok{};
    //    try
    //    {
    //        lambda();
    //    }
    //    catch (...)
    //    {
    //        ok = true;
    //    }
    //    EXPECT_TRUE(ok);
    //}
#else
    EXPECT_THROW(lambda(), ul::fail_fast);
#endif
}

TEST(bits_isBigEndian, basics)
{
    std::cout << "is_bigendian: " << ul::bits::is_bigendian() << "\n";
}

// NOLINTEND
