#ifndef EX_FOR_TEST_H_SGF4876KGE643IUYFYUTW3
#define EX_FOR_TEST_H_SGF4876KGE643IUYFYUTW3

#include "ul/assert.h"
#include <functional>
#include <string>

struct NoncommutativeAdditiveSemigroupEx {
    explicit NoncommutativeAdditiveSemigroupEx(std::string non_empty_str)
        : non_empty_str_{std::move(non_empty_str)} {
        UL_EXPECT(!non_empty_str_.empty());
    }

    NoncommutativeAdditiveSemigroupEx operator+=(const NoncommutativeAdditiveSemigroupEx& other) {
        non_empty_str_ += other.non_empty_str_;
        return *this;
    }

    friend bool operator==(const NoncommutativeAdditiveSemigroupEx& lhs, const NoncommutativeAdditiveSemigroupEx& rhs);
    friend NoncommutativeAdditiveSemigroupEx operator+(
        const NoncommutativeAdditiveSemigroupEx& a, const NoncommutativeAdditiveSemigroupEx& b);

private:
    std::string non_empty_str_{"a"};
};

inline bool operator==(const NoncommutativeAdditiveSemigroupEx& lhs, const NoncommutativeAdditiveSemigroupEx& rhs) {
    return lhs.non_empty_str_ == rhs.non_empty_str_;
}

inline NoncommutativeAdditiveSemigroupEx operator+(
    const NoncommutativeAdditiveSemigroupEx& a, const NoncommutativeAdditiveSemigroupEx& b) {
    NoncommutativeAdditiveSemigroupEx res{a};
    res += b;
    return res;
}

using NoncommutativeAdditiveMonoidEx = std::string;

using NoncommutativeAdditiveGroupEx = int;

using SemiRingEx = unsigned int;
using SemiRingOpCommMonoidEx = std::plus<SemiRingEx>;
using SemiRingOpMonoidEx = std::multiplies<SemiRingEx>;

using RingEx = int;

#endif
