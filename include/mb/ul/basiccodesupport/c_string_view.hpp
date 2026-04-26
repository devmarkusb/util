#ifndef C_STRING_VIEW_HPP_NFAGIYUGFYEWNGFY64359254
#define C_STRING_VIEW_HPP_NFAGIYUGFYEWNGFY64359254

#include <cassert>
#include <compare>
#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <string>
#include <string_view>

namespace mb::ul {
namespace detail {
[[noreturn]] inline void throw_invalid([[maybe_unused]] const char* msg) {
#if defined(__cpp_exceptions)
    throw std::invalid_argument{msg};
#else
    std::abort();
#endif
}

[[noreturn]] inline void throw_out_of_range([[maybe_unused]] const char* msg) {
#if defined(__cpp_exceptions)
    throw std::out_of_range{msg};
#else
    std::abort();
#endif
}
} // namespace detail

/// A non-owning view of a \0-terminated C string.
/// Similar to std::string_view but guarantees \0 termination for c_str().
class CStringView final {
public:
    using value_type = char;
    using pointer = const char*;
    using const_pointer = const char*;
    using reference = const char&;
    using const_reference = const char&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using iterator = const char*;
    using const_iterator = const char*;

    static constexpr size_type npos = std::string_view::npos;

    /// Constructs an empty CStringView (points to a stable empty string literal).
    constexpr CStringView() noexcept
        : sv_("", 0) {
    }

    /// Constructs from a \0-terminated C string pointer.
    /// Precondition: s != nullptr and s is \0-terminated.
    explicit CStringView(const char* s) {
        if (!s)
            detail::throw_invalid("CStringView: null pointer");
        sv_ = std::string_view(s, std::strlen(s));
    }

    /// Constructs from pointer+length, validating \0 termination at s[len].
    CStringView(const char* s, size_type len) {
        if (!s) {
            if (len == 0) {
                sv_ = std::string_view("", 0);
                return;
            }
            detail::throw_invalid("CStringView: null pointer with nonzero length");
        }
        if (s[len] != '\0')
            detail::throw_invalid("CStringView: buffer not \0-terminated at given length");
        sv_ = std::string_view(s, len);
    }

    /// Constructs from a char array (string literal).
    template <size_type N>
    /*implicit*/ constexpr CStringView(const char (&arr)[N]) noexcept // cppcheck-suppress noExplicitConstructor
        : sv_(arr, N - 1) {
        static_assert(N > 0, "CStringView: zero-length array not supported");
        assert(arr[N - 1] == '\0' && "CStringView: array must be \0-terminated");
    }

    /// Constructs from std::string (guaranteed \0-terminated since C++11).
    explicit CStringView(const std::string& s) noexcept
        : sv_(s.c_str(), s.size()) {
    }

    /// Constructs from std::string_view ONLY if \0-terminated at view end.
    explicit CStringView(std::string_view sv) {
        if (sv.data() == nullptr) {
            if (sv.size() == 0) {
                sv_ = std::string_view("", 0);
                return;
            }
            detail::throw_invalid("CStringView: string_view has null data with nonzero size");
        }
        if (sv.data()[sv.size()] != '\0')
            detail::throw_invalid("CStringView: string_view is not \0-terminated at end");
        sv_ = sv;
    }

    // Observers
    constexpr const char* data() const noexcept {
        return sv_.data();
    }

    constexpr const char* c_str() const noexcept {
        return sv_.data();
    }

    constexpr size_type size() const noexcept {
        return sv_.size();
    }

    constexpr size_type length() const noexcept {
        return sv_.size();
    }

    constexpr bool empty() const noexcept {
        return sv_.empty();
    }

    constexpr const_reference operator[](size_type i) const noexcept {
        return sv_[i];
    }

    const_reference at(size_type i) const {
        if (i >= sv_.size())
            detail::throw_out_of_range("CStringView: index out of range");
        return sv_[i];
    }

