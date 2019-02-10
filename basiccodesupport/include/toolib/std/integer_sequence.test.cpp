#include "integer_sequence.h"
#include "gtest/gtest.h"


namespace test_int
{
template<typename... T>
void toBeCalled(T... t)
{
    std::array<int, 3> ta = {t...};
    EXPECT_EQ(ta[0], 1);
    EXPECT_EQ(ta[1], 2);
    EXPECT_EQ(ta[2], 3);
}

template <int... Is>
void caller_impl(const std::array<int, 3>& theArray, too::idx::seq<Is...>)
{
    toBeCalled(theArray[Is]...);
}
} // test_int

TEST(idx_seq, array)
{
    std::array<int, 3> arr{1, 2, 3};

    test_int::caller_impl(arr, too::idx::gen_seq<arr.size()>{});
}


namespace test_size_t
{
template<typename... T>
void toBeCalled(T... t)
{
    std::array<size_t, 3> ta = {t...};
    EXPECT_EQ(ta[0], 1);
    EXPECT_EQ(ta[1], 2);
    EXPECT_EQ(ta[2], 3);
}

template <int... Is>
void caller_impl(const std::array<size_t, 3>& theArray, too::idx::seq<Is...>)
{
    toBeCalled(theArray[Is]...);
}
} // test_size_t

TEST(idx_seq, array_size_t)
{
    std::array<size_t, 3> arr{1, 2, 3};

    test_size_t::caller_impl(arr, too::idx::gen_seq<arr.size()>{});
}
