#include "mb/ul/basiccodesupport/c_string_view.hpp"

#include "gtest/gtest.h"
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_set>

namespace mb::ul {
// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

TEST(CStringViewConstruct, DefaultEmpty) {
    CStringView sv;
    EXPECT_EQ(0U, sv.size());
    EXPECT_TRUE(sv.empty());
    EXPECT_STREQ("", sv.c_str());
    EXPECT_EQ('\0', sv.c_str()[0]);
}

TEST(CStringViewConstruct, FromCStr) {
    CStringView sv{"hello"};
    EXPECT_EQ(5U, sv.size());
    EXPECT_FALSE(sv.empty());
    EXPECT_STREQ("hello", sv.c_str());
}

TEST(CStringViewConstruct, FromCStrEmpty) {
    CStringView sv{""};
    EXPECT_EQ(0U, sv.size());
    EXPECT_TRUE(sv.empty());
    EXPECT_EQ('\0', sv.c_str()[0]);
}

TEST(CStringViewConstruct, FromCStrNullThrows) {
    EXPECT_THROW(CStringView{static_cast<const char*>(nullptr)}, std::invalid_argument);
}

TEST(CStringViewConstruct, FromPtrLen) {
    const char buf[] = "hello";
    CStringView sv{buf, 5};
    EXPECT_EQ(5U, sv.size());
    EXPECT_STREQ("hello", sv.c_str());
}

TEST(CStringViewConstruct, FromPtrLenZero) {
    // len==0 still requires ptr[0]=='\0' to satisfy the null-termination invariant
    const char* empty = "";
    CStringView sv{empty, 0};
    EXPECT_EQ(0U, sv.size());
    EXPECT_TRUE(sv.empty());
    EXPECT_EQ('\0', sv.c_str()[0]);
}

TEST(CStringViewConstruct, FromPtrLenZeroNonEmptyPtrThrows) {
    // ptr points at 'h', so ptr[0] != '\0' — invalid even with len==0
    const char buf[] = "hi";
    EXPECT_THROW(CStringView(buf, 0), std::invalid_argument);
}

TEST(CStringViewConstruct, FromNullPtrLenZeroSucceeds) {
    // null + len==0 is accepted as empty by design
    CStringView sv{nullptr, 0};
    EXPECT_TRUE(sv.empty());
    EXPECT_EQ('\0', sv.c_str()[0]);
}

TEST(CStringViewConstruct, FromNullPtrNonzeroLenThrows) {
    EXPECT_THROW(CStringView(nullptr, 3), std::invalid_argument);
}

TEST(CStringViewConstruct, FromPtrLenNotNullTerminatedThrows) {
    // buf[3]=='d', not '\0', so claiming len==3 must throw
    const char buf[4] = {'a', 'b', 'c', 'd'};
    EXPECT_THROW(CStringView(buf, 3), std::invalid_argument);
}

TEST(CStringViewConstruct, FromPtrLenWrongLengthThrows) {
    // "hello\0" but we claim length 3 — buf[3] is 'l', not '\0'
    const char* s = "hello";
    EXPECT_THROW(CStringView(s, 3), std::invalid_argument);
}

TEST(CStringViewConstruct, FromArray) {
    CStringView sv{"world"};
    EXPECT_EQ(5U, sv.size());
    EXPECT_STREQ("world", sv.c_str());
}

TEST(CStringViewConstruct, FromArrayEmpty) {
    CStringView sv{""};
    EXPECT_EQ(0U, sv.size());
    EXPECT_EQ('\0', sv.c_str()[0]);
}

TEST(CStringViewConstruct, FromStdString) {
    std::string s{"hello"};
    CStringView sv{s};
    EXPECT_EQ(5U, sv.size());
    EXPECT_STREQ("hello", sv.c_str());
}

TEST(CStringViewConstruct, FromStdStringEmpty) {
    std::string s;
    CStringView sv{s};
    EXPECT_EQ(0U, sv.size());
    EXPECT_EQ('\0', sv.c_str()[0]);
}

TEST(CStringViewConstruct, FromStringViewNullTerminated) {
    // Build a string_view that reaches into a null-terminated string including its terminator space
    std::string s{"hello"};
    // s.c_str()[5] == '\0' so this string_view is backed by null-terminated storage
    std::string_view sv_raw{s.c_str(), 5};
    CStringView sv{sv_raw};
    EXPECT_EQ(5U, sv.size());
    EXPECT_STREQ("hello", sv.c_str());
}

TEST(CStringViewConstruct, FromStringViewEmptyNullTerminated) {
    std::string s{""};
    std::string_view sv_raw{s.c_str(), 0};
    CStringView sv{sv_raw};
    EXPECT_EQ(0U, sv.size());
}

TEST(CStringViewConstruct, FromStringViewNotNullTerminatedThrows) {
    // Middle of a string — data()[size()] is not '\0'
    const char buf[] = "abcde";
    std::string_view sv_raw{buf, 3}; // 'a','b','c' — buf[3]=='d', not '\0'
    EXPECT_THROW(CStringView{sv_raw}, std::invalid_argument);
}

TEST(CStringViewConstruct, FromStringViewNullDataZeroSizeSucceeds) {
    std::string_view sv_raw{nullptr, 0};
    CStringView sv{sv_raw};
    EXPECT_TRUE(sv.empty());
}

// ---------------------------------------------------------------------------
// Observers
// ---------------------------------------------------------------------------

TEST(CStringViewObservers, DataAndCStr) {
    CStringView sv{"abc"};
    EXPECT_EQ(sv.data(), sv.c_str());
    EXPECT_EQ('a', sv.data()[0]);
}

TEST(CStringViewObservers, SizeAndLength) {
    CStringView sv{"test"};
    EXPECT_EQ(sv.size(), sv.length());
    EXPECT_EQ(4U, sv.size());
}

TEST(CStringViewObservers, NullTerminationGuarantee) {
    CStringView sv{"hello"};
    EXPECT_EQ('\0', sv.c_str()[sv.size()]);
}

TEST(CStringViewObservers, NullTerminationGuaranteeEmpty) {
    CStringView sv;
    EXPECT_EQ('\0', sv.c_str()[sv.size()]);
}

// ---------------------------------------------------------------------------
// Element access
// ---------------------------------------------------------------------------

TEST(CStringViewAccess, SubscriptOperator) {
    CStringView sv{"abc"};
    EXPECT_EQ('a', sv[0]);
    EXPECT_EQ('b', sv[1]);
    EXPECT_EQ('c', sv[2]);
}

TEST(CStringViewAccess, AtValid) {
    CStringView sv{"abc"};
    EXPECT_EQ('a', sv.at(0));
    EXPECT_EQ('c', sv.at(2));
}

TEST(CStringViewAccess, AtOutOfRangeThrowsOutOfRange) {
    CStringView sv{"abc"};
    EXPECT_THROW(sv.at(3), std::out_of_range);
    EXPECT_THROW(sv.at(100), std::out_of_range);
}

TEST(CStringViewAccess, AtEmptyThrowsOutOfRange) {
    CStringView sv;
    EXPECT_THROW(sv.at(0), std::out_of_range);
}

TEST(CStringViewAccess, AtDoesNotThrowInvalidArgument) {
    CStringView sv{"x"};
    EXPECT_THROW(sv.at(1), std::out_of_range);
    // Confirm it is NOT invalid_argument (which was the bug)
    try {
        sv.at(1);
    } catch (const std::out_of_range&) {
        // correct
    } catch (const std::invalid_argument&) {
        FAIL() << "at() threw invalid_argument instead of out_of_range";
    }
}

TEST(CStringViewAccess, Front) {
    CStringView sv{"hello"};
    EXPECT_EQ('h', sv.front());
}

TEST(CStringViewAccess, Back) {
    CStringView sv{"hello"};
    EXPECT_EQ('o', sv.back());
}

TEST(CStringViewAccess, FrontBackSingleChar) {
    CStringView sv{"x"};
    EXPECT_EQ('x', sv.front());
    EXPECT_EQ('x', sv.back());
}

// ---------------------------------------------------------------------------
// Iterators
// ---------------------------------------------------------------------------

TEST(CStringViewIterators, BeginEnd) {
    CStringView sv{"abc"};
    EXPECT_EQ(sv.data(), sv.begin());
    EXPECT_EQ(sv.data() + 3, sv.end());
}

TEST(CStringViewIterators, RangeFor) {
    CStringView sv{"abc"};
    std::string collected;
    for (char c : sv)
        collected += c;
    EXPECT_EQ("abc", collected);
}

TEST(CStringViewIterators, RangeForEmpty) {
    CStringView sv;
    std::string collected;
    for (char c : sv)
        collected += c;
    EXPECT_TRUE(collected.empty());
}

TEST(CStringViewIterators, CbeginCend) {
    CStringView sv{"hi"};
    EXPECT_EQ(sv.begin(), sv.cbegin());
    EXPECT_EQ(sv.end(), sv.cend());
}

TEST(CStringViewIterators, Distance) {
    CStringView sv{"hello"};
    EXPECT_EQ(static_cast<std::ptrdiff_t>(sv.size()), sv.end() - sv.begin());
}

// ---------------------------------------------------------------------------
// Conversions
// ---------------------------------------------------------------------------

TEST(CStringViewConversions, Sv) {
    CStringView csv{"hello"};
    std::string_view sv = csv.sv();
    EXPECT_EQ("hello", sv);
    EXPECT_EQ(5U, sv.size());
}

TEST(CStringViewConversions, ImplicitToStringView) {
    CStringView csv{"world"};
    std::string_view sv = csv;
    EXPECT_EQ("world", sv);
}

TEST(CStringViewConversions, ToString) {
    CStringView csv{"test"};
    EXPECT_EQ(std::string{"test"}, csv.to_string());
}

TEST(CStringViewConversions, ToStringEmpty) {
    CStringView csv;
    EXPECT_EQ(std::string{}, csv.to_string());
}

TEST(CStringViewConversions, PassToFunctionTakingStringView) {
    auto f = [](std::string_view sv) {
        return sv.size();
    };
    CStringView csv{"abc"};
    EXPECT_EQ(3U, f(csv));
}

// ---------------------------------------------------------------------------
// substr
// ---------------------------------------------------------------------------

TEST(CStringViewSubstr, PosOnly) {
    CStringView sv{"hello"};
    auto suffix = sv.substr(2);
    EXPECT_EQ("llo", suffix.sv());
    EXPECT_EQ(3U, suffix.size());
    EXPECT_EQ('\0', suffix.c_str()[suffix.size()]);
}

TEST(CStringViewSubstr, PosZero) {
    CStringView sv{"hello"};
    auto full = sv.substr(0);
    EXPECT_EQ("hello", full.sv());
}

TEST(CStringViewSubstr, PosAtEnd) {
    CStringView sv{"hello"};
    auto empty = sv.substr(5);
    EXPECT_TRUE(empty.empty());
    EXPECT_EQ('\0', empty.c_str()[0]);
}

TEST(CStringViewSubstr, PosPastEndThrows) {
    CStringView sv{"hello"};
    EXPECT_THROW(sv.substr(6), std::invalid_argument);
}

TEST(CStringViewSubstr, PosAndCountSuffix) {
    CStringView sv{"hello"};
    auto suffix = sv.substr(2, 3);
    EXPECT_EQ("llo", suffix.sv());
    EXPECT_EQ('\0', suffix.c_str()[suffix.size()]);
}

TEST(CStringViewSubstr, PosAndCountNonSuffixThrows) {
    CStringView sv{"hello"};
    // pos=1, count=2 → "el", but that ends at index 3, not at len=5
    EXPECT_THROW(sv.substr(1, 2), std::invalid_argument);
}

TEST(CStringViewSubstr, PosAndCountNpos) {
    CStringView sv{"hello"};
    auto suffix = sv.substr(2, CStringView::npos);
    EXPECT_EQ("llo", suffix.sv());
}

TEST(CStringViewSubstr, PosAndCountZeroAtEnd) {
    CStringView sv{"hello"};
    auto empty = sv.substr(5, 0);
    EXPECT_TRUE(empty.empty());
}

TEST(CStringViewSubstr, PosAndCountOutOfRangeThrows) {
    CStringView sv{"hello"};
    EXPECT_THROW(sv.substr(3, 5), std::invalid_argument);
}

TEST(CStringViewSubstr, ResultNullTerminated) {
    CStringView sv{"hello"};
    auto s = sv.substr(1);
    EXPECT_EQ('\0', s.c_str()[s.size()]);
}

// ---------------------------------------------------------------------------
// remove_prefix
// ---------------------------------------------------------------------------

TEST(CStringViewRemovePrefix, Valid) {
    CStringView sv{"hello"};
    sv.remove_prefix(2);
    EXPECT_EQ("llo", sv.sv());
    EXPECT_EQ(3U, sv.size());
}

TEST(CStringViewRemovePrefix, Zero) {
    CStringView sv{"hello"};
    sv.remove_prefix(0);
    EXPECT_EQ("hello", sv.sv());
}

TEST(CStringViewRemovePrefix, AllChars) {
    CStringView sv{"hello"};
    sv.remove_prefix(5);
    EXPECT_TRUE(sv.empty());
    EXPECT_EQ('\0', sv.c_str()[0]);
}

TEST(CStringViewRemovePrefix, OutOfRangeThrows) {
    CStringView sv{"hello"};
    EXPECT_THROW(sv.remove_prefix(6), std::invalid_argument);
}

TEST(CStringViewRemovePrefix, ResultNullTerminated) {
    CStringView sv{"hello"};
    sv.remove_prefix(3);
    EXPECT_EQ('\0', sv.c_str()[sv.size()]);
}

// ---------------------------------------------------------------------------
// find
// ---------------------------------------------------------------------------

TEST(CStringViewFind, FindCharFound) {
    CStringView sv{"hello"};
    EXPECT_EQ(1U, sv.find('e'));
}

TEST(CStringViewFind, FindCharNotFound) {
    CStringView sv{"hello"};
    EXPECT_EQ(CStringView::npos, sv.find('z'));
}

TEST(CStringViewFind, FindCharWithPos) {
    CStringView sv{"hello"};
    EXPECT_EQ(3U, sv.find('l', 3));
}

TEST(CStringViewFind, FindCharFirst) {
    CStringView sv{"aabaa"};
    EXPECT_EQ(0U, sv.find('a'));
    EXPECT_EQ(1U, sv.find('a', 1)); // 'a' at index 1, searching from pos 1
    EXPECT_EQ(3U, sv.find('a', 2)); // index 2 is 'b', first 'a' at or after 2 is index 3
}

TEST(CStringViewFind, FindStringView) {
    CStringView sv{"hello world"};
    EXPECT_EQ(6U, sv.find(std::string_view{"world"}));
}

TEST(CStringViewFind, FindStringViewNotFound) {
    CStringView sv{"hello"};
    EXPECT_EQ(CStringView::npos, sv.find(std::string_view{"xyz"}));
}

TEST(CStringViewFind, FindStringViewWithPos) {
    CStringView sv{"abcabc"};
    EXPECT_EQ(3U, sv.find(std::string_view{"abc"}, 1));
}

// ---------------------------------------------------------------------------
// Comparisons — CStringView vs CStringView
// ---------------------------------------------------------------------------

TEST(CStringViewCompare, EqualityEqual) {
    CStringView a{"hello"};
    CStringView b{"hello"};
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST(CStringViewCompare, EqualityNotEqual) {
    CStringView a{"hello"};
    CStringView b{"world"};
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST(CStringViewCompare, EqualityDifferentLength) {
    CStringView a{"hi"};
    CStringView b{"hix"};
    EXPECT_FALSE(a == b);
}

TEST(CStringViewCompare, SpaceshipLess) {
    CStringView a{"abc"};
    CStringView b{"abd"}; // codespell:ignore
    EXPECT_TRUE((a <=> b) == std::strong_ordering::less);
    EXPECT_TRUE(a < b);
}

TEST(CStringViewCompare, SpaceshipGreater) {
    CStringView a{"xyz"};
    CStringView b{"abc"};
    EXPECT_TRUE((a <=> b) == std::strong_ordering::greater);
    EXPECT_TRUE(a > b);
}

TEST(CStringViewCompare, SpaceshipEqual) {
    CStringView a{"same"};
    CStringView b{"same"};
    EXPECT_TRUE((a <=> b) == std::strong_ordering::equal);
}

TEST(CStringViewCompare, SpaceshipLengthDifference) {
    CStringView a{"abc"};
    CStringView b{"abcd"};
    EXPECT_TRUE(a < b);
    EXPECT_TRUE(b > a);
}

TEST(CStringViewCompare, EmptyEqualsEmpty) {
    CStringView a;
    CStringView b;
    EXPECT_TRUE(a == b);
    EXPECT_TRUE((a <=> b) == std::strong_ordering::equal);
}

TEST(CStringViewCompare, EmptyLessThanNonEmpty) {
    CStringView a;
    CStringView b{"a"};
    EXPECT_TRUE(a < b);
}

// ---------------------------------------------------------------------------
// Comparisons — CStringView vs std::string_view
// ---------------------------------------------------------------------------

TEST(CStringViewCompareStringView, EqualityEqual) {
    CStringView csv{"hello"};
    std::string_view sv{"hello"};
    // cppcheck-suppress knownConditionTrueFalse
    EXPECT_TRUE(csv == sv);
    // cppcheck-suppress knownConditionTrueFalse
    EXPECT_TRUE(sv == csv);
}

TEST(CStringViewCompareStringView, EqualityNotEqual) {
    CStringView csv{"hello"};
    std::string_view sv{"world"};
    EXPECT_FALSE(csv == sv);
    EXPECT_FALSE(sv == csv);
}

// ---------------------------------------------------------------------------
// Comparisons — CStringView vs const char*
// ---------------------------------------------------------------------------

TEST(CStringViewCompareCStr, EqualityEqual) {
    CStringView csv{"hello"};
    // cppcheck-suppress knownConditionTrueFalse
    EXPECT_TRUE(csv == "hello");
    // cppcheck-suppress knownConditionTrueFalse
    EXPECT_TRUE("hello" == csv);
}

TEST(CStringViewCompareCStr, EqualityNotEqual) {
    CStringView csv{"hello"};
    EXPECT_FALSE(csv == "world");
    EXPECT_FALSE("world" == csv);
}

TEST(CStringViewCompareCStr, NullCStrNotEqual) {
    CStringView csv{"hello"};
    const char* np = nullptr;
    EXPECT_FALSE(csv == np);
    EXPECT_FALSE(np == csv);
}

TEST(CStringViewCompareCStr, NullCStrEmptyViewNotEqual) {
    CStringView csv;
    const char* np = nullptr;
    EXPECT_FALSE(csv == np);
    EXPECT_FALSE(np == csv);
}

// ---------------------------------------------------------------------------
// Hash
// ---------------------------------------------------------------------------

TEST(CStringViewHash, ConsistentWithStringViewHash) {
    CStringView csv{"hello"};
    std::string_view sv{"hello"};
    EXPECT_EQ(std::hash<std::string_view>{}(sv), std::hash<CStringView>{}(csv));
}

TEST(CStringViewHash, EmptyConsistentWithStringViewHash) {
    CStringView csv;
    std::string_view sv{""};
    EXPECT_EQ(std::hash<std::string_view>{}(sv), std::hash<CStringView>{}(csv));
}

TEST(CStringViewHash, UsableInUnorderedSet) {
    std::unordered_set<CStringView> s;
    CStringView a{"hello"};
    CStringView b{"world"};
    s.insert(a);
    s.insert(b);
    EXPECT_EQ(2U, s.size());
    EXPECT_NE(s.end(), s.find(CStringView{"hello"}));
    EXPECT_NE(s.end(), s.find(CStringView{"world"}));
    EXPECT_EQ(s.end(), s.find(CStringView{"other"}));
}

// ---------------------------------------------------------------------------
// NUL-termination invariant across all operations
// ---------------------------------------------------------------------------

TEST(CStringViewNulTermination, AfterSubstr) {
    CStringView sv{"hello"};
    auto s = sv.substr(1);
    EXPECT_EQ('\0', s.c_str()[s.size()]);
}

TEST(CStringViewNulTermination, AfterRemovePrefix) {
    CStringView sv{"hello"};
    sv.remove_prefix(3);
    EXPECT_EQ('\0', sv.c_str()[sv.size()]);
}

TEST(CStringViewNulTermination, AfterRemovePrefixAll) {
    CStringView sv{"hi"};
    sv.remove_prefix(2);
    EXPECT_EQ('\0', sv.c_str()[sv.size()]);
}

TEST(CStringViewNulTermination, FromStringAfterModification) {
    std::string s{"hello"};
    CStringView csv{s};
    // After constructing, s may reallocate — but the view holds the old pointer.
    // Just verify the invariant at construction time.
    EXPECT_EQ('\0', csv.c_str()[csv.size()]);
}

// ---------------------------------------------------------------------------
// Edge cases
// ---------------------------------------------------------------------------

TEST(CStringViewEdge, SingleChar) {
    CStringView sv{"x"};
    EXPECT_EQ(1U, sv.size());
    EXPECT_EQ('x', sv[0]);
    EXPECT_EQ('x', sv.front());
    EXPECT_EQ('x', sv.back());
    EXPECT_EQ('\0', sv.c_str()[1]);
}

TEST(CStringViewEdge, EmbeddedNulInPtrLenThrows) {
    // Claiming len=5 but the NUL terminator is at position 3 — buf[5] != '\0'
    const char buf[8] = {'a', 'b', 'c', '\0', 'd', 'e', 'f', '\0'};
    // buf[5] == 'e', not '\0'
    EXPECT_THROW(CStringView(buf, 5), std::invalid_argument);
}

TEST(CStringViewEdge, CopyAndAssign) {
    CStringView a{"hello"};
    const char* const ptr = a.c_str();
    CStringView b = a;
    EXPECT_EQ(a, b);
    // cppcheck-suppress knownConditionTrueFalse
    EXPECT_EQ(ptr, b.c_str()); // non-owning: copy shares the same pointer

    CStringView c;
    c = a;
    EXPECT_EQ(a, c);
}

TEST(CStringViewEdge, NposValue) {
    EXPECT_EQ(static_cast<CStringView::size_type>(-1), CStringView::npos);
}
} // namespace mb::ul
