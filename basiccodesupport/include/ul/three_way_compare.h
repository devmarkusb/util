//! \file

#ifndef THREE_WAY_COMPARE_H_eruotzxgh23g3
#define THREE_WAY_COMPARE_H_eruotzxgh23g3

#include <type_traits>

namespace mb::ul
{
/**
 * As long as there is no spaceship operator <=>, this is the equivalent.
 * The idea is to implement a safe three way comparison returning -1, 0, 1
 * for params lhs < rhs, lhs == rhs, lhs > rhs respectively.
 * "Safe" means that an otherwise naive implementation of returning lhs - rhs suffers from dangers of signed overflow
 * and e.g. the impossibility to always find a large enough signed type to hold differences of the largest available
 * unsigned type.
 * Cf. https://stackoverflow.com/questions/48042955/how-is-the-three-way-comparison-operator-different-from-subtraction
 * Impl. note: didn't consider implementing this for floating types as well, might be easy, might be not.
 */
template <typename LessComparableType>
int threeWayCompare(LessComparableType lhs, LessComparableType rhs)
{
    static_assert(!std::is_floating_point_v<LessComparableType>);
    return (rhs < lhs) - (lhs < rhs);
}
} // namespace mb::ul

#endif
