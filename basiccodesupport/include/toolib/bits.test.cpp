#include "bits.h"
#include "toolib/assert.h"
#include "toolib/enum_cast.h"
#include "toolib/ignore_arg.h"
#include "gtest/gtest.h"
#include <type_traits>


TEST(bits_count, various)
{
    static_assert(too::bits::count<uint8_t>() == 8);
    static_assert(too::bits::count<uint32_t>() == 32);
    static_assert(too::bits::count<uint64_t>() == 64);

    static_assert(too::bits::count<int8_t>() == 8);
    static_assert(too::bits::count<int32_t>() == 32);
    static_assert(too::bits::count<int64_t>() == 64);
}

TEST(bits_countSetBits, various)
{
    EXPECT_EQ(too::bits::countSet(0), 0);
    EXPECT_EQ(too::bits::countSet(1), 1);
    EXPECT_EQ(too::bits::countSet(0b11), 2);
    EXPECT_EQ(too::bits::countSet(0b101), 2);
    EXPECT_EQ(too::bits::countSet(0b111), 3);
    EXPECT_EQ(too::bits::countSet(0b1111110), 6);
    EXPECT_EQ(too::bits::countSet(0b1111110111), 9);
    EXPECT_EQ(too::bits::countSet(0b10000000000), 1);
}

TEST(bits_set, various)
{
    EXPECT_EQ(too::bits::set(0b000, 0), 0b001);
    EXPECT_EQ(too::bits::set(0b000, 1), 0b010);
    EXPECT_EQ(too::bits::set<uint8_t>(0b1, 7), 0b10000001);
    EXPECT_DEBUG_DEATH(too::bits::set<uint8_t>(0b1, 8), too::death_assert_regex);
}

TEST(bits_unset, various)
{
    EXPECT_EQ(too::bits::unset(0b111, 0), 0b110);
    EXPECT_EQ(too::bits::unset(0b111, 1), 0b101);
    EXPECT_EQ(too::bits::unset<uint8_t>(1 << 7, 7), 0);
    EXPECT_DEBUG_DEATH(too::bits::unset<uint8_t>(0b1, 8), too::death_assert_regex);
}

TEST(bits_toggle, various)
{
    EXPECT_EQ(too::bits::toggle(0b111, 0), 0b110);
    EXPECT_EQ(too::bits::toggle(0b110, 0), 0b111);
    EXPECT_EQ(too::bits::toggle(0b101, 1), 0b111);
    EXPECT_EQ(too::bits::toggle<uint8_t>(1 << 7, 7), 0);
    EXPECT_DEBUG_DEATH(too::bits::unset<uint8_t>(0b1, 8), too::death_assert_regex);
}

TEST(bits_check, various)
{
    EXPECT_EQ(too::bits::check(0b111, 0), 1);
    EXPECT_EQ(too::bits::check(0b110, 0), 0);
    EXPECT_EQ(too::bits::check(0b101, 1), 0);
    EXPECT_EQ(too::bits::check<uint8_t>(1 << 7, 7), 1);
    EXPECT_DEBUG_DEATH(too::bits::check<uint8_t>(0b1, 8), too::death_assert_regex);
}

TEST(bits_change, various)
{
    EXPECT_EQ(too::bits::change(0b111, 0, 1), 0b111);
    EXPECT_EQ(too::bits::change(0b110, 0, 1), 0b111);
    EXPECT_EQ(too::bits::change(0b111, 0, 0), 0b110);
    EXPECT_EQ(too::bits::change(0b110, 0, 0), 0b110);
    EXPECT_EQ(too::bits::change(0b101, 1, 1), 0b111);
    EXPECT_EQ(too::bits::change<uint8_t>(1 << 7, 7, 0), 0);
    EXPECT_DEBUG_DEATH(too::bits::change<uint8_t>(0b1, 8, 1), too::death_assert_regex);
}

TEST(bits_setMask, various)
{
    EXPECT_EQ(too::bits::setMask(0b000, 0b001), 0b001);
    EXPECT_EQ(too::bits::setMask(0b000, 0b010), 0b010);
    EXPECT_EQ(too::bits::setMask(0b000, 0b101), 0b101);
    EXPECT_EQ(too::bits::setMask<uint8_t>(0b1, 1 << 7), (1 << 7) | 0b1);
}

