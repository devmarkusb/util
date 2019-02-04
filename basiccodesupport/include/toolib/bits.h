// Markus Borris, 2019
// This file is part of tfl library.

//!
/**
 */
//! \file

#ifndef BITS_H_sljkhvnx2hng7835ghm3g7h3
#define BITS_H_sljkhvnx2hng7835ghm3g7h3

#include "toolib/assert.h"
#include <cstdint>
#include <limits>


namespace too
{
namespace bits
{
//! \Returns number of bits of the arbitrary \param Type.
template<typename Type>
inline constexpr uint16_t num() noexcept
{
    return 8 * sizeof(Type);
}

/** Produces, \returns, a bitmask with a connected range of 1's (and 0's out of that range) starting with the least
    significant bit (from the right) given by the 0-based \param beginLSB up to one before the least significant bit
    (from the right) given by the 0-based \param endLSB. To emphasize: we start counting bits with 0. And endLSB marks
    one place after the last bit of the range, that one isn't set.
    The maximum supported range goes up to the 63th bit.
    Examples:
     - range (x, x) wouldn't set any bit, since it is the empty range (effectively tries to fill from x to x-1, which is
         an empty interval in our sense where beginLSB <= endLSB needs to hold)
     - range (0, 1) would only set the 0th bit from the right, 0b1
     - range (0, 2) would result in 0b11, the 0th and 1st ones
     - range (1, 3) would result in 0b110, the 1st and 2nd ones
     - range (4, 5) would result in 0b10000, the 4th one
     - range (1, 5) would result in 0b11110*/
inline uint64_t makeRangeOfOnes(uint16_t beginLSB, uint16_t endLSB) noexcept
{
    TOO_EXPECT(beginLSB <= endLSB);
    TOO_EXPECT(beginLSB < num<uint64_t>());
    TOO_EXPECT(endLSB <= num<uint64_t>());

    return ((uint64_t(1) << (endLSB - beginLSB)) - 1) << beginLSB;
}
} // bits
} // too
#endif
