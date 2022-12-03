#include "ul/hash.h"
#include "gtest/gtest.h"
#include <string>
#include <unordered_set>

namespace ul = mb::ul;

namespace
{
struct UserType
{
    int a{};
    std::string b;
};
} // namespace

namespace std
{
template <>
struct hash<UserType>
{
    size_t operator()(const UserType& ut) const
    {
        return ul::hashCombine(std::hash<int>()(ut.a), std::hash<std::string>()(ut.b));
    }
};
} // namespace std

TEST(hashCombine, basic)
{
    UserType ut1{1, "qwertzuiopasdfghjklyxcvbnm1234567890"};
    UserType ut2{1, "qwertzuiopasdfghjklyxcvbnm1234567891"};
    // just to check that this compiles (wouldn't without specializing std::hash for UserType)
    std::unordered_set<UserType> hashed_container;
    // a questionable test checking if the hash seems ok (little change should lead to a completely different hash)
    EXPECT_NE(std::hash<UserType>()(ut1), std::hash<UserType>()(ut2));
}