TEST(bits_unsetMask, various)
{
    EXPECT_EQ(too::bits::unsetMask(0b111, 0b001), 0b110);
    EXPECT_EQ(too::bits::unsetMask(0b111, 0b010), 0b101);
    EXPECT_EQ(too::bits::unsetMask(0b111, 0b101), 0b010);
    EXPECT_EQ(too::bits::unsetMask<uint8_t>((1 << 7) | 0b1, 1 << 7), 0b1);
}

TEST(bits_toggleMask, various)
{
    EXPECT_EQ(too::bits::toggleMask(0b111, 0b001), 0b110);
    EXPECT_EQ(too::bits::toggleMask(0b110, 0b001), 0b111);
    EXPECT_EQ(too::bits::toggleMask(0b111, 0b101), 0b010);
    EXPECT_EQ(too::bits::toggleMask<uint8_t>((1 << 7) | 0b1, 1 << 7), 0b1);
}

TEST(bits_checkAllOfMask, various)
{
    EXPECT_TRUE(too::bits::checkAllOfMask(0b111, 0b101));
    EXPECT_TRUE(too::bits::checkAllOfMask(0b101, 0b101));
    EXPECT_FALSE(too::bits::checkAllOfMask(0b110, 0b101));
    EXPECT_FALSE(too::bits::checkAllOfMask(0b010, 0b101));
    EXPECT_TRUE(too::bits::checkAllOfMask(0b010, 0b010));
    EXPECT_FALSE(too::bits::checkAllOfMask(0b101, 0b010));
}

TEST(bits_checkAnyOfMask, various)
{
    EXPECT_TRUE(too::bits::checkAnyOfMask(0b111, 0b101));
    EXPECT_TRUE(too::bits::checkAnyOfMask(0b101, 0b101));
    EXPECT_TRUE(too::bits::checkAnyOfMask(0b100, 0b101));
    EXPECT_TRUE(too::bits::checkAnyOfMask(0b001, 0b101));
    EXPECT_TRUE(too::bits::checkAnyOfMask(0b110, 0b101));
    EXPECT_FALSE(too::bits::checkAnyOfMask(0b010, 0b101));
    EXPECT_TRUE(too::bits::checkAnyOfMask(0b010, 0b010));
    EXPECT_FALSE(too::bits::checkAnyOfMask(0b101, 0b010));
}

TEST(bits_setRange, empty)
{
    EXPECT_DEBUG_DEATH(too::bits::setRange(5, 0), too::death_assert_regex);
}

TEST(bits_setRange, bitNr0)
{
    EXPECT_EQ(too::bits::setRange(0, 1), 0b1u);
}

TEST(bits_setRange, bitNr1)
{
    EXPECT_EQ(too::bits::setRange(1, 1), 0b10u);
}

TEST(bits_setRange, bitNr63)
{
    EXPECT_EQ(too::bits::setRange<uint64_t>(63, 1), uint64_t(1) << uint64_t(63));
}

TEST(bits_setRange, highestTwoBits)
{
    EXPECT_EQ(too::bits::setRange<uint64_t>(62, 2), (uint64_t(1) << uint64_t(63)) | (uint64_t(1) << uint64_t(62)));
}

TEST(bits_setRange, lowestTwoBits)
{
    EXPECT_EQ(too::bits::setRange(0, 2), 0b11u);
}

TEST(bits_setRange, someMiddleRangeOfBits)
{
    EXPECT_EQ(too::bits::setRange(3, 6), 0b111111000u);
}

TEST(bits_setRange, someMiddleRangeOfBits_castedToSufficientSmallerType)
{
    EXPECT_EQ(too::bits::setRange<uint8_t>(2, 6),   0b11111100u);
    EXPECT_DEBUG_DEATH(too::bits::setRange<uint8_t>(2, 7), too::death_assert_regex);
    EXPECT_EQ(too::bits::setRange<uint16_t>(2, 7), 0b111111100u);
}

