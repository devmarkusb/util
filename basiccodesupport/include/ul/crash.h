#ifndef CRASH_H_SIUHFNGYOG6842KJBFTF87643
#define CRASH_H_SIUHFNGYOG6842KJBFTF87643

#include "ul/config.h"
#include "ul/warnings.h"
#include <csignal>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>

namespace mb::ul {
/** Pass e.g. SIGABRT, SIGSEGV as signal, 0 for no crash, try_indirect false definitely raises the passed signal,
    otherwise it tries to simulate real errors.*/
inline void crash(int signal, bool try_indirect = true) {
#if UL_OS_MAC
    // the below procedure just didn't do it there yet
    if (signal == SIGABRT)
        try_indirect = false;
#endif
    if (!try_indirect) {
        const auto success{std::raise(signal)};
        if (!success) {
            std::string msg{"couldn't raise signal, returned: "};
            msg.append(std::to_string(success));
            throw std::runtime_error{msg};
        }
    } else {
        // NOLINTBEGIN
        UL_PRAGMA_WARNINGS_PUSH
        UL_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC
        UL_WARNING_DISABLE_CLANG_ALL
        // clang-format off
        UL_WARNING_DISABLE_GCC(stringop-overflow)
        // clang-format on
        if (signal == SIGABRT) {
            // try heap corruption, crashing may depend on heap implementation
            for (size_t i = 1; i < 512; ++i) {
                auto* mem = reinterpret_cast<size_t*>(std::malloc(64));
                std::memset(mem - i, 0, i);
                std::free(mem);
            }
        } else if (signal == SIGSEGV) {
            const auto* foo = reinterpret_cast<int*>(-1);
            std::printf("%d\n", *foo);
        }
        // NOLINTEND
        UL_PRAGMA_WARNINGS_POP
    }
}
} // namespace mb::ul

UL_HEADER_END

#endif
