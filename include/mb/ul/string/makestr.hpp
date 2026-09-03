//! \file From David Rodr?guez - dribeas, stackoverflow.


#ifndef MAKESTR_H_92NZR29H3F8
#define MAKESTR_H_92NZR29H3F8

#include "mb/ul/buildenv/macros.hpp"
#include <sstream>
#include <string>

namespace mb::ul::str {
//! Usage: f( (makestr() << "Some string" << sSome_string << iSomeInt << dSomeDouble << "...").c_str() );
class Makestr {
public:
    template <typename T>
    Makestr& operator<<(const T& data) UL_LIFETIMEBOUND {
        buffer_ << data; // NOLINT
        return *this;
    }

    // NOLINTNEXTLINE(cppcoreguidelines-explicit-constructor, misc-explicit-constructor)
    /*implicit*/ operator std::string() const {
        return buffer_.str();
    }

private:
    std::ostringstream buffer_;
};
} // namespace mb::ul::str

#endif
