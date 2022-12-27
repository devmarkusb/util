// 2019

//! \file

#ifndef HASH_H_erugxh7842g873hgx782gh3
#define HASH_H_erugxh7842g873hgx782gh3

#include <cstddef>

namespace mb::ul
{
/** Generates a combined hash from a list of hashes.
    Typical usage is to specialize std's hash for a user type composed of std-types, like so:
    struct UserType
    {
        int a{};
        std::string b;
    };
    namespace std
    {
    template<>
    struct hash<UserType>
    {
        size_t operator()(const UserType& ut) const
        {
            return ul::hashCombine(
                std::hash<int>()(ut.a),
                std::hash<std::string>()(ut.b));
        }
    };
    } // std*/
//!@{
inline size_t hashCombine(std::size_t hash) noexcept
{
    return hash;
}

template <typename... Size_ts>
inline size_t hashCombine(std::size_t hash1, Size_ts... hashes) noexcept
{
    // implemented like boost's hash_combine
    return hash1 ^ (hashCombine(hashes...) + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
}

//!@}
} // namespace mb::ul

#endif
