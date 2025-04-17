/** \file
*/

#ifndef FERMAT_MILLER_RABIN_H_IUDGAYUIFGYUIASG567574FYG
#define FERMAT_MILLER_RABIN_H_IUDGAYUIFGYUIASG567574FYG

#include "algebra.h"
#include "integer.h"
#include "ul/config.h"

#if __cpp_concepts && __cpp_lib_concepts
namespace mb::ul {
template <Integer I>
I multiplicative_inverse_fermat(I a, I p) {
    UL_EXPECT(is_prime(p) && a > I{});
    ModuloMultiply<I> op{p};
    return power_monoid(a, p - I{2}, op, identity_element(op));
}

//! Reads: n no prime if false, probably prime if true, more probable for more witnesses.
template <Integer I>
bool fermat_test(I n, I witness) {
    UL_EXPECT(I{} < witness);
    UL_EXPECT(witness < n);
    I remainder{power_semigroup(witness, n - I{1}, ModuloMultiply<I>(n))};
    return remainder == I{1};
}

template <Integer I>
bool miller_rabin_test(I n, I q, I k, I w) {
    UL_EXPECT(n > I{1});
    UL_EXPECT(n - I{1} == I{2} * k * q);
    UL_EXPECT(odd(q));
    ModuloMultiply<I> mmult{n};
    I x = power_semigroup(w, q, mmult);
    if (x == I{1} || x == n - I{1})
        return true;
    for (I i{1}; i < k; ++i) {
        // invariant x = w^(2^(i−1)q)
        x = mmult(x, x);
        if (x == n - I{1})
            return true;
        if (x == I{1})
            return false;
    }
    return false;
}
} // namespace mb::ul
#endif

UL_HEADER_END

#endif
