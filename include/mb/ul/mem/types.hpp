//! \file

#ifndef TYPES_H_EROUI342ZT347G8HX3XG713H4
#define TYPES_H_EROUI342ZT347G8HX3XG713H4

#include "mb/ul/buildenv/macros.hpp"
#include <cstddef>
#include <ostream>

namespace mb::ul::mem {
const size_t kb = 1'000;
constexpr size_t mb = 1'000 * kb;
constexpr size_t gb = 1'000 * mb;

const size_t kib = 1'024;
constexpr size_t mib = 1'024 * kb;
constexpr size_t gib = 1'024 * mb;

struct Bytes {
    size_t value{};

    constexpr Bytes& operator+=(Bytes rhs) noexcept {
        value += rhs.value;
        return *this;
    }

    constexpr Bytes& operator-=(Bytes rhs) noexcept {
        value -= rhs.value;
        return *this;
    }

    constexpr explicit operator bool() const noexcept {
        return value;
    }
};

constexpr Bytes operator+(Bytes a, Bytes b) noexcept {
    a += b;
    return a;
}

constexpr Bytes operator-(Bytes a, Bytes b) noexcept {
    a -= b;
    return a;
}

constexpr bool operator==(Bytes lhs, Bytes rhs) noexcept {
    return lhs.value == rhs.value;
}

constexpr bool operator!=(Bytes lhs, Bytes rhs) noexcept {
    return !operator==(lhs, rhs);
}

constexpr bool operator<(Bytes lhs, Bytes rhs) noexcept {
    return lhs.value < rhs.value;
}

constexpr bool operator>(Bytes lhs, Bytes rhs) noexcept {
    return operator<(rhs, lhs);
}

constexpr bool operator<=(Bytes lhs, Bytes rhs) noexcept {
    return !operator>(lhs, rhs);
}

constexpr bool operator>=(Bytes lhs, Bytes rhs) noexcept {
    return !operator<(lhs, rhs);
}

namespace impl {
class ThousandsSep : public std::numpunct<char> {
protected:
    [[nodiscard]] char do_thousands_sep() const override {
        return '\'';
    }

    [[nodiscard]] std::string do_grouping() const override {
        return "\003";
    }
};
} // namespace impl

inline std::ostream& operator<<(std::ostream& os, Bytes x) {
    // strangely under Windows this crashes (Mac the same or otherwise malfunctioning)
#if UL_HAS_NOCRASH_IMBUE_LOCALE
    os.imbue(std::locale(os.getloc(), new impl::ThousandsSep));
#endif
    os << x.value << " B";
    return os;
}
} // namespace mb::ul::mem

#endif
