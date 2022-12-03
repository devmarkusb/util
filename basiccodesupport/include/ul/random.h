// 2019

//!
/**
 */
//! \file

#ifndef RANDOM_H_94xt4378j37843784n3
#define RANDOM_H_94xt4378j37843784n3

#include <random>


namespace mb::ul::math
{
//! Generates a random number within half-open interval [0.0, 1.0).
inline double random_frac()
{
    static std::random_device dev;
    static std::mt19937 mt{dev()};
    static std::uniform_real_distribution<double> dist{0.0, 1.0};
    return dist(mt);
}
} // namespace mb::ul::math

#endif
