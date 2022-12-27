// 2019

//! \file

#ifndef ARRAY_H_gh4578xdtn4813478tx34
#define ARRAY_H_gh4578xdtn4813478tx34

#include <array>

namespace mb::ul::array
{
/** Declaring and initialising an array without explicit (and redundant) statement of size, e.g.
    auto myarray = util::make_array(42, 43, 44); // which is a std::array<int, 3> then*/
template <typename FirstT, typename... RemainingTs>
constexpr auto make(FirstT first, RemainingTs... remaining) noexcept -> std::array<FirstT, 1 + sizeof...(RemainingTs)>
{
    std::array<FirstT, 1 + sizeof...(RemainingTs)> ret = {first, remaining...};
    return ret;
}
} // namespace mb::ul::array

#endif
