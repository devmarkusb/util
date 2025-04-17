/** \file
    Note, there are some rather trivial functions implemented here, hopefully for exposition/exemplary reasons only.
    The personal guess is, that there are won't be applications of faster e.g. 'twice' special case implementations,
    outperforming a generic *-operator overload. So we prefer using them when appropriate only, and otherwise just
    rely on C++ expressiveness and operator overloading. (Just saying, what about `*= 2`? Strictly we would have
    to provide it here as well, along with the uglieness of a reference parameter.)*/

#ifndef INTEGER_H_HGX23HX8730GZHRHGX1387G
#define INTEGER_H_HGX23HX8730GZHRHGX1387G

#include "assert.h"
#include "foundations.h"
#include "ul/config.h"
#if __has_include(<concepts>)
#include <concepts>
#endif

#if __cpp_concepts && __cpp_lib_concepts
namespace mb::ul {
template <typename T>
concept Integral = std::integral<T>;

template <Integral N>
N successor(N n) {
    return n + N{1};
}

template <Integral N>
N predecessor(N n) {
    return n - N{1};
}

template <Integral N>
N twice(N n) {
    // No n << 1? The saying lately goes: don't be too clever, show the compiler and the reader intent
    return n * N{2};
}

template <Integral N>
N half_nonnegative(N n) {
    UL_EXPECT(n >= N{});
    // No n >> 1? The saying lately goes: don't be too clever, show the compiler and the reader intent
    return n / N{2};
}

template <Integral N>
bool positive(N n) {
    return n > N{};
}

template <Integral N>
bool negative(N n) {
    return n < N{};
}

template <Integral N>
bool zero(N n) {
    return !n;
}

template <Integral N>
bool one(N n) {
    return n == N{1};
}

//! Fast mul/div by 2 and even/odd testing.
template <typename T>
concept BinaryInteger = Integral<T>;

template <BinaryInteger N>
N binary_scale_down_nonnegative(N n, std::unsigned_integral auto k) {
    UL_EXPECT(n >= N{});
    return n >> k;
}

template <BinaryInteger N>
N binary_scale_up_nonnegative(N n, std::unsigned_integral auto k) {
    UL_EXPECT(n >= N{});
    return n << k;
}

template <BinaryInteger N>
bool odd(N n) {
    // or static_cast<bool>(n & 0x1)
    return n % N{2};
}

template <BinaryInteger N>
bool even(N n) {
    return !odd(n);
}

//! Broader than Integral, which is restricted to built-in integer types.
/** Would be interesting to construct. Won't like to built it upon algebraic structure like ring, but perhaps more
    likely upon Peano axioms.*/
template <typename T>
concept Integer = Regular<T>;

template <typename T>
concept UnsignedInteger = Regular<T>;

template <typename T>
concept NaturalNumber = UnsignedInteger<T>;

template <Integer I>
bool divides(I i, I n) {
    UL_EXPECT(i != I{});
    return n % i == I{};
}

//! Other than the trivial 1.
template <Integer I>
I smallest_divisor(I n) {
    UL_EXPECT(n > I{});
    if (even(n))
        return I{2};
    for (I i{3}; i * i <= n; i += I{2}) {
        if (divides(i, n))
            return i;
    }
    return n;
}

//! Beware: slow, just to be used sparingly.
template <Integer I>
I is_prime(I n) {
    return n > I{1} && smallest_divisor(n) == n;
}

template <Integer I>
struct ModuloMultiply {
    I modulus{};

    explicit ModuloMultiply(I i)
        : modulus(i) {
    }

    I operator()(I n, I m) const {
        return (n * m) % modulus;
    }
};

template <Integer I>
I identity_element(ModuloMultiply<I>) {
    return I{1};
}
} // namespace mb::ul
#endif

UL_HEADER_END

#endif
