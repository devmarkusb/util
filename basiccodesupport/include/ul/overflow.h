#ifndef OVERFLOW_H_423509491CC54DFE8E60F36007D88CE6
#define OVERFLOW_H_423509491CC54DFE8E60F36007D88CE6

#include "ul/concepts.h"
#include "ul/config.h"
#include <limits>

namespace mb::ul::math {
template <typename N>
UL_REQUIRES(Arithmetic<N>)
bool mul_overflow(N a, N b) {
    if (a == -1 && b == std::numeric_limits<N>::min())
        return true;
    // a * x or a / x can overflow
    if (b == -1 && a == std::numeric_limits<N>::min())
        return true;
    if (b != 0 && a > std::numeric_limits<N>::max() / b)
        return true;
    if (b != 0 && a < std::numeric_limits<N>::min() / b)
        return true;
    return false;
}
} // namespace mb::ul::math

UL_HEADER_END

#endif
