#ifndef OPERATION_H_3B9BF55249448A2F6A415A2D6
#define OPERATION_H_3B9BF55249448A2F6A415A2D6

#include "concepts.h"
#include "foundations.h"
#include "integer.h"
#include "ul/assert.h"
#include "ul/config.h"
#if __has_include(<concepts>)
#include <concepts>
#endif

#if __cpp_concepts && __cpp_lib_concepts
namespace mb::ul {
template <BinaryOperation Op, Integer N>
    requires Associative<Op>
Domain<Op> power_accumulate_pos(Domain<Op> r, Domain<Op> x, N n, Op op) {
    UL_EXPECT(n > 0);
    while (true) {
        if (n % 2) {
            r = op(r, x);
            if (n == 1)
                return r;
        }
        x = op(x, x);
        n /= 2;
    }
}

//! Should be fast when the op itself is slow. But measure.
template <BinaryOperation Op, Integer N>
    requires Associative<Op>
Domain<Op> power_opt2(Domain<Op> x, N n, Op op) {
    UL_EXPECT(n > 0);
    while (!(n % 2)) {
        x = op(x, x);
        n /= 2;
    }
    n /= 2;
    if (!n)
        return x;
    return power_accumulate_pos(x, op(x, x), n, op);
}
} // namespace mb::ul
#endif

UL_HEADER_END

#endif
