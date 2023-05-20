//! \file

#ifndef HASH_H_ERUGXH7842G873HGX782GH3
#define HASH_H_ERUGXH7842G873HGX782GH3

#include <cstddef>

namespace mb::ul {
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
inline size_t hash_combine(std::size_t hash) noexcept {
    return hash;
}

template <typename... SizeTs>
inline size_t hash_combine(std::size_t hash1, SizeTs... hashes) noexcept {
    // implemented like boost's hash_combine
    return hash1 ^ (hashCombine(hashes...) + 0x9e3779b9 + (hash1 << 6u) + (hash1 >> 2u)); // NOLINT
}

//!@}
} // namespace mb::ul

#endif