    constexpr const_reference front() const noexcept {
        assert(!sv_.empty());
        return sv_.front();
    }

    constexpr const_reference back() const noexcept {
        assert(!sv_.empty());
        return sv_.back();
    }

    // Iteration
    constexpr const_iterator begin() const noexcept {
        return sv_.data();
    }

    constexpr const_iterator end() const noexcept {
        return sv_.data() + sv_.size();
    }

    constexpr const_iterator cbegin() const noexcept {
        return begin();
    }

    constexpr const_iterator cend() const noexcept {
        return end();
    }

    // Conversions
    constexpr std::string_view sv() const noexcept {
        return sv_;
    }

    constexpr operator std::string_view() const noexcept {
        return sv_;
    }

    std::string to_string() const {
        return std::string(sv_);
    }

    // Operations
    /// substr(pos): suffix starting at pos; always \0-terminated.
    CStringView substr(size_type pos) const {
        if (pos > sv_.size())
            detail::throw_invalid("CStringView::substr: pos out of range");
        return CStringView(sv_.substr(pos), Trusted{});
    }

    /// substr(pos, count): only valid when pos+count == size() (suffix of exact length).
    CStringView substr(size_type pos, size_type count) const {
        if (pos > sv_.size())
            detail::throw_invalid("CStringView::substr: pos out of range");
        const size_type rcount = (count == npos) ? (sv_.size() - pos) : count;
        if (rcount > sv_.size() - pos)
            detail::throw_invalid("CStringView::substr: count out of range");
        if (pos + rcount != sv_.size())
            detail::throw_invalid("CStringView::substr: requested substring is not \0-terminated");
        return CStringView(sv_.substr(pos, rcount), Trusted{});
    }

    /// Remove prefix (remains \0-terminated as a suffix of the original).
    void remove_prefix(size_type n) {
        if (n > sv_.size())
            detail::throw_invalid("CStringView::remove_prefix: out of range");
        sv_.remove_prefix(n);
    }

    /// Removing a suffix would break \0-termination.
    void remove_suffix(size_type) = delete;

    // Find (delegates to string_view)
    size_type find(char ch, size_type pos = 0) const noexcept {
        return sv_.find(ch, pos);
    }

    size_type find(std::string_view needle, size_type pos = 0) const noexcept {
        return sv_.find(needle, pos);
    }

    // Comparisons — CStringView is a 16-byte view type, passed by value like std::string_view
    // cppcheck-suppress passedByValue
    friend bool operator==(CStringView a, CStringView b) noexcept {
        return a.sv_ == b.sv_;
    }

    // cppcheck-suppress passedByValue
    friend std::strong_ordering operator<=>(CStringView a, CStringView b) noexcept {
        const auto cmp = a.sv_.compare(b.sv_);
        if (cmp < 0)
            return std::strong_ordering::less;
        if (cmp > 0)
            return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }

    // cppcheck-suppress passedByValue
    friend bool operator==(CStringView a, std::string_view b) noexcept {
        return a.sv_ == b;
    }

    // cppcheck-suppress passedByValue
    friend bool operator==(std::string_view a, CStringView b) noexcept {
        return a == b.sv_;
    }

    // cppcheck-suppress passedByValue
    friend bool operator==(CStringView a, const char* b) noexcept {
        return b ? a.sv_ == std::string_view(b) : false;
    }

    // cppcheck-suppress passedByValue
    friend bool operator==(const char* a, CStringView b) noexcept {
        return a ? std::string_view(a) == b.sv_ : false;
    }

private:
    struct Trusted {};

    explicit CStringView(std::string_view sv, Trusted) noexcept
        : sv_{sv} {
    }

    std::string_view sv_;
};
} // namespace mb::ul

// Hash support
namespace std {
template <>
struct hash<mb::ul::CStringView> {
    size_t operator()(const mb::ul::CStringView& v) const noexcept {
        return std::hash<std::string_view>{}(v.sv());
    }
};
} // namespace std

#endif
