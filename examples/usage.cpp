#include "gsl/gsl" // NOLINT(misc-include-cleaner)
#include "mb/ul/ul.hpp" // NOLINT(misc-include-cleaner)
#include "utf8/core.h"

#include <cstdint>
#include <exception>
#include <iostream>
#include <limits>
#include <print>
#include <string>

int main() {
    try {
        // random feature from ul
        constexpr auto example_nr{static_cast<int32_t>(std::numeric_limits<int8_t>::max())};
        std::println(
            "narrowing {} int to int8_t: {}",
            example_nr,
            ul::narrow<int8_t>(example_nr)); // NOLINT(misc-include-cleaner)

        // random feature from gsl
        gsl::owner<int*> p{new int{42}}; // NOLINT // NOSONAR
        const auto _{ul::finally([&p]() { // NOLINT(misc-include-cleaner)
            delete p; // NOLINT // NOSONAR
        })};
        UL_ASSERT(p); // NOLINT(misc-include-cleaner)
        std::println("p: {}", *p);

        // random feature from utf8 (via ul/string)
        const std::string s = "\xe4\xf6\xfc"; // latin1 (aeoeue)
        std::print("{} valid utf8: {}", s, utf8::is_valid(s.begin(), s.end()));

        return ul::prog_exit_success; // NOLINT(misc-include-cleaner)
    } catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << "\n";
        return ul::prog_exit_failure; // NOLINT(misc-include-cleaner)
    }
}