TEST(bits_read, typical)
{
    EXPECT_EQ(too::bits::read(0b11000101100u, 2, 5), 0b01011u);
    const auto x = too::bits::readAndCast<uint8_t>(uint16_t{0b11000101100u}, 2, 5);
    EXPECT_EQ(x, uint8_t{0b01011});
    EXPECT_DEBUG_DEATH(too::bits::setRange<uint8_t>(2, 7), too::death_assert_regex);
}

namespace
{
// workaround, because gtest understands comma between template parameters as comma for the gtest macro;
// so we need a template with one instead of two parameters
template<typename TargetType>
constexpr TargetType read_from16_testhelper(uint16_t data, too::bits::Idx idx, too::bits::Count count) noexcept
{
    return too::bits::readAndCast<TargetType>(data, idx, count);
}
} // namespace

TEST(bits_read, range_overflows)
{
    EXPECT_DEBUG_DEATH(too::bits::read<uint8_t>(0b10101100u, 2, 7), too::death_assert_regex);
    EXPECT_DEBUG_DEATH(too::bits::read<uint8_t>(0b10101100u, 9, 0), too::death_assert_regex);
    EXPECT_DEBUG_DEATH(too::bits::read<uint8_t>(0b10101100u, 0, 9), too::death_assert_regex);
    EXPECT_DEBUG_DEATH(read_from16_testhelper<uint8_t>(0b11000101100u, 2, 9), too::death_assert_regex);
}

TEST(bits_read, none)
{
    EXPECT_DEBUG_DEATH(too::bits::read<uint8_t>(0b10101100u, 2, 0), too::death_assert_regex);
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

    // the actually interesting one
    uint8_t u8{0b10101100u};
    auto x3 = too::bits::read(u8, 2, 1);
    static_assert(std::is_same_v<decltype(x3), uint8_t>);
}

TEST(bits_read, types_cast)
{
    auto x = too::bits::readAndCast<uint8_t, uint16_t>(0b10101100u, 2, 1);
    static_assert(std::is_same_v<decltype(x), uint8_t>);
    auto x2 = too::bits::readAndCast<uint16_t>(uint32_t{0b10101100u}, 2, 1);
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
    EXPECT_DEBUG_DEATH(too::bits::write(0b10101100u, 2, 0, 1), too::death_assert_regex);
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
    EXPECT_DEBUG_DEATH(too::bits::write<uint16_t>(0, 0, 17, 0), too::death_assert_regex);
    auto _ = too::bits::write<uint16_t>(0, 0, 16, 0);
    EXPECT_DEBUG_DEATH(write_to8_testhelper<uint8_t >(0, 0, 9, 0), too::death_assert_regex);
    _ = write_to8_testhelper<uint8_t >(0, 0, 8, 0);
    too::ignore_arg(_);
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

TEST(bits_BitArray, construct)
{
    too::bits::Array<128, uint64_t> a1;
    for (too::bits::Idx i = 0; i < 128; ++i)
        EXPECT_FALSE(a1.isSet(i));

    too::bits::Array<2> a2;
    for (too::bits::Idx i = 0; i < 2; ++i)
        EXPECT_FALSE(a2.isSet(i));

    too::bits::Array<9, uint8_t> a3;
    for (too::bits::Idx i = 0; i < 9; ++i)
        EXPECT_FALSE(a3.isSet(i));
}

TEST(bits_BitArray, set)
{
    too::bits::Array<42> a1;
    a1.set(41);
    EXPECT_TRUE(a1.isSet(41));
    EXPECT_DEBUG_DEATH(a1.set(42), too::death_assert_regex);

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
    too::bits::Array<42> a1;
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
    for (too::bits::Idx i = 0; i < 42; ++i)
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
#if TOO_COMP_GNU_CPP && TOO_COMP_GNU_CPP_VER >= 80200 && TOO_COMP_GNU_CPP_VER < 90000
    too::ignore_arg(lambda);
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
    EXPECT_THROW(lambda(), too::fail_fast);
#endif
}

TEST(bits_isBigEndian, basics)
{
    std::cout << "is_bigendian: " << too::bits::is_bigendian() << "\n";
}
