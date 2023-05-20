#include "ul/std/integer_sequence.h"
#include "gtest/gtest.h"
#include <array>

namespace ul = mb::ul;

namespace test_int {
template <typename... T>
void to_be_called(T... t) {
    std::array<int, 3> ta = {t...};
    EXPECT_EQ(ta[0], 1);
    EXPECT_EQ(ta[1], 2);
    EXPECT_EQ(ta[2], 3);
}

template <int... is>
void caller_impl(const std::array<int, 3>& theArray, ul::idx::Seq<is...> /*unused*/) {
    to_be_called(theArray[is]...);
}
} // namespace test_int

TEST(idx_seq, array) {
    const std::array<int, 3> arr{1, 2, 3};

    test_int::caller_impl(arr, ul::idx::GenSeq<arr.size()>{});
}

namespace test_size_t {
template <typename... T>
void to_be_called(T... t) {
    std::array<size_t, 3> ta = {t...};
    EXPECT_EQ(ta[0], 1);
    EXPECT_EQ(ta[1], 2);
    EXPECT_EQ(ta[2], 3);
}

template <int... is>
void caller_impl(const std::array<size_t, 3>& a, ul::idx::Seq<is...> /*unused*/) {
    to_be_called(a[is]...);
}
} // namespace test_size_t

TEST(idx_seq, array_size_t) {
    const std::array<size_t, 3> arr{1, 2, 3};

    test_size_t::caller_impl(arr, ul::idx::GenSeq<arr.size()>{});
}
